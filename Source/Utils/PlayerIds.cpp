#include "PlayerIds.hpp"

#include "../Interfaces.hpp"

int Utils::GetEntityId(CBaseEntity* entity)
{
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		if (Interfaces::entityList->GetClientEntity(i) == entity) {
			return i;
		}
	}
}