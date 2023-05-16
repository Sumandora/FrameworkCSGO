#include "Features/Visuals/ESP/EntityCache/EntityCache.hpp"

void Hostage::Update(CHostage* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::Update(entity, index, handle, clientClass);

	state = *entity->HostageState();
	grabTime = *entity->GrabSuccessTime();
}
