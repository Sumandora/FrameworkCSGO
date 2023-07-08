#include "EventLog.hpp"

#include "Watermark.hpp"

#include "../../GUI/Elements/ShadowString.hpp"
#include "../../GUI/ImGuiColors.hpp"

#include <chrono>
#include <cmath>
#include <cstddef>
#include <unordered_map>
#include <vector>

struct Entry {
	long time;
	std::string text;
};

static std::vector<Entry> entries;

static long time()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void EventLog::ImGuiRender(ImDrawList* drawList)
{
	if (entries.empty())
		return;

	const long currentTime = time();

	std::erase_if(entries, [&](const Entry& entry) { return currentTime - entry.time > duration; });

	// Do the enabled check after erasing to prevent memory leaks
	if (!enabled)
		return;

	double yOffset = 0;

	if (watermark.enabled) {
		yOffset += ImGui::GetTextLineHeightWithSpacing();
	}

	for (const Entry& entry : entries) {
		// Normalized from 0.0-1.0
		double animation = (double)(currentTime - entry.time) / (double)duration;
		// If you are curious, what this formula does, then just put it into geogebra or wolframalpha
		// FYI: The `10` is the duration the notification stays visible (uneven numbers break it)
		animation = 1.0 - (float)pow(((animation - 0.5) * 2.0), 10.0);

		// Because we are calculating everything using floats, we will see micro movements
		// If we are close to 1.0 then just set it to 1.0, we don't care about movements in the 0.99-1.0 range
		animation = fmin(animation + 0.01, 1.0);

		const char* cstr = entry.text.c_str();

		const ImVec2 size = ImGui::CalcTextSize(cstr);
		const ImVec2 position((float)(-size.x * (1.0 - animation) + 10.0), (float)(yOffset + 10.0));

		ShadowString::AddText(drawList, position, ImGuiColors::white, cstr);

		yOffset += ImGui::GetTextLineHeightWithSpacing() * animation;
	}
}

void EventLog::SetupGUI()
{
	ImGui::Checkbox("Enabled", &enabled);
	if (!enabled) {
		ImGui::Text("Warning: Certain features may use the event log as communication");
	}
	ImGui::SliderInt("Duration", &duration, 0, 10000);
}

void EventLog::CreateReport(const char* fmt, ...)
{
	va_list args;

	size_t size = 0;
	va_start(args, fmt);
	size = vsnprintf(nullptr, size, fmt, args);
	va_end(args);

	size += 1;
	char str[size];

	va_start(args, fmt);
	vsnprintf(str, size, fmt, args);
	va_end(args);

	entries.push_back({ time(), str });
}

SCOPED_SERIALIZER(EventLog)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE("Duration", duration);
}
