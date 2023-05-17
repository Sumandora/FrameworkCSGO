#include "../EntityCache.hpp"

#include "../../../../../Interfaces.hpp"

void Projectile::Update(CBaseEntity* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::Update(entity, index, handle, clientClass);

	trail.push_back(*entity->Origin());
}
