#include "../EntityCache.hpp"

void Drone::Update(CDrone* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::Update(entity, index, handle, clientClass);

	target = *entity->MoveToThisEntity();
}
