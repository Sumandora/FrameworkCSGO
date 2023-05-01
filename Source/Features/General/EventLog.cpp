#include "General.hpp"

#include "xorstr.hpp"

#include "../../GUI/Elements/ShadowString.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include <chrono>
#include <cmath>
#include <vector>

static bool enabled = true;
static int duration = 5000;

struct Entry {
	long time{};
	char text[512]{}; // When your message is longer than that, you should probably rephrase it lmao
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

	const long currentTime = time();

	while (!entries.empty() && currentTime - entries.front().time > duration)
		entries.erase(entries.begin());

	// Do the enabled check after erasing to prevent memory leaks
	if (!enabled)
		return;

	double yOffset = 0;

	if (Watermark::enabled) {
		yOffset += ImGui::GetTextLineHeightWithSpacing();
	}

	for (Entry entry : entries) {
		// Normalized from 0.0-1.0
		double animation = (double)(currentTime - entry.time) / (double)duration;
		// If you are curious, what this formula does, then just put it into geogebra or wolframalpha
		// FYI: The `10` is the duration the notification stays visible (uneven numbers break it)
		animation = 1.0 - (float)pow(((animation - 0.5) * 2.0), 10.0);

		// Because we are calculating everything using floats, we will see micro movements
		// If we are close to 1.0 then just set it to 1.0, we don't care about movements in the 0.99-1.0 range
		animation = fmin(animation + 0.01, 1.0);

		const ImVec2 size = ImGui::CalcTextSize(entry.text);
		const ImVec2 position((float)(-size.x * (1.0 - animation) + 10.0), (float)(yOffset + 10.0));

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

BEGIN_SERIALIZED_STRUCT(Features::General::EventLog::Serializer)
SERIALIZED_TYPE(xorstr_("Enabled"), enabled)
SERIALIZED_TYPE(xorstr_("Duration"), duration)
END_SERIALIZED_STRUCT
