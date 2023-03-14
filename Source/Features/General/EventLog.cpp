#include "../General.hpp"

#include "xorstr.hpp"

#include "../../GUI/Elements/ShadowString.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include <chrono>
#include <vector>

bool Features::General::EventLog::enabled = true;
int Features::General::EventLog::duration = 5000;

struct Entry {
	long time {};
	char text[512] {}; // When your message is longer than that, you should probably rephrase it lmao
};

std::vector<Entry> entries;

long time()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Features::General::EventLog::ImGuiRender(ImDrawList* drawList)
{
	if (entries.empty())
		return;

	long currentTime = time();

	while (!entries.empty() && currentTime - entries.front().time > duration)
		entries.erase(entries.begin());

	// Do the enabled check after erasing to prevent memory leaks
	if (!enabled)
		return;

	float yOffset = 0;

	if (Watermark::enabled) {
		yOffset += ImGui::GetTextLineHeightWithSpacing();
	}

	for (Entry entry : entries) {
		// Normalized from 0.0-1.0
		float animation = (float)(currentTime - entry.time) / (float)duration;
		// If you are curious, what this formula does, then just put it into geogebra or wolframalpha
		// FYI: The `10` is the duration the notification stays visible (uneven numbers break it)
		animation = 1.0f - pow(((animation - 0.5f) * 2.0f), 10.0f);
		if (animation > 0.99f) // Because we are calculating everything using floats, we will see micro movements
			animation = 1.0f; // If we are close to 1.0 then just set it to 1.0, we don't care about movements in the 0.99-1.0 range

		ImVec2 size = ImGui::CalcTextSize(entry.text);
		ImVec2 position(-size.x * (1.0f - animation) + 10.0f, yOffset + 10.0f);

		ShadowString::AddText(drawList, position, ImGuiColors::white, entry.text);

		yOffset += ImGui::GetTextLineHeightWithSpacing() * animation;
	}
}

void Features::General::EventLog::SetupGUI()
{
	ImGui::Checkbox(xorstr_("Enabled"), &enabled);
	if (!enabled) {
		ImGui::Text(xorstr_("Warning: Certain features may use the event log as communication"));
	}
	ImGui::SliderInt(xorstr_("Duration"), &duration, 0, 10000);
}

void Features::General::EventLog::CreateReport(const char* fmt, ...)
{
	Entry entry;
	entry.time = time();

	va_list args;
	va_start(args, fmt);
	vsnprintf(entry.text, sizeof(Entry::text), fmt, args);
	va_end(args);

	entries.push_back(entry);
}

BEGIN_SERIALIZED_STRUCT(Features::General::EventLog::Serializer, xorstr_("Event log"))
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Duration"), duration)
END_SERIALIZED_STRUCT
