#include "../ESPStructure.hpp"

#include <iomanip>

#include "../../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../../GUI/ImGuiColors.hpp"

PlantedC4Settings::PlantedC4Settings()
	: overrideDefuseColor(true)
	, defusePossibleWithKit(ImGuiColors::yellow)
	, defuseImpossible(ImGuiColors::red)
	, accuracy(1)
{
}

bool PlantedC4Settings::isEnabled() const
{
	return boxName.isEnabled() || timer.enabled || defuseTimer.enabled;
}

static std::string FloatToString(float num, int accuracy)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(accuracy) << num;
	return stream.str();
}

void PlantedC4Settings::draw(ImDrawList* drawList, PlantedC4& bomb) const
{
	if (!isEnabled())
		return;

	const std::optional<ImVec4> rectangle = bomb.screenRectangle.get();
	if (!rectangle.has_value())
		return;

	boxName.draw(drawList, rectangle.value(), "Planted C4");

	if (!bomb.defused && bomb.bombTicking) { // TODO Ask GameRules if the bomb is actually planted
		if (bomb.bombTime < Memory::globalVars->curtime)
			return; // You can't defuse the bomb anymore
		const float middle = rectangle->x + (rectangle->z - rectangle->x) * 0.5f;

		const float defusableCountDown = bomb.bombTime - Memory::globalVars->curtime;

		std::optional<ImColor> color = std::nullopt;
		if (overrideDefuseColor && defusableCountDown <= 10.0) {
			if (defusableCountDown > 5.0)
				color = defusePossibleWithKit;
			else
				color = defuseImpossible;
		}

		timer.draw(drawList, middle, rectangle->w, true, FloatToString(defusableCountDown, accuracy).c_str(), color);

		if (EntityCache::playerByHandle(bomb.defuser)) {
			const float defuseCountDown = bomb.defuseCountDown - Memory::globalVars->curtime;
			if (defuseCountDown < 0)
				return; // We are done defusing, no point in showing this anymore

			float y = rectangle->w;

			if (timer.enabled)
				y += timer.getLineHeight();

			defuseTimer.draw(drawList, middle, y, true, FloatToString(defuseCountDown, accuracy).c_str(), defuseCountDown > defusableCountDown ? std::optional<ImColor>(defuseImpossible) : std::nullopt);
		}
	}
}

void PlantedC4Settings::setupGUI(const char* id)
{
	ImGui::PushID(id);
	boxName.setupGUI(id);
	timer.setupGUI("Timer");
	defuseTimer.setupGUI("Defuse timer");

	if (timer.enabled) {
		ImGui::Checkbox("Override defuse color", &overrideDefuseColor);
		if (overrideDefuseColor) {
			ImGui::ClickableColorButton("Defuse possible with kit", defusePossibleWithKit);
			ImGui::ClickableColorButton("Defuse possible impossible", defuseImpossible);
		}
	}

	if (timer.enabled || defuseTimer.enabled)
		ImGui::SliderInt("Accuracy", &accuracy, 1, 5);
	ImGui::PopID();
}

SCOPED_SERIALIZER(PlantedC4Settings)
{
	SERIALIZE_STRUCT("Box name", boxName);
	SERIALIZE_STRUCT("Timer", timer);
	SERIALIZE_STRUCT("Defuse timer", defuseTimer);

	SERIALIZE("Override defuse color", overrideDefuseColor);
	SERIALIZE_VECTOR4D("Defuse possible with kit", defusePossibleWithKit.Value);
	SERIALIZE_VECTOR4D("Defuse possible impossible", defuseImpossible.Value);

	SERIALIZE("Accuracy", accuracy);
}