#include "Legit.hpp"

#include "imgui.h"

#include "../../Interfaces.hpp"

#include "../../GameCache.hpp"
#include "../../Utils/Raytrace.hpp"
#include "../../Utils/Trigonometry.hpp"
#include "../../Utils/WeaponConfig/WeaponConfig.hpp"

#include <algorithm>
#include <cmath>

static bool enabled = false;
// TODO Input

struct LegitAimbotWeaponConfig {
	float fov = 3.0f;
	float smoothness = 4.0f;
	int maximalInfluence = 1;

	DECLARE_SERIALIZER(Serializer);
};
void WeaponGUI(LegitAimbotWeaponConfig& LegitAimbotWeaponConfig);

static WeaponConfigurator<LegitAimbotWeaponConfig> weaponConfigurator(WeaponGUI);

static int maximalFlashAmount = 0;
static bool dontAimThroughSmoke = true;
static bool friendlyFire = false;

void Features::Legit::Aimbot::PollEvent(SDL_Event* event)
{
	if (!enabled)
		return;

	if (event->type != SDL_MOUSEMOTION)
		return;

	if (!Interfaces::engine->IsInGame())
		return;

	CBasePlayer* localPlayer = GameCache::GetLocalPlayer();
	if (!localPlayer || !localPlayer->IsAlive())
		return;

	if (!IsParticipatingTeam(*localPlayer->Team()))
		return;

	if (localPlayer->GetFlashAlpha() > (float)maximalFlashAmount)
		return;

	auto* combatWeapon = reinterpret_cast<CBaseCombatWeapon*>(Interfaces::entityList->GetClientEntityFromHandle(localPlayer->ActiveWeapon()));
	if (!combatWeapon)
		return;

	if (!IsFirearm(*combatWeapon->WeaponDefinitionIndex()))
		return;

	LegitAimbotWeaponConfig* LegitAimbotWeaponConfig = weaponConfigurator.GetConfig(*combatWeapon->WeaponDefinitionIndex());
	if (!LegitAimbotWeaponConfig)
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
		auto* player = reinterpret_cast<CBasePlayer*>(Interfaces::entityList->GetClientEntity(i));
		if (!player || player == localPlayer || player->GetDormant() || !player->IsAlive() || *player->GunGameImmunity())
			continue;
		if (!IsParticipatingTeam(*player->Team()))
			continue;

		if (!(friendlyFire || player->IsEnemy(localPlayer)))
			continue;

		Matrix3x4* boneMatrix = player->SetupBones();

		const Vector head = boneMatrix[8].Origin();

		if (dontAimThroughSmoke && Memory::LineGoesThroughSmoke(playerEye, head, 1))
			continue;

		const Trace trace = Utils::TraceRay(playerEye, head, &filter);

		if (trace.m_pEnt != player)
			continue; // The enemy is behind something...

		Vector rotation = Utils::CalculateView(playerEye, head);
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

	if (bestRotation.Length() > LegitAimbotWeaponConfig->fov)
		return;

	bestRotation /= LegitAimbotWeaponConfig->smoothness;

	const Vector before = Vector((float)event->motion.xrel, (float)event->motion.yrel, 0);
	const Vector goal = Vector(-round(bestRotation.y), round(bestRotation.x), 0);

	const float dir = before.Normalized().Dot(goal.Normalized());
	if (dir < 0)
		return; // We are trying to aim away

	event->motion.xrel += std::clamp((int)goal.x, -LegitAimbotWeaponConfig->maximalInfluence, LegitAimbotWeaponConfig->maximalInfluence);
	event->motion.yrel += std::clamp((int)goal.y, -LegitAimbotWeaponConfig->maximalInfluence, LegitAimbotWeaponConfig->maximalInfluence);
}

void WeaponGUI(LegitAimbotWeaponConfig& LegitAimbotWeaponConfig)
{
	ImGui::SliderFloat(xorstr_("FOV"), &LegitAimbotWeaponConfig.fov, 0.0f, 10.0f, xorstr_("%.2f"));
	ImGui::SliderFloat(xorstr_("Smoothness"), &LegitAimbotWeaponConfig.smoothness, 1.0f, 5.0f, xorstr_("%.2f"));
	ImGui::SliderInt(xorstr_("Maximal influence"), &LegitAimbotWeaponConfig.maximalInfluence, 1, 5);
}

void Features::Legit::Aimbot::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);

	ImGui::SliderInt(xorstr_("Maximal flash amount"), &maximalFlashAmount, 0, 255);
	ImGui::Checkbox(xorstr_("Don't aim through smoke"), &dontAimThroughSmoke);
	ImGui::Checkbox(xorstr_("Friendly fire"), &friendlyFire);

	weaponConfigurator.SetupGUI();
}

BEGIN_SERIALIZED_STRUCT(LegitAimbotWeaponConfig::Serializer)
SERIALIZED_TYPE(xorstr_("FOV"), fov)
SERIALIZED_TYPE(xorstr_("Smoothness"), smoothness)
SERIALIZED_TYPE(xorstr_("Maximal influence"), maximalInfluence)
END_SERIALIZED_STRUCT

BEGIN_SERIALIZED_STRUCT(Features::Legit::Aimbot::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Maximal flash amount"), maximalFlashAmount)
SERIALIZED_TYPE(xorstr_("Don't aim through smoke"), dontAimThroughSmoke)
SERIALIZED_TYPE(xorstr_("Friendly fire"), friendlyFire)

SERIALIZED_STRUCTURE(xorstr_("Weapons"), weaponConfigurator)
END_SERIALIZED_STRUCT
