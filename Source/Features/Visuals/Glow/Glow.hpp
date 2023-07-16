#ifndef FEATURES_VISUALS_GLOW
#define FEATURES_VISUALS_GLOW

#include "../../Features.hpp"

#include "imgui.h"

#include "../../../SDK/GameClass/Entities/CBaseGrenade.hpp"
#include "../../../SDK/GameClass/Entities/CBasePlayer.hpp"

#include "../../../GUI/ImGuiColors.hpp"

#include "../../../Utils/VisibilityChecker.hpp"

#include <vector>

inline class Glow : public Feature {
public:
	Glow()
		: Feature("Visuals", "Glow")
	{
	}
	~Glow();

	bool enabled = false;
	VisibilityChecker visibilityChecker;

	// This system could be combined with the esp structure system, however that would complicate many things, since this is not rendering elements

	struct GlowSettings {
		bool enabled = false;
		ImColor color = ImGuiColors::white;
		bool filled = false;
		int style = 0;

		void Apply(int entindex, CBaseEntity* entity);
		void SetupGUI();
		SERIALIZER();
	};

	struct Player { // TODO Health based
		GlowSettings teammate{};
		GlowSettings enemy{};
		GlowSettings local{};

		void Apply(int entindex, CBasePlayer* player);
		void SetupGUI();
		SERIALIZER();
	} playerSettings{};

	GlowSettings weapons{};
	GlowSettings ragdolls{};

	// TODO Glow other things?

	std::map<int, int> customGlows{}; // These are added glow objects, which have to be removed later

	void UpdateGlow();

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} glow;

#endif