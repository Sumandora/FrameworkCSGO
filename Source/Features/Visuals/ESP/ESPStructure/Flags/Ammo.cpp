#include "../PlayerFlags.hpp"

#include "imgui.h"
#include <cstring>
#include <optional>
#include <string>

// TODO Don't show reserve ammo count

std::optional<std::string> Ammo::GetText(const Player& player) const
{
	const int ammoClip = player.ammo;
	if (ammoClip != -1) { // Does this weapon even have an ammo capacity? (Knifes, Bombs etc...)
		return std::to_string(ammoClip) + '/' + std::to_string(player.reserveAmmoCount);
	}
	return std::nullopt;
}