#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"

PlantedC4Settings::PlantedC4Settings()
	: showDefuseTimer(false)
{
}

void PlantedC4Settings::Draw(ImDrawList* drawList, ImVec4 rectangle, const PlantedC4& bomb) const
{ // TODO Accuracy
	boxName.Draw(drawList, rectangle, xorstr_("Planted C4"));
	if (!bomb.defused && bomb.bombTicking) { // TODO Ask GameRules if the bomb is actually planted
		if (bomb.bombTime < Memory::globalVars->curtime)
			return; // You can't defuse the bomb anymore
		const float middle = rectangle.x + (rectangle.z - rectangle.x) * 0.5f;
		timer.Draw(drawList, middle, rectangle.w, true, std::to_string(bomb.bombTime - Memory::globalVars->curtime).c_str());
		if (showDefuseTimer && bomb.defuser) {
			if (bomb.defuseCountDown < Memory::globalVars->curtime)
				return; // We are done defusing, no point in showing this anymore
			timer.Draw(drawList, middle, rectangle.w + timer.GetLineHeight(), true, std::to_string(bomb.defuseCountDown - Memory::globalVars->curtime).c_str());
		}
	}
}

void PlantedC4Settings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	timer.SetupGUI(xorstr_("Timer"));
	ImGui::Checkbox(xorstr_("Show defuse timer"), &showDefuseTimer);
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlantedC4Settings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Timer"), timer)

SERIALIZED_TYPE(xorstr_("Show defuse timer"), showDefuseTimer)
END_SERIALIZED_STRUCT