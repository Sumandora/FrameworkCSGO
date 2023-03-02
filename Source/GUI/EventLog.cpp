#include "EventLog.hpp"

#include <chrono>
#include <vector>

const int duration = 5000; // How long should one line last?

struct Entry {
	long time {};
	char text[512] {}; // When your message is longer than that, you should probably rephrase it lmao
};

std::vector<Entry> entries;

long time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Gui::EventLog::ImGuiRender(ImDrawList* drawList) {
	if (entries.empty())
		return;

	long currentTime = time();

	while (!entries.empty() && currentTime - entries.front().time > duration)
		entries.erase(entries.begin());

	ImColor white = ImColor(255, 255, 255, 255);
	ImColor black = ImColor(0, 0, 0, 255);

	float yOffset = 0;
	for (Entry entry : entries) {
		// Normalized from 0.0-1.0
		float animation = (float)(currentTime - entry.time) / (float) duration;
		// If you are curious, what this formula does, then just put it into geogebra or wolframalpha
		// FYI: The `10` is the duration the notification stays visible (uneven numbers break it)
		animation		= 1.0f - pow(((animation - 0.5f) * 2.0f), 10.0f);
		if(animation > 0.99f) // Because we are calculating everything using floats, we will see micro movements
			animation = 1.0f; // If we are close to 1.0 then just set it to 1.0, we don't care about movements in the 0.99-1.0 range

		ImVec2 size = ImGui::CalcTextSize(entry.text);
		ImVec2 position(-size.x * (1.0f - animation) + 10.0f, yOffset + 10.0f);

		drawList->AddText(ImVec2(position.x + 1.0f, position.y + 1.0f), black, entry.text);
		drawList->AddText(position, white, entry.text);

		yOffset += ImGui::GetTextLineHeightWithSpacing() * animation;
	}
}

void Gui::EventLog::CreateReport(const char* fmt, ...) {
	Entry entry;
	entry.time = time();

	va_list args;
	va_start(args, fmt);
	vsnprintf(entry.text, sizeof(Entry::text), fmt, args);
	va_end(args);

	entries.push_back(entry);
}