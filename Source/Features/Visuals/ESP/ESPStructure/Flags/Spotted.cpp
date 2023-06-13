#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

std::optional<std::string> Spotted::GetText(const Player& player) const
{
	if (player.spotted)
		return xorstr_("Spotted");

	return std::nullopt;
}