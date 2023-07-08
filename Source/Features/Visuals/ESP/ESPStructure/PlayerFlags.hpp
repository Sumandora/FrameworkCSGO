#ifndef FEATURES_VISUALS_ESP_ESPSTRUCTURE_PLAYERFLAGS
#define FEATURES_VISUALS_ESP_ESPSTRUCTURE_PLAYERFLAGS

#include "../EntityCache/EntityCache.hpp"

#include "../../../../GUI/ImGuiColors.hpp"
#include "../../../../Serialization/Serializer.hpp"

#include "imgui.h"

#include <optional>
#include <string>

class Flag {
public:
	bool enabled = false;
	ImColor color = ImGuiColors::white;

	virtual ~Flag() { }

	virtual float GetAlpha(const Player& player) const { return 1.0f; }
	virtual std::optional<std::string> GetText(const Player& player) const = 0;

	virtual std::string GetName() const = 0;

	SERIALIZER()
	{
		SERIALIZE("Enabled", enabled);
		SERIALIZE_VECTOR4D("Color", color.Value);
	}
};

#define DECLARE_FLAG(flagName, displayName)                                              \
	class flagName : public Flag {                                                       \
	public:                                                                              \
		virtual std::optional<std::string> GetText(const Player& player) const override; \
                                                                                         \
		virtual std::string GetName() const override                                     \
		{                                                                                \
			return displayName;                                                          \
		}                                                                                \
	};

#define DECLARE_ALPHA_FLAG(flagName, displayName)                                        \
	class flagName : public Flag {                                                       \
	public:                                                                              \
		virtual float GetAlpha(const Player& player) const override;                     \
		virtual std::optional<std::string> GetText(const Player& player) const override; \
                                                                                         \
		virtual std::string GetName() const override                                     \
		{                                                                                \
			return displayName;                                                          \
		}                                                                                \
	};

// TODO Generate boilerplate for the "if(check) text else nullopt"-flags
// TODO Flags (Has Armor (m_ArmorValue)/Heavy Armor/Helmet, Is Walking, Is Reloading, Is Bot)
// TODO Flags for non-players?
DECLARE_FLAG(Money, "Money")
DECLARE_FLAG(Scoped, "Scoped")
DECLARE_FLAG(PinPulled, "Pin pulled")
DECLARE_FLAG(Location, "Location")
DECLARE_FLAG(Objective, "Objective")
DECLARE_FLAG(HasDefuser, "Has defuser")
DECLARE_FLAG(HasBomb, "Has bomb")
DECLARE_FLAG(Planting, "Planting")
DECLARE_FLAG(Spotted, "Spotted")
DECLARE_FLAG(Ammo, "Ammo")
DECLARE_FLAG(Immune, "Immune")
DECLARE_FLAG(Walking, "Walking")

DECLARE_ALPHA_FLAG(Flashed, "Flashed")

#endif