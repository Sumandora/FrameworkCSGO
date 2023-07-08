#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <string>

std::optional<std::string> PinPulled::GetText(const Player& player) const
{
	if (player.pinPulled)
		return "Pin pulled";
	else
		return std::nullopt;
}