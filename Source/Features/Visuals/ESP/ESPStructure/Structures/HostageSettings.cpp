#include "../ESPStructure.hpp"
#include <iomanip>

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

HostageSettings::HostageSettings()
	: accuracy(1)
{
}

bool HostageSettings::IsEnabled() const
{
	return boxName.IsEnabled() || timer.enabled;
}

void HostageSettings::Draw(ImDrawList* drawList, Hostage& hostage) const
{
	if (!IsEnabled())
		return;

	const std::optional<ImVec4> rectangle = hostage.screenRectangle.Get();
	if (!rectangle.has_value())
		return;

	boxName.Draw(drawList, rectangle.value(), xorstr_("Hostage"));

	if (hostage.state == 1) { // Is the hostage being picked up?
		const float countDown = hostage.grabTime - Memory::globalVars->curtime;
		std::stringstream stream;
		stream << std::fixed << std::setprecision(accuracy) << countDown;
		timer.Draw(drawList, rectangle->x + (rectangle->z - rectangle->x) * 0.5f, rectangle->w, true, stream.str().c_str());
	}
}

void HostageSettings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	timer.SetupGUI(xorstr_("Timer"));
	if (timer.enabled)
		ImGui::SliderInt(xorstr_("Accuracy"), &accuracy, 1, 5);
	ImGui::PopID();
}

SCOPED_SERIALIZER(HostageSettings)
{
	SERIALIZE_STRUCT(xorstr_("Box name"), boxName);
	SERIALIZE_STRUCT(xorstr_("Timer"), timer);

	SERIALIZE(xorstr_("Accuracy"), accuracy);
}