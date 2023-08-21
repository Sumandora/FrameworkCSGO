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

	// This system could be combined with the esp structure system, however that would complicate many things, since this is not rendering elements

	struct GlowSettings {
		bool enabled = false;
		ImColor color = ImGuiColors::white;
		bool filled = false;
		int style = 0;

		bool apply(int entindex, CBaseEntity* entity) const;
		void setupGUI();
		SERIALIZER();
	};

	struct Player { // TODO Health based
		GlowSettings teammate{};
		GlowSettings enemy{};
		GlowSettings local{};

		bool apply(int entindex, CBasePlayer* player) const;
		void setupGUI();
		SERIALIZER();
	} players{};

	GlowSettings weapons{};
	GlowSettings ragdolls{};

	// TODO Glow other things?

	std::map<int, int> customGlows{}; // These are added glow objects, which have to be removed later

private:
	bool updateGlow(int entindex, CBaseEntity* entity) const;

public:
	void updateGlow();

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} glow;

#endif