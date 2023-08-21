#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

std::optional<std::string> Location::getText(const Player& player) const
{
	if (strlen(player.location) > 0)
		return player.location;
	return std::nullopt;
}