#include "Keybind.hpp"

#include "xorstr.hpp"

#include "../GUI.hpp"

#include <cstdlib>
#include <optional>

// TODO Rewrite input values
bool IsInputDown(int key, bool _default, std::optional<bool> inMenu)
{
	if (Gui::visible && inMenu.has_value())
		return inMenu.value();

	if (key > 0)
		return ImGui::IsKeyDown(static_cast<ImGuiKey>(key));
	if (key < 0)
		return ImGui::IsMouseDown(abs(key) - 1);
	if (key == 0)
		return _default;
	return false;
}

IMGUI_API bool ImGui::InputSelector(const char* label, int& key, const ImVec2& size)
{
	static std::optional<ImGuiID> waiting;
	ImGuiID currentID = ImGui::GetID(label);

	char newLabel[128];

	if (waiting == currentID) {
		strcpy(newLabel, xorstr_("Waiting for input"));

		bool found = false;
		for (int imguikey = ImGuiKey_Tab; imguikey != ImGuiKey_KeypadEqual; imguikey++) {
			if (ImGui::IsKeyDown(static_cast<ImGuiKey>(imguikey))) {
				waiting.reset();
				if (imguikey == ImGuiKey_Escape)
					key = 0; // Unset
				else
					key = imguikey;
				found = true;
				break;
			}
		}
		if (!found) {
			// Maybe a mouse button?
			auto* buttons = ImGui::GetIO().MouseDown;
			for (int i = 0; i < 5; i++) {
				if (buttons[i]) {
					waiting.reset();
					key = -1 - i; // Negative ids indicate mouse buttons (LMB has id 0, in order to not conflict with unset which is also zero, we do that)
					break;
				}
			}
		}
	} else {
		char keyName[128];

		if (key == 0) {
			strcpy(keyName, xorstr_("Unset"));
		} else if (key > 0) {
			strcpy(keyName, ImGui::GetKeyName(static_cast<ImGuiKey>(key)));
		} else if (key < 0) {
			strcpy(keyName, xorstr_("Mouse"));
			keyName[5] = (char)(48 + abs(key)); // 48 = '0' in ascii
			keyName[6] = '\0';
		}

		sprintf(newLabel, label, keyName);
	}

	if (ImGui::Button(newLabel, size)) {
		waiting = currentID;
		return true;
	}
	return false;
}
