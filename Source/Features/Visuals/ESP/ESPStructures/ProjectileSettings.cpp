#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../Hooks/Game/GameFunctions.hpp"
#include "../../Visuals.hpp"

static std::map<ClientClassID, const char*> projectileNames{
	{ ClientClassID::CBreachChargeProjectile, strdup(xorstr_("Breach charge")) },
	{ ClientClassID::CBumpMineProjectile, strdup(xorstr_("Bump Mine")) },
	{ ClientClassID::CDecoyProjectile, strdup(xorstr_("Decoy")) },
	{ ClientClassID::CMolotovProjectile, strdup(xorstr_("Molotov")) },
	{ ClientClassID::CSensorGrenadeProjectile, strdup(xorstr_("Sensor grenade")) },
	{ ClientClassID::CSmokeGrenadeProjectile, strdup(xorstr_("Smoke grenade")) },
	{ ClientClassID::CSnowballProjectile, strdup(xorstr_("Snowball")) }
};

void ProjectileSettings::Draw(ImDrawList* drawList, ImVec4 rectangle, const Projectile& projectile) const
{
	const char* flashbang = xorstr_("Flashbang");
	const char* highExplosiveGrenade = xorstr_("High Explosive Grenade");
	const char* name = xorstr_("Unknown projectile");
	if (boxName.nametag.enabled) {
		if (projectile.clientClass->m_ClassID == ClientClassID::CBaseCSGrenadeProjectile) {
			if (strstr(projectile.model->szPathName, xorstr_("flashbang")))
				name = flashbang;
			else
				name = highExplosiveGrenade;
		} else if (projectileNames.contains(projectile.clientClass->m_ClassID)) {
			name = projectileNames[projectile.clientClass->m_ClassID];
		}
	}

	boxName.Draw(drawList, rectangle, name);

	std::vector<ImVec2> points;

	for (const Vector& pos : projectile.trail) {
		ImVec2 point2D;
		if (!Features::Visuals::Esp::WorldToScreen(Hooks::Game::FrameStageNotify::worldToScreenMatrix, pos, point2D)) {
			if (!points.empty()) {
				trail.Draw(drawList, points);
				points.clear();
			}
			continue;
		}
		points.push_back(point2D);
	}

	trail.Draw(drawList, points);
}

void ProjectileSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	trail.SetupGUI(xorstr_("Trail"));
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(ProjectileSettings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Trail"), trail)
END_SERIALIZED_STRUCT