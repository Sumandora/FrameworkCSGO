#ifndef FEATURES_LEGIT_AIMBOT
#define FEATURES_LEGIT_AIMBOT

#include "../Features.hpp"

#include "../../Utils/WeaponConfig/WeaponConfig.hpp"

#include "imgui.h"

#include "SDL.h"

inline class LegitAimbot : public Feature {
public:
	LegitAimbot()
		: Feature("Legit", "Aimbot")
	{
	}

	bool enabled = false;
	// TODO Input

	struct WeaponConfig : public BaseWeaponConfig {
		float fov = 3.0f;
		float smoothness = 4.0f;
		int maximalInfluence = 1;

		virtual void setupGUI() override;
		virtual SERIALIZER() override;
	};

	WeaponConfigurator<WeaponConfig> weaponConfigurator;

	int maximalFlashAmount = 0;
	bool dontAimThroughSmoke = true;
	bool friendlyFire = false;

	void pollEvent(SDL_Event* event);

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} legitAimbot;

#endif