#include "../ESPStructure.hpp"

#include "../../../../../Interfaces.hpp"

bool SpectatorSettings::IsEnabled() const
{
	return boxName.IsEnabled();
}

void SpectatorSettings::Draw(ImDrawList* drawList, Spectator& spectator) const
{
	if (!IsEnabled())
		return;

	const Player* spectated = nullptr;

	if (onlyShowWhenSpectatedEntityIsDormant || displayNameOfSpectatedEntity) {
		Player* player = EntityCache::PlayerByHandle(spectator.observerTarget);
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

	const std::optional<ImVec4> rectangle = spectator.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	boxName.Draw(drawList, rectangle.value(), name);
}

void SpectatorSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);

	ImGui::Checkbox(xorstr_("Only show when spectated entity is dormant"), &onlyShowWhenSpectatedEntityIsDormant);
	ImGui::Checkbox(xorstr_("Display name of spectated entity"), &displayNameOfSpectatedEntity);
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(SpectatorSettings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_TYPE(xorstr_("Only show when spectated entity is dormant"), onlyShowWhenSpectatedEntityIsDormant)
SERIALIZED_TYPE(xorstr_("Display name of spectated entity"), displayNameOfSpectatedEntity)
END_SERIALIZED_STRUCT