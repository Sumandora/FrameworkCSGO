#include "Features/Visuals/ESP/EntityCache/EntityCache.hpp"

void Spectator::Update(CBasePlayer* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Player::Update(entity, index, handle, clientClass);

	observerMode = *entity->ObserverMode();
	observerTarget = *entity->ObserverTarget();
}
