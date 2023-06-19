#include "../GameFunctions.hpp"

#include "../../../Features/Semirage/Aimbot.hpp"

void Hooks::Game::FireEvent::HookFunc(void* thisptr, CGameEvent* event, bool bServerOnly, bool bClientOnly)
{
#ifdef DEBUG
	const char* name = event->GetName();
	if (!events.contains(name))
		events[name] = 0;
	events[name]++;
#endif
	semirageAimbot.FireEvent(event);

	return InvokeFunction<void, void*, CGameEvent*, bool, bool>(hook->proxy, thisptr, event, bServerOnly, bClientOnly);
}
