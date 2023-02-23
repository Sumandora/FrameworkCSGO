#include "Keybind.hpp"

#include "xorstr.hpp"

#include "../GUI.hpp"

#include "SDL.h"

#include <cstdlib>

bool IsInputDown(int key) {
	if(key > 0)
		return ImGui::IsKeyDown(static_cast<ImGuiKey>(key));
	else if(key < 0)
		return ImGui::IsMouseDown(abs(key) - 1);
	return false;
}

IMGUI_API bool ImGui::InputSelector(const char* label, int& key, const ImVec2& size) {

	static const char* waiting = nullptr;

	const char* newLabel;

	if(waiting == label) {
		newLabel = xorstr_("Waiting for input");

		bool found = false;
		for(int imguikey = ImGuiKey_Tab; imguikey != ImGuiKey_KeypadEqual; imguikey++) {
			if(ImGui::IsKeyDown(static_cast<ImGuiKey>(imguikey))) {
				waiting = nullptr;
				if(imguikey == ImGuiKey_Escape)
					key = 0; // Unset
				else
					key = imguikey;
				found = true;
				break;
			}
		}
		if(!found) {
			// Maybe a mouse button?
			auto buttons = ImGui::GetIO().MouseDown;
			for(int i = 0; i < 5; i++) {
				if(buttons[i])  {
					waiting = nullptr;
					key = -1 - i; // Negative ids indicate mouse buttons (LMB has id 0, in order to not conflict with unset which is also zero, we do that)
					break;
				}
			}
		}
	} else {
		char keyName[128];

		if(key == 0) {
			strcpy(keyName, "Unset");
		} else if(key > 0) {
			strcpy(keyName, ImGui::GetKeyName(static_cast<ImGuiKey>(key)));
		} else if(key < 0) {
			strcpy(keyName, "Mouse");
			keyName[5] = 48 + abs(key); // 48 = '0' in ascii
			keyName[6] = '\0';
		}

		char buf[512];
		sprintf(buf, label, keyName);
		newLabel = buf;
	}

	if(ImGui::Button(newLabel, size)) {
		waiting = label;
	}
}
