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

	screenRectangle.Update(origin, boundingBox);

	markForRemoval = false; // We just got updated, don't remove us, we are still alive!
}
