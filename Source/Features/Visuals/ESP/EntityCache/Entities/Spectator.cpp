#include "../EntityCache.hpp"

void Spectator::update(CBasePlayer* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Player::update(entity, index, handle, clientClass);

	observerMode = *entity->ObserverMode();
	observerTarget = *entity->ObserverTarget();
}
