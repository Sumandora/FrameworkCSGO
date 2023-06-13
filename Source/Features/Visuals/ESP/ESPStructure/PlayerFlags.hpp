#ifndef FEATURES_VISUALS_ESP_PLAYERFLAGS
#define FEATURES_VISUALS_ESP_PLAYERFLAGS

#include "../EntityCache/EntityCache.hpp"

#include "../../../../GUI/ImGuiColors.hpp"
#include "../../../../Serialization/Serializer.hpp"

#include "imgui.h"
#include "xorstr.hpp"

#include <optional>
#include <string>

class Flag {
public:
	bool enabled = false;
	ImColor color = ImGuiColors::white;

	virtual float GetAlpha(const Player& player) const { return 1.0f; }
	virtual std::optional<std::string> GetText(const Player& player) const = 0;

	virtual std::string GetName() const = 0;

	BEGIN_SERIALIZED_STRUCT(Serializer)
	SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
	SERIALIZED_TYPE(xorstr_("Color"), color)
	END_SERIALIZED_STRUCT
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
DECLARE_FLAG(Money, xorstr_("Money"))
DECLARE_FLAG(Scoped, xorstr_("Scoped"))
DECLARE_FLAG(PinPulled, xorstr_("Pin pulled"))
DECLARE_FLAG(Location, xorstr_("Location"))
DECLARE_FLAG(Objective, xorstr_("Objective"))
DECLARE_FLAG(HasDefuser, xorstr_("Has defuser"))
DECLARE_FLAG(HasBomb, xorstr_("Has bomb"))
DECLARE_FLAG(Planting, xorstr_("Planting"))
DECLARE_FLAG(Spotted, xorstr_("Spotted"))
DECLARE_FLAG(Ammo, xorstr_("Ammo"))
DECLARE_FLAG(Immune, xorstr_("Immune"))
DECLARE_FLAG(Walking, xorstr_("Walking"))

DECLARE_ALPHA_FLAG(Flashed, xorstr_("Flashed"))

#endif