#include "Features/Visuals/ESP/EntityCache/EntityCache.hpp"

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

	model = entity->GetModel();
}
