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
	bool enabled;
	ImColor color;

	inline Flag(ImColor defaultColor)
		: enabled(false)
		, color(defaultColor){};

	virtual float GetAlpha(const Player& player) const { return 1.0f; }
	virtual std::optional<std::string> GetText(const Player& player) const = 0;

	virtual std::string GetName() const = 0;

	BEGIN_SERIALIZED_STRUCT(Serializer)
	SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
	SERIALIZED_TYPE(xorstr_("Color"), color)
	END_SERIALIZED_STRUCT
};

#define DECLARE_FLAG(flagName, defaultColor)                                             \
	class flagName : public Flag {                                                       \
	public:                                                                              \
		inline flagName()                                                                \
			: Flag(defaultColor)                                                         \
		{                                                                                \
		}                                                                                \
		virtual std::optional<std::string> GetText(const Player& player) const override; \
                                                                                         \
		virtual std::string GetName() const override                                     \
		{                                                                                \
			return xorstr_(#flagName);                                                   \
		}                                                                                \
	};

#define DECLARE_ALPHA_FLAG(flagName, defaultColor)                                       \
	class flagName : public Flag {                                                       \
	public:                                                                              \
		inline flagName()                                                                \
			: Flag(defaultColor)                                                         \
		{                                                                                \
		}                                                                                \
		virtual float GetAlpha(const Player& player) const override;                     \
		virtual std::optional<std::string> GetText(const Player& player) const override; \
                                                                                         \
		virtual std::string GetName() const override                                     \
		{                                                                                \
			return xorstr_(#flagName);                                                   \
		}                                                                                \
	};

DECLARE_FLAG(Money, ImGuiColors::green)
DECLARE_FLAG(Scoped, ImGuiColors::blue)
DECLARE_FLAG(PinPulled, ImGuiColors::green)

DECLARE_ALPHA_FLAG(Flashed, ImGuiColors::white)

#endif