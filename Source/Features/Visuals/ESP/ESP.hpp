#ifndef FEATURES_VISUALS_ESP
#define FEATURES_VISUALS_ESP

#include "../../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include "ESPStructure/ESPStructure.hpp"

#include "../../../SDK/GameClass/IMaterial.hpp"

#include "../../../GUI/ImGuiColors.hpp"

inline class ESP : public Feature {
public:
	ESP()
		: Feature(xorstr_("Visuals"), xorstr_("ESP"))
	{
	}

	bool enabled = false;
	int onKey = ImGuiKey_None;
	int drawDistance = 1024 * 8;
	bool considerSpottedEntitiesAsVisible = false;
	bool considerSmokedOffEntitiesAsOccluded = true;
	bool considerEveryoneVisibleWhenDead = false;
	bool alignBoundingBox = true;
	bool outOfView = false;
	float outOfViewSize = 30.0f;
	float outOfViewDistance = 300.0f;
	PlayerSettings players;
	SpectatorSettings spectators;
	WeaponSettings weapons;
	ProjectileTypeSettings projectiles;
	PlantedC4Settings plantedC4;
	HostageSettings hostages;
	LootCrateTypeSettings dzLootCrates;
	GenericEntitySettings dzAmmoBoxes;
	SentrySettings dzSentries;
	DroneSettings dzDrones;
	GenericEntitySettings other;

	void Update();
	void ImGuiRender(ImDrawList* drawList);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} esp;

#endif