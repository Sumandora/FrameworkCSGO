#include "../GameFunctions.hpp"

#include <cstdio>
#include <cstring>

#include "../../../Features/Misc/IgnoreSvPure.hpp"

bool Hooks::Game::SendNetMsg::HookFunc(void* thisptr, INetMessage* msg, bool bForceReliable, bool bVoice)
{
	if (ignoreSvPure.cancelNetMessage && std::strcmp(msg->GetName(), "CCLCMsg_FileCRCCheck") == 0)
		return false;
	return InvokeFunction<bool, void*, INetMessage*, bool, bool>(hook->proxy, thisptr, msg, bForceReliable, bVoice);
}
