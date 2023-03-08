#include "../ESPStructure.hpp"

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"

void PlantedC4Settings::Draw(ImDrawList* drawList, ImVec4 rectangle, CPlantedC4* bomb) {
	boxName.Draw(drawList, rectangle, xorstr_("Planted C4"));
	timer.Draw(drawList, rectangle, std::to_string(*bomb->BombTime() - Memory::globalVars->curtime).c_str(), 1.0f);
}

void PlantedC4Settings::SetupGUI(const char* tag) {
	ImGui::PushID(tag);
	boxName.SetupGUI(tag);
	timer.SetupGUI(xorstr_("Timer"));
	ImGui::PopID();
}