#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

std::optional<std::string> HasBomb::GetText(const Player& player) const
{
	if (player.hasBomb)
		return xorstr_("Has bomb");
	return std::nullopt;
}