#include "Memory.hpp"

#include "Interfaces.hpp"

#include "BCRL.hpp"
#include "ldisasm.h"
#include "RetAddrSpoofer.hpp"
#include "SDK/GameClass/CGlowObjectManager.hpp"
#include "SDK/GameClass/Panorama/CUIEngine.hpp"
#include "SignatureScanner.hpp"

#include "SDK/GameClass/CGlobalVars.hpp"
#include "SDK/GameClass/Entities/CBasePlayer.hpp"
#include "SDK/GameClass/IMoveHelper.hpp"
#include <cstdint>

void* RetAddrSpoofer::leaveRet = nullptr;

static void* lineGoesThroughSmoke;
static CBasePlayer** localPlayerList;

int32_t CUIEngine::panelArrayOffset;

void Memory::Create()
{
	// Set the address for the return address spoofer
	RetAddrSpoofer::leaveRet = BCRL::Session::ArrayPointer(Interfaces::baseClient, 0) // random code piece
								   .NextByteOccurence("c9 c3")
								   .Pointer()
								   .value();

	clientMode = BCRL::Session::ArrayPointer(Interfaces::baseClient, 10)
					 .Add(12)
					 .RelativeToAbsolute()
					 .Add(4)
					 .RelativeToAbsolute()
					 .Dereference()
					 .Pointer()
					 .value();

	globalVars = static_cast<CGlobalVars*>(BCRL::Session::ArrayPointer(Interfaces::baseClient, 11)
											   .Add(16)
											   .RelativeToAbsolute()
											   .Dereference()
											   .Pointer()
											   .value());

	// cinput = BCRL::Session::ArrayPointer(Interfaces::baseClient, 16)
	// 			 .Add(3)
	// 			 .RelativeToAbsolute()
	// 			 .Dereference()
	// 			 .Pointer()
	// 			 .value();

	// If this index changes I'm mad bro...
	// To find the method, just search for the moveHelper and look at all usages
	// The method which contains the 1.25 and 1.0 float literals is the one...
	moveHelper
		= static_cast<IMoveHelper*>(BCRL::Session::ArrayPointer(Interfaces::gameMovement, 69)
										.NextByteOccurence("48 8d 05" /* lea rax */)
										.Add(3)
										.RelativeToAbsolute()
										.Dereference()
										.Pointer()
										.value());

	// Has reference to "splitscreenplayer" inside itself
	localPlayerList = static_cast<CBasePlayer**>(
		BCRL::Session::ArrayPointer(clientMode, 55)
			.NextByteOccurence("48 89 85 10 d2 ff ff")
			.Add(8)
			.RelativeToAbsolute()
			.Add(7)
			.RelativeToAbsolute()
			.Pointer()
			.value());

	lineGoesThroughSmoke = BCRL::Session::Module("client_client.so")
							   .NextStringOccurence("sv_show_voip_indicator_for_enemies")
							   .FindXREFs("client_client.so", true, false)
							   .ForEach([](BCRL::SafePointer& safePointer) { safePointer = safePointer.Add(4); })
							   .Repeater([](BCRL::SafePointer& pointer) {
								   if (pointer.DoesMatch("e8 ?? ?? ?? ?? 84 c0")) {
									   BCRL::SafePointer newPointer = pointer.Add(1).RelativeToAbsolute();
									   if (newPointer.DoesMatch("55 48 89 e5 41 56 41 55")) {
										   pointer = newPointer;
										   return false;
									   }
								   }

								   pointer = pointer.NextInstruction();
								   return true;
							   })
							   .Pointer()
							   .value();

	CUIEngine::panelArrayOffset = *static_cast<int32_t*>(
		BCRL::Session::ArrayPointer(
			Interfaces::panoramaUIEngine->AccessUIEngine(),
			CUIEngine::isValidPanelPointerIndex)
			.NextByteOccurence("48 8b 97")
			.Add(3)
			.Pointer()
			.value());

	glowObjectManager = static_cast<CGlowObjectManager*>(
		BCRL::Session::Module("client_client.so")
			.NextStringOccurence("Music.StopAllExceptMusic")
			.FindXREFs("client_client.so", true, false)
			.Add(4)
			.Filter([](const BCRL::SafePointer& safePointer) { return safePointer.DoesMatch("66 0f ef c0"); })
			.Repeater(6, [](BCRL::SafePointer& safePointer) { safePointer = safePointer.NextInstruction(); })
			.Add(1)
			.RelativeToAbsolute()
			.Repeater(2, [](BCRL::SafePointer& safePointer) { safePointer = safePointer.NextInstruction(); })
			.Add(3)
			.RelativeToAbsolute()
			.Pointer()
			.value());
}

bool Memory::LineGoesThroughSmoke(const Vector& from, const Vector& to, const short _)
{
	// Little explanation why I make this struct here:
	// GCC for some reason decides that pushing the from and to Vector over general purpose registers is a good idea.
	// It basically creates pointers for these which are then pushed.
	// We want the XMM registers though, so it ends up completely destroying this function.
	// I create these VectorStructs in order to make GCC think, that XMM registers are the better choice.
	struct VectorStruct {
		float x, y, z;
	};

	VectorStruct fromStruct{};
	fromStruct.x = from.x;
	fromStruct.y = from.y;
	fromStruct.z = from.z;

	VectorStruct toStruct{};
	toStruct.x = to.x;
	toStruct.y = to.y;
	toStruct.z = to.z;

	return InvokeFunction<bool, VectorStruct, VectorStruct, short>(lineGoesThroughSmoke, fromStruct, toStruct, _);
}

CBasePlayer* Memory::GetLocalPlayer()
{
	return localPlayerList[0]; // There are no split screens in csgo, so safely assume we never want to have something else than 0
}
