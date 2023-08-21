#include "Memory.hpp"

#include "Interfaces.hpp"

#include <cstdint>

#include "BCRL.hpp"
#include "ldisasm.h"
#include "RetAddrSpoofer.hpp"
#include "SDK/GameClass/CGlowObjectManager.hpp"
#include "SDK/GameClass/Panorama/CUIEngine.hpp"
#include "SignatureScanner.hpp"

#include "SDK/GameClass/CGlobalVars.hpp"
#include "SDK/GameClass/Entities/CBasePlayer.hpp"
#include "SDK/GameClass/IMoveHelper.hpp"

void* RetAddrSpoofer::leaveRet = nullptr;

static void* lineGoesThroughSmoke;
static CBasePlayer** localPlayerList;

int32_t CUIEngine::panelArrayOffset;

void Memory::Create()
{
	// Set the address for the return address spoofer
	RetAddrSpoofer::leaveRet = BCRL::Session::arrayPointer(Interfaces::baseClient, 0) // random code piece
								   .nextByteOccurence("c9 c3", true)
								   .getPointer()
								   .value();

	clientMode = BCRL::Session::arrayPointer(Interfaces::baseClient, 10)
					 .add(12)
					 .relativeToAbsolute()
					 .add(4)
					 .relativeToAbsolute()
					 .dereference()
					 .getPointer()
					 .value();

	globalVars = static_cast<CGlobalVars*>(BCRL::Session::arrayPointer(Interfaces::baseClient, 11)
											   .add(16)
											   .relativeToAbsolute()
											   .dereference()
											   .getPointer()
											   .value());

	// cinput = BCRL::Session::arrayPointer(Interfaces::baseClient, 16)
	// 			 .add(3)
	// 			 .relativeToAbsolute()
	// 			 .dereference()
	// 			 .getPointer()
	// 			 .value();

	// If this index changes I'm mad bro...
	// To find the method, just search for the moveHelper and look at all usages
	// The method which contains the 1.25 and 1.0 float literals is the one...
	moveHelper
		= static_cast<IMoveHelper*>(BCRL::Session::arrayPointer(Interfaces::gameMovement, 69)
										.nextByteOccurence("48 8d 05" /* lea rax */)
										.add(3)
										.relativeToAbsolute()
										.dereference()
										.getPointer()
										.value());

	// Has reference to "splitscreenplayer" inside itself
	localPlayerList = static_cast<CBasePlayer**>(
		BCRL::Session::arrayPointer(clientMode, 55)
			.nextByteOccurence("48 89 85 10 d2 ff ff")
			.add(8)
			.relativeToAbsolute()
			.add(7)
			.relativeToAbsolute()
			.getPointer()
			.value());

	lineGoesThroughSmoke = BCRL::Session::module("client_client.so")
							   .nextStringOccurence("sv_show_voip_indicator_for_enemies")
							   .findXREFs("client_client.so", true, false)
							   .forEach([](BCRL::SafePointer& safePointer) { safePointer = safePointer.add(4); })
							   .repeater([](BCRL::SafePointer& pointer) {
								   if (pointer.doesMatch("e8 ?? ?? ?? ?? 84 c0")) {
									   BCRL::SafePointer newPointer = pointer.add(1).relativeToAbsolute();
									   if (newPointer.doesMatch("55 48 89 e5 41 56 41 55")) {
										   pointer = newPointer;
										   return false;
									   }
								   }

								   pointer = pointer.nextInstruction();
								   return true;
							   })
							   .getPointer()
							   .value();

	CUIEngine::panelArrayOffset = *static_cast<int32_t*>(
		BCRL::Session::arrayPointer(Interfaces::panoramaUIEngine->AccessUIEngine(), CUIEngine::isValidPanelPointerIndex)
			.nextByteOccurence("48 8b 97")
			.add(3)
			.getPointer()
			.value());

	glowObjectManager = static_cast<CGlowObjectManager*>(
		BCRL::Session::module("client_client.so")
			.nextStringOccurence("Music.StopAllExceptMusic")
			.findXREFs("client_client.so", true, false)
			.add(4)
			.filter([](const BCRL::SafePointer& safePointer) { return safePointer.doesMatch("66 0f ef c0"); })
			.repeater(6, [](BCRL::SafePointer& safePointer) { safePointer = safePointer.nextInstruction(); })
			.add(1)
			.relativeToAbsolute()
			.repeater(2, [](BCRL::SafePointer& safePointer) { safePointer = safePointer.nextInstruction(); })
			.add(3)
			.relativeToAbsolute()
			.getPointer()
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
