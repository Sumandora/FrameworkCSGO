#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

std::optional<std::string> HasDefuser::getText(const Player& player) const
{
	if (player.hasDefuser)
		return "Has defuser";
	else
		return std::nullopt;
}