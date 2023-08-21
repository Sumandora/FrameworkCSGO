#include "../EntityCache.hpp"

void Drone::update(CDrone* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::update(entity, index, handle, clientClass);

	target = *entity->MoveToThisEntity();
}
