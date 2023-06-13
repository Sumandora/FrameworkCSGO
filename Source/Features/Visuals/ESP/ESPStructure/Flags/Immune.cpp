#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

std::optional<std::string> Immune::GetText(const Player& player) const
{
	if (player.immune)
		return xorstr_("Immune");
	return std::nullopt;
}