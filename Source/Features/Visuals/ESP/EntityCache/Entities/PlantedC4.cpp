#include "../EntityCache.hpp"

void PlantedC4::update(CPlantedC4* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::update(entity, index, handle, clientClass);

	defused = *entity->Defused();
	bombTicking = *entity->BombTicking();
	bombTime = *entity->BombTime();
	defuser = *entity->Defuser();
	defuseCountDown = *entity->DefuseCountDown();
}
