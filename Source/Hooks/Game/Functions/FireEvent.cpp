#include "../GameFunctions.hpp"

#include "../../../Features/Misc/VoteRevealer.hpp"
#include "../../../Features/Semirage/Aimbot.hpp"

void Hooks::Game::FireEvent::hookFunc(void* thisptr, CGameEvent* event, bool bServerOnly, bool bClientOnly)
{
	if (!event) // I'm not sure if this can happen, but if it does then I don't wanna mess with the event for sure
		return invokeFunction<void, void*, CGameEvent*, bool, bool>(hook->proxy, thisptr, event, bServerOnly, bClientOnly);

#ifdef DEBUG
	const char* name = event->GetName();
	if (!events.contains(name))
		events[name] = 0;
	events[name]++;
#endif

	semirageAimbot.fireEvent(event);
	voteRevealer.fireEvent(event);

	return invokeFunction<void, void*, CGameEvent*, bool, bool>(hook->proxy, thisptr, event, bServerOnly, bClientOnly);
}
