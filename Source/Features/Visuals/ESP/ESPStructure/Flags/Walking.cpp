#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

std::optional<std::string> Walking::GetText(const Player& player) const
{
	if (player.walking)
		return "Walking";
	else
		return std::nullopt;
}