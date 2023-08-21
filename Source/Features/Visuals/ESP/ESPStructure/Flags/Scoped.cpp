#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <optional>
#include <string>

std::optional<std::string> Scoped::getText(const Player& player) const
{
	if (player.scoped)
		return "Scoped";
	else
		return std::nullopt;
}