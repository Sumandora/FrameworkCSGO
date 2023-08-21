#ifndef FEATURES_SEMIRAGE_AIMBOT
#define FEATURES_SEMIRAGE_AIMBOT

#include "../Features.hpp"

#include "../../Utils/WeaponConfig/WeaponConfig.hpp"

#include "imgui.h"

#include "../../SDK/GameClass/CGameEvent.hpp"
#include "../../SDK/GameClass/CUserCmd.hpp"
#include "../../SDK/GameClass/Entities/CBasePlayer.hpp"

#include "../../GUI/Elements/Keybind.hpp"
#include "../../GUI/ImGuiColors.hpp"

inline class SemirageAimbot : public Feature {
public:
	SemirageAimbot()
		: Feature("Semirage", "Aimbot")
	{
	}

	bool enabled = false;
	bool autoFire = false;
	Input autoFireKey{};
	// TODO Only when scoped
	// TODO Bones
	// TODO ignore when jumping

	// TODO Better computation of recoil

	struct WeaponConfig : public BaseWeaponConfig {
		bool disabled = false;

		bool autoFireRecklessly = false;

		bool onlyWhenShooting = false;

		float fov = 0.0f;
		float fovScaleX = 1.0f;
		float fovScaleY = 1.0f;

		bool controlRecoil = true;
		float recoilScaleX = 1.0f;
		float recoilScaleY = 1.0f;

		bool smoothOut = false;
		float horizontalAimSpeed = 0.2f;
		float verticalAimSpeed = 0.2f;

		bool silent = false;

		bool smoothRotateToOrigin = false;
		float horizontalRotateToOriginSpeed = 0.2f;
		float verticalRotateToOriginSpeed = 0.2f;
		float recombineViews = 0.1f;

		virtual void setupGUI() override;
		virtual SERIALIZER() override;
	};

	WeaponConfigurator<WeaponConfig> weaponConfigurator;

	int maximalFlashAmount = 255;
	bool dontAimThroughSmoke = false;
	bool friendlyFire = false;

	bool fovCircle = false; // Technically an ellipse if you use FOV scaling
	ImColor circleColor = ImGuiColors::white;
	float thickness = 1.0f;

	bool showDesyncedView = false;
	ImColor viewColor = ImGuiColors::red;
	float radius = 5.0f;

private:
	WeaponConfig* getWeaponConfig(CBasePlayer* localPlayer);
	bool shouldAttackPlayer(CBasePlayer* localPlayer, CBasePlayer* player);
	bool canPointBeSeen(CBasePlayer* localPlayer, CBasePlayer* otherPlayer);
	CBasePlayer* findTarget(CBasePlayer* localPlayer, const Vector& viewAngles);
	bool shouldAim(CBasePlayer* localPlayer, WeaponConfig* weaponConfig, bool attacking);
	bool performAutoFire(WeaponConfig* weaponConfig, bool hasTarget, CBasePlayer* localPlayer, const Vector& viewangles);
	void calculateAimTarget(CBasePlayer* localPlayer);

public:
	void fireEvent(CGameEvent* gameEvent);
	bool createMove(CUserCmd* cmd);
	void imGuiRender(ImDrawList* drawList);

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} semirageAimbot;

#endif