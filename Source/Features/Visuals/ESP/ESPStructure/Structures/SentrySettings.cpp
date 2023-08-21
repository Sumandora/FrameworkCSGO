#include "../ESPStructure.hpp"

#include "../../../../../GUI/Elements/Popup.hpp"

#include "../../../../General/EventLog.hpp"

bool SentrySettings::isEnabled() const
{
	return boxName.isEnabled() || healthbar.enabled;
}

void SentrySettings::draw(ImDrawList* drawList, Sentry& sentry) const
{
	if (!isEnabled())
		return;

	const std::optional<ImVec4> rectangle = sentry.screenRectangle.get();
	if (!rectangle.has_value())
		return;

	boxName.draw(drawList, rectangle.value(), "Sentry");
	healthbar.draw(drawList, rectangle.value(), sentry.health, 220); // MaxHealth is hardcoded as 220 (Revealed by doing "ent_dump Dronegun" in console)
}

void SentrySettings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.setupGUI(id);
	healthbar.setupGUI("Healthbar");
	ImGui::PopID();
}

SCOPED_SERIALIZER(SentrySettings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Healthbar", healthbar);
}