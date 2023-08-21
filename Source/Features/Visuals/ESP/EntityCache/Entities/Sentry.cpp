#include "../EntityCache.hpp"

void Sentry::update(CDronegun* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::update(entity, index, handle, clientClass);

	health = *entity->Health();
}
