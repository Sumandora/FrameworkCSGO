#include "../EntityCache.hpp"

#include "../../../../../Interfaces.hpp"

LocalPlayer::LocalPlayer(CBasePlayer* entity, int index)
{
	// We don't need to most stuff here
	Player::Update(entity, -1, index, nullptr);

	observerMode = *entity->ObserverMode();
	observerTarget = *entity->ObserverTarget();

	Interfaces::engine->GetViewAngles(&viewangles);
}
