#include "Features/Visuals/ESP/ESPStructure.hpp"

#include <iomanip>

#include "xorstr.hpp"

#include "GUI/Elements/ClickableColorButton.hpp"

HostageSettings::HostageSettings()
	: accuracy(1)
{
}

void HostageSettings::Draw(ImDrawList* drawList, ImVec4 rectangle, const Hostage& hostage) const
{
	boxName.Draw(drawList, rectangle, xorstr_("Hostage"));

	if (hostage.state == 1) { // Is the hostage being picked up?
		const float countDown = hostage.grabTime - Memory::globalVars->curtime;
		std::stringstream stream;
		stream << std::fixed << std::setprecision(accuracy) << countDown;
		timer.Draw(drawList, rectangle.x + (rectangle.z - rectangle.x) * 0.5f, rectangle.w, true, stream.str().c_str());
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

BEGIN_SERIALIZED_STRUCT(HostageSettings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Timer"), timer)

SERIALIZED_TYPE(xorstr_("Accuracy"), accuracy)
END_SERIALIZED_STRUCT