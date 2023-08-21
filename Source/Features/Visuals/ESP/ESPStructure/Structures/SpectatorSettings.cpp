#include "../ESPStructure.hpp"

#include "../../../../../Interfaces.hpp"

bool SpectatorSettings::isEnabled() const
{
	return boxName.isEnabled();
}

void SpectatorSettings::draw(ImDrawList* drawList, Spectator& spectator) const
{
	if (!isEnabled())
		return;

	const Player* spectated = nullptr;

	if (onlyShowWhenSpectatedEntityIsDormant || displayNameOfSpectatedEntity) {
		Player* player = EntityCache::playerByHandle(spectator.observerTarget);
		if (player)
			spectated = player;
	}

	if (onlyShowWhenSpectatedEntityIsDormant && spectated && !spectated->dormant)
		return;

	char name[MAX_NAME_LEN];
	if (boxName.nametag.enabled) { // Don't ask the engine for the name, if we don't have to
		PlayerInfo info{};
		if (Interfaces::engine->GetPlayerInfo(displayNameOfSpectatedEntity && spectated ? spectated->index : spectator.index, &info))
			strcpy(name, info.name);
	}

	const std::optional<ImVec4> rectangle = spectator.screenRectangle.get();
	if (!rectangle.has_value())
		return;

	boxName.draw(drawList, rectangle.value(), name);
}

void SpectatorSettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.setupGUI(id);

	ImGui::Checkbox("Only show when spectated entity is dormant", &onlyShowWhenSpectatedEntityIsDormant);
	ImGui::Checkbox("Display name of spectated entity", &displayNameOfSpectatedEntity);
	ImGui::PopID();
}

SCOPED_SERIALIZER(SpectatorSettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE("Only show when spectated entity is dormant", onlyShowWhenSpectatedEntityIsDormant);
	SERIALIZE("Display name of spectated entity", displayNameOfSpectatedEntity);
}