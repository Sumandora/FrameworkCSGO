#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

std::optional<std::string> Planting::GetText(const Player& player) const
{
	if (player.planting)
		return xorstr_("Planting");

	return std::nullopt;
}