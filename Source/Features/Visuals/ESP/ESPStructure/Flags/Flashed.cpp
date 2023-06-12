#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <algorithm>
#include <string>

float Flashed::GetAlpha(const Player& player) const
{
	return std::clamp(player.flashAlpha / 255.0f, 0.0f, 1.0f);
}

std::optional<std::string> Flashed::GetText(const Player& player) const
{
	if (player.flashAlpha <= 0)
		return std::nullopt;
	return xorstr_("Flashed: ") + std::to_string((int)player.flashAlpha);
}