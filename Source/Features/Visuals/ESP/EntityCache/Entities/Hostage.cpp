#include "../EntityCache.hpp"

void Hostage::update(CHostage* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::update(entity, index, handle, clientClass);

	state = *entity->HostageState();
	grabTime = *entity->GrabSuccessTime();
}
