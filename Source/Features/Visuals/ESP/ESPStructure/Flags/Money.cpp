#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <string>

std::optional<std::string> Money::GetText(const Player& player) const
{
	return xorstr_("$") + std::to_string(player.account);
}