#include "../EntityCache.hpp"

void Entity::Update(CBaseEntity* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	this->backingEntity = entity;
	this->index = index;
	this->handle = handle;
	this->clientClass = clientClass;

	team = *entity->Team();
	origin = *entity->Origin();

	boundingBox = entity->EntityBounds();

	spotted = *entity->Spotted();
	dormant = entity->GetDormant();

	std::optional<LocalPlayer> localPlayer = EntityCache::GetLocalPlayer();
	if(localPlayer.has_value())
		distanceToLocal = (*entity->Origin() - localPlayer->origin).Length();

	model = entity->GetModel();
}
