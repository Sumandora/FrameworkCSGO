#include "../ESPStructure.hpp"

#include "../../../../GUI/Elements/Popup.hpp"
#include "../../../General/General.hpp"

void SentrySettings::Draw(ImDrawList* drawList, Sentry& sentry) const
{
	const std::optional<ImVec4> rectangle = sentry.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	boxName.Draw(drawList, rectangle.value(), xorstr_("Sentry"));
	healthbar.Draw(drawList, rectangle.value(), sentry.health, 220);	// MaxHealth is hardcoded as 220 (Revealed by doing "ent_dump Dronegun" in console)
}

void SentrySettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	healthbar.SetupGUI(xorstr_("Healthbar"));
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(SentrySettings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Healthbar"), healthbar)
END_SERIALIZED_STRUCT