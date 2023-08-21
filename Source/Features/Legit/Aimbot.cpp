#include "Aimbot.hpp"

#include "../../Interfaces.hpp"

#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"

#include <algorithm>
#include <cmath>

void LegitAimbot::pollEvent(SDL_Event* event)
{
	if (!enabled)
		return;

	if (event->type != SDL_MOUSEMOTION)
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = Memory::getLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return;

	if (localPlayer->GetFlashAlpha() > (float)maximalFlashAmount)
		return;

	auto* combatWeapon = static_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));
	if (!combatWeapon)
		return;

	if (!IsFirearm(*combatWeapon->WeaponDefinitionIndex()))
		return;

	WeaponConfig* weaponConfig = weaponConfigurator.getConfig(*combatWeapon->WeaponDefinitionIndex());
	if (!weaponConfig)
		return;

	const Vector playerEye = localPlayer->GetEyePosition();

	Vector viewAngles;
	Interfaces::engine->GetViewAngles(&viewAngles);

	CBasePlayer* target = nullptr;
	float bestDistance{};
	Vector bestRotation;

	CTraceFilterEntity filter(localPlayer);

	// The first object is always the WorldObj
	for (int i = 1; i < Interfaces::engine->GetMaxClients(); i++) {
		auto* player = static_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player || player == localPlayer || player->GetDormant() || !player->IsAlive() || *player->GunGameImmunity())
			continue;
		if (!IsParticipatingTeam(*player->Team()))
			continue;

		if (!(friendlyFire || player->IsEnemy(localPlayer)))
			continue;

		Matrix3x4* boneMatrix = player->SetupBones();

		const Vector head = boneMatrix[8].Origin();

		if (dontAimThroughSmoke && Memory::lineGoesThroughSmoke(playerEye, head, 1))
			continue;

		const Trace trace = Utils::traceRay(playerEye, head, &filter);

		if (trace.m_pEnt != player)
			continue; // The enemy is behind something...

		Vector rotation = Utils::calculateView(playerEye, head);
		rotation -= *localPlayer->AimPunchAngle() * 2.0f;
		rotation -= viewAngles;
		rotation = rotation.Wrap();

		const float delta = rotation.Length();
		if (!target || bestDistance > delta) {
			target = player;
			bestDistance = delta;
			bestRotation = rotation;
		}
	}

	if (!target)
		return;

	if (bestRotation.Length() > weaponConfig->fov)
		return;

	bestRotation /= weaponConfig->smoothness;

	const Vector before = Vector((float)event->motion.xrel, (float)event->motion.yrel, 0);
	const Vector goal = Vector(-round(bestRotation.y), round(bestRotation.x), 0);

	const float dir = before.Normalized().Dot(goal.Normalized());
	if (dir < 0)
		return; // We are trying to aim away

	event->motion.xrel += std::clamp((int)goal.x, -weaponConfig->maximalInfluence, weaponConfig->maximalInfluence);
	event->motion.yrel += std::clamp((int)goal.y, -weaponConfig->maximalInfluence, weaponConfig->maximalInfluence);
}

void LegitAimbot::WeaponConfig::setupGUI()
{
	ImGui::SliderFloat("FOV", &fov, 0.0f, 10.0f, "%.2f");
	ImGui::SliderFloat("Smoothness", &smoothness, 1.0f, 5.0f, "%.2f");
	ImGui::SliderInt("Maximal influence", &maximalInfluence, 1, 5);
}

void LegitAimbot::setupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);

	ImGui::SliderInt("Maximal flash amount", &maximalFlashAmount, 0, 255);
	ImGui::Checkbox("Don't aim through smoke", &dontAimThroughSmoke);
	ImGui::Checkbox("Friendly fire", &friendlyFire);

	weaponConfigurator.setupGUI();
}

SCOPED_SERIALIZER(LegitAimbot::WeaponConfig)
{
	SERIALIZE("FOV", fov);
	SERIALIZE("Smoothness", smoothness);
	SERIALIZE("Maximal influence", maximalInfluence);
}

SCOPED_SERIALIZER(LegitAimbot)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Maximal flash amount", maximalFlashAmount);
	SERIALIZE("Don't aim through smoke", dontAimThroughSmoke);
	SERIALIZE("Friendly fire", friendlyFire);

	SERIALIZE_STRUCT("Weapons", weaponConfigurator);
}
