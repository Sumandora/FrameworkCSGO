#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

// TODO Different colors?

std::optional<std::string> Objective::getText(const Player& player) const
{
	if (player.defusing)
		return "Defusing";
	else if (player.grabbingHostage)
		return "Grabbing Hostage";
	else if (player.rescuing)
		return "Rescuing";
	else
		return std::nullopt;
}