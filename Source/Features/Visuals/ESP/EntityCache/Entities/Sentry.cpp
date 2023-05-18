#include "../EntityCache.hpp"

void Sentry::Update(CDronegun* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::Update(entity, index, handle, clientClass);

	health = *entity->Health();
}
