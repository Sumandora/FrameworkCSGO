#include "../../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"

PlantedC4Settings::PlantedC4Settings(const char* id)
	: id(id)
	, boxName(BoxNameSetting(id))
	, timer(TextSetting(strdup(xorstr_("Timer"))))
{
}

void PlantedC4Settings::Draw(ImDrawList* drawList, ImVec4 rectangle, CPlantedC4* bomb)
{
	boxName.Draw(drawList, rectangle, xorstr_("Planted C4"));
	if (!*bomb->Defused())
		timer.Draw(drawList, rectangle, std::to_string(*bomb->BombTime() - Memory::globalVars->curtime).c_str(), 1.0f);
}

void PlantedC4Settings::SetupGUI()
{
	ImGui::PushID(id);
	boxName.SetupGUI();
	timer.SetupGUI();
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlantedC4Settings::Serializer, id)
SERIALIZED_STRUCTURE(boxName)
SERIALIZED_STRUCTURE(timer)
END_SERIALIZED_STRUCT