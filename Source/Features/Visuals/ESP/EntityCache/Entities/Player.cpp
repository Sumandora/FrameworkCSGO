#include "../EntityCache.hpp"

#include "../../../../../Interfaces.hpp"
#include "../../../../../Utils/Raytrace.hpp"
#include "../../../Visuals.hpp"

bool IsVisible(CBasePlayer* localPlayer, CBasePlayer* otherPlayer)
{
	Matrix3x4* boneMatrix = otherPlayer->SetupBones();

	const Vector playerEye = localPlayer->GetEyePosition();
	const Vector head = boneMatrix[8].Origin();

	if (Features::Visuals::Esp::considerSmokedOffEntitiesAsOccluded && Memory::LineGoesThroughSmoke(playerEye, head, 1))
		return false;

	CTraceFilterEntity filter(localPlayer);
	const Trace trace = Utils::TraceRay(playerEye, head, &filter);

	return trace.m_pEnt == otherPlayer;
}

void Player::Update(CBasePlayer* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::Update(entity, index, handle, clientClass);

	activeWeapon = WeaponID::WEAPON_NONE;
	if (*entity->ActiveWeapon() != INVALID_EHANDLE_INDEX) {
		CBaseEntity* weaponEntity = Interfaces::entityList->GetClientEntityFromHandle(entity->ActiveWeapon());
		if (weaponEntity)
			activeWeapon = *reinterpret_cast<CBaseAttributableItem*>(weaponEntity)->WeaponDefinitionIndex();
	}
	health = *entity->Health();
	flashAlpha = entity->GetFlashAlpha();

	std::optional<LocalPlayer> localPlayer = EntityCache::localPlayer;
	if (localPlayer.has_value()) {
		auto* backingLocalPlayerEntity = reinterpret_cast<CBasePlayer*>(localPlayer.value().backingEntity);
		enemy = entity->IsEnemy(backingLocalPlayerEntity);

		if (Features::Visuals::Esp::considerSpottedEntitiesAsVisible && spotted)
			visible = true;
		else
			visible = IsVisible(entity, backingLocalPlayerEntity);
	} else {
		enemy = false;

		visible = true;
	}

	alive = entity->IsAlive();

	dormant = entity->GetDormant();
}
