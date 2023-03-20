#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../Interfaces.hpp"

PlantedC4Settings::PlantedC4Settings(const char* id)
	: id(id)
	, boxName(BoxNameSetting(id))
	, timer(TextSetting(strdup(xorstr_("Timer"))))
	, showDefuseTimer(false)
{
}

void PlantedC4Settings::Draw(ImDrawList* drawList, ImVec4 rectangle, CPlantedC4* bomb) const
{
	boxName.Draw(drawList, rectangle, xorstr_("Planted C4"));
	if (!*bomb->Defused() && *bomb->BombTicking()) {
		float middle = rectangle.x + (rectangle.z - rectangle.x) * 0.5f;
		timer.Draw(drawList, middle, rectangle.w, true, std::to_string(*bomb->BombTime() - Memory::globalVars->curtime).c_str());
		if(showDefuseTimer && Interfaces::entityList->GetClientEntityFromHandle(bomb->Defuser()))
			timer.Draw(drawList, middle, rectangle.w + timer.GetLineHeight(), true, std::to_string(*bomb->DefuseCountDown() - Memory::globalVars->curtime).c_str());
	}
}

void PlantedC4Settings::SetupGUI()
{
	ImGui::PushID(id);
	boxName.SetupGUI();
	timer.SetupGUI();
	ImGui::Checkbox(xorstr_("Show defuse timer"), &showDefuseTimer);
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlantedC4Settings::Serializer, id)
SERIALIZED_STRUCTURE(boxName)
SERIALIZED_STRUCTURE(timer)

SERIALIZED_TYPE("Show defuse timer", showDefuseTimer)
END_SERIALIZED_STRUCT