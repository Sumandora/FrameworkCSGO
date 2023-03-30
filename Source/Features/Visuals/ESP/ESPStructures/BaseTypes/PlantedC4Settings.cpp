#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../Interfaces.hpp"

PlantedC4Settings::PlantedC4Settings()
	: boxName(BoxNameSetting())
	, timer(TextSetting())
	, showDefuseTimer(false)
{
}

void PlantedC4Settings::Draw(ImDrawList* drawList, ImVec4 rectangle, CPlantedC4* bomb) const
{
	boxName.Draw(drawList, rectangle, xorstr_("Planted C4"));
	if (!*bomb->Defused() && *bomb->BombTicking()) {
		if(*bomb->BombTime() < Memory::globalVars->curtime)
			return; // You can't defuse the bomb anymore
		float middle = rectangle.x + (rectangle.z - rectangle.x) * 0.5f;
		timer.Draw(drawList, middle, rectangle.w, true, std::to_string(*bomb->BombTime() - Memory::globalVars->curtime).c_str());
		if(showDefuseTimer && Interfaces::entityList->GetClientEntityFromHandle(bomb->Defuser()))
			timer.Draw(drawList, middle, rectangle.w + timer.GetLineHeight(), true, std::to_string(*bomb->DefuseCountDown() - Memory::globalVars->curtime).c_str());
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
SERIALIZED_STRUCTURE(boxName, name)
SERIALIZED_STRUCTURE(timer, xorstr_("Timer"))

SERIALIZED_TYPE("Show defuse timer", showDefuseTimer)
END_SERIALIZED_STRUCT