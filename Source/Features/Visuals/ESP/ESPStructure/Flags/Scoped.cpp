#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <optional>
#include <string>

std::optional<std::string> Scoped::GetText(const Player& player) const
{
	if (player.scoped)
		return xorstr_("Scoped");
	return std::nullopt;
}