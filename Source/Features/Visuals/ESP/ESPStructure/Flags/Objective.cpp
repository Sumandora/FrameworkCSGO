#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

// TODO Different colors?

std::optional<std::string> Objective::GetText(const Player& player) const
{
	if (player.defusing)
		return xorstr_("Defusing");
	else if (player.grabbingHostage)
		return xorstr_("Grabbing Hostage");
	else if (player.rescuing)
		return xorstr_("Rescuing");

	return std::nullopt;
}