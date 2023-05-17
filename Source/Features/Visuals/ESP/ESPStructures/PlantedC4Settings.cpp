#include "../ESPStructure.hpp"
#include <iomanip>

#include "xorstr.hpp"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../GUI/ImGuiColors.hpp"

PlantedC4Settings::PlantedC4Settings()
	: overrideDefuseColor(true)
	, defusePossibleWithKit(ImGuiColors::yellow)
	, defuseImpossible(ImGuiColors::red)
	, accuracy(1)
{
}

std::string FloatToString(float num, int accuracy)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(accuracy) << num;
	return stream.str();
}

void PlantedC4Settings::Draw(ImDrawList* drawList, ImVec4 rectangle, const PlantedC4& bomb) const
{
	boxName.Draw(drawList, rectangle, xorstr_("Planted C4"));
	if (!bomb.defused && bomb.bombTicking) { // TODO Ask GameRules if the bomb is actually planted
		if (bomb.bombTime < Memory::globalVars->curtime)
			return; // You can't defuse the bomb anymore
		const float middle = rectangle.x + (rectangle.z - rectangle.x) * 0.5f;

		const float defusableCountDown = bomb.bombTime - Memory::globalVars->curtime;

		std::optional<ImColor> color = std::nullopt;
		if (overrideDefuseColor && defusableCountDown <= 10.0) {
			if (defusableCountDown > 5.0)
				color = defusePossibleWithKit;
			else
				color = defuseImpossible;
		}

		timer.Draw(drawList, middle, rectangle.w, true, FloatToString(defusableCountDown, accuracy).c_str(), color);

		if(bomb.defuser != INVALID_EHANDLE_INDEX) {
			const float defuseCountDown = bomb.defuseCountDown - Memory::globalVars->curtime;
			if (defuseCountDown < 0)
				return; // We are done defusing, no point in showing this anymore

			float y = rectangle.w;

			if(timer.enabled)
				y += timer.GetLineHeight();

			defuseTimer.Draw(drawList, middle, y, true, FloatToString(defuseCountDown, accuracy).c_str(), defuseCountDown > defusableCountDown ? std::optional<ImColor>(defuseImpossible) : std::nullopt);
		}
	}
}

void PlantedC4Settings::SetupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.SetupGUI(id);
	timer.SetupGUI(xorstr_("Timer"));
	defuseTimer.SetupGUI(xorstr_("Defuse timer"));

	if(timer.enabled) {
		ImGui::Checkbox(xorstr_("Override defuse color"), &overrideDefuseColor);
		if (overrideDefuseColor) {
			ImGui::ClickableColorButton(xorstr_("Defuse possible with kit"), defusePossibleWithKit);
			ImGui::ClickableColorButton(xorstr_("Defuse possible impossible"), defuseImpossible);
		}
	}

	if(timer.enabled || defuseTimer.enabled)
		ImGui::SliderInt(xorstr_("Accuracy"), &accuracy, 1, 5);
	ImGui::PopID();
}

BEGIN_SERIALIZED_STRUCT(PlantedC4Settings::Serializer)
SERIALIZED_STRUCTURE(name, boxName)
SERIALIZED_STRUCTURE(xorstr_("Timer"), timer)
SERIALIZED_STRUCTURE(xorstr_("Defuse timer"), defuseTimer)

SERIALIZED_TYPE(xorstr_("Override defuse color"), overrideDefuseColor)
SERIALIZED_TYPE(xorstr_("Defuse possible with kit"), defusePossibleWithKit)
SERIALIZED_TYPE(xorstr_("Defuse possible impossible"), defuseImpossible)

SERIALIZED_TYPE(xorstr_("Accuracy"), accuracy)
END_SERIALIZED_STRUCT