#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/Popup.hpp"

#include "../../../../General/EventLog.hpp"

bool SentrySettings::IsEnabled() const
{
	return boxName.IsEnabled() || healthbar.enabled;
}

void SentrySettings::Draw(ImDrawList* drawList, Sentry& sentry) const
{
	if (!IsEnabled())
		return;

	const std::optional<ImVec4> rectangle = sentry.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	boxName.Draw(drawList, rectangle.value(), "Sentry");
	healthbar.Draw(drawList, rectangle.value(), sentry.health, 220); // MaxHealth is hardcoded as 220 (Revealed by doing "ent_dump Dronegun" in console)
}

void SentrySettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	healthbar.SetupGUI("Healthbar");
	ImGui::PopID();
}

SCOPED_SERIALIZER(SentrySettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Healthbar", healthbar);
}