#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

std::optional<std::string> Spotted::getText(const Player& player) const
{
	if (player.spotted)
		return "Spotted";
	else
		return std::nullopt;
}