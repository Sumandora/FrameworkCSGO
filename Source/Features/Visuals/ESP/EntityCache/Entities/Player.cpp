#include "../EntityCache.hpp"

#include "../../../../../Interfaces.hpp"
#include "../../../../../Utils/Raytrace.hpp"
#include "../../ESP.hpp"

#include "../../../../../SDK/GameClass/Entities/CBaseCSGrenade.hpp"
#include "../../../../../SDK/GameClass/Entities/CC4.hpp"

bool IsVisible(CBasePlayer* localPlayer, CBasePlayer* otherPlayer)
{
	if (esp.considerEveryoneVisibleWhenDead && !localPlayer->IsAlive())
		return true;

	Matrix3x4* boneMatrix = otherPlayer->SetupBones();

	const Vector playerEye = localPlayer->GetEyePosition();
	const Vector head = boneMatrix[8].Origin();

	if (esp.considerSmokedOffEntitiesAsOccluded && Memory::LineGoesThroughSmoke(playerEye, head, 1))
		return false;

	CTraceFilterEntity filter(localPlayer);
	const Trace trace = Utils::TraceRay(playerEye, head, &filter);

	return trace.m_pEnt == otherPlayer;
}

void Player::Update(CBasePlayer* entity, int index, const CBaseHandle& handle, ClientClass* clientClass)
{
	Entity::Update(entity, index, handle, clientClass);

	activeWeapon = WeaponID::WEAPON_NONE;
	CBaseCombatWeapon* weaponEntity = nullptr;
	if (*entity->ActiveWeapon() != invalidEHandleIndex) {
		weaponEntity = static_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(entity->ActiveWeapon()));
		if (weaponEntity) {
			activeWeapon = *weaponEntity->WeaponDefinitionIndex();
			ammo = *weaponEntity->Ammo();
			reserveAmmoCount = *weaponEntity->ReserveAmmoCount();
		}
	}
	health = *entity->Health();
	flashAlpha = entity->GetFlashAlpha();

	std::optional<LocalPlayer> localPlayer = EntityCache::localPlayer;
	if (localPlayer.has_value()) {
		auto* backingLocalPlayerEntity = static_cast<CBasePlayer*>(localPlayer.value().backingEntity);
		enemy = entity->IsEnemy(backingLocalPlayerEntity);

		if (esp.considerSpottedEntitiesAsVisible && spotted)
			visible = true;
		else
			visible = IsVisible(backingLocalPlayerEntity, entity);
	} else {
		enemy = false;

		visible = true;
	}

	alive = entity->IsAlive();

	dormant = entity->GetDormant();

	account = *entity->Account();
	scoped = *entity->Scoped();
	if (IsGrenade(activeWeapon)) {
		pinPulled = *static_cast<CBaseCSGrenade*>(weaponEntity)->PinPulled();
	} else
		pinPulled = false;

	// The place name can't be longer than this, if the game changes however, we just copy MAX_PLACE_NAME_LENGTH bytes even if it is more than that
	// In case you are here wondering why sometimes place names get cut off, check if the MAX_PLACE_NAME_LENGTH changed
	strncpy(location, entity->LastPlaceName(), MAX_PLACE_NAME_LENGTH);

	defusing = *entity->IsDefusing();
	grabbingHostage = *entity->IsGrabbingHostage();
	rescuing = *entity->IsRescuing();

	hasDefuser = *entity->HasDefuser();

	hasBomb = false;
	planting = false;
	for (unsigned short i = 0; i < MAX_WEAPONS; i++) {
		CBaseCombatWeapon* weaponEntity = (CBaseCombatWeapon*)Interfaces::entityList->GetClientEntityFromHandle(entity->Weapons() + i);
		if (!weaponEntity)
			continue;

		if (*weaponEntity->WeaponDefinitionIndex() == WeaponID::WEAPON_C4) {
			hasBomb = true;
			planting = *static_cast<CC4*>(weaponEntity)->StartedArming();
			break;
		}
	}

	immune = *entity->GunGameImmunity();
	walking = *entity->IsWalking();
}
