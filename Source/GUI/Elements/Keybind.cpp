#include "Keybind.hpp"

#include "../GUI.hpp"
#include "imgui.h"

#include <cstdlib>
#include <optional>

bool Input::IsActive()
{
	return active;
}

void Input::UpdateState()
{
	if ((Gui::visible && !menuKey) || !key.has_value()) {
		active = false;
		return;
	}

	// This code is terrible...
	bool prevActive = active;
	switch (type) {
	case Type::HOLD: {
		switch (method) {
		case Method::KEY: {
			active = ImGui::IsKeyDown(static_cast<ImGuiKey>(key.value()));
			break;
		}
		case Method::MOUSE: {
			active = ImGui::IsMouseDown(static_cast<ImGuiMouseButton>(key.value()));
			break;
		}
		}
		break;
	}
	case Type::TOGGLE: {
		switch (method) {
		case Method::KEY: {
			if (ImGui::IsKeyPressed(static_cast<ImGuiKey>(key.value()), false))
				active = !active;
			break;
		}
		case Method::MOUSE: {
			if (ImGui::IsMouseClicked(static_cast<ImGuiMouseButton>(key.value()), false))
				active = !active;
			break;
		}
		}
		break;
	}
	}
	if (active != prevActive)
		OnChange();
}

SCOPED_SERIALIZER(Input)
{
	SERIALIZE_ENUM("Method", method);
	SERIALIZE_ENUM("Type", type);
	bool hasKey = key.has_value();
	SERIALIZE("Has key", hasKey);
	if (hasKey)
		SERIALIZE("Key", key.value());
}

IMGUI_API bool ImGui::InputSelector(const char* label, Input& input, const ImVec2& size)
{
	static std::optional<ImGuiID> waiting;
	ImGuiID currentID = ImGui::GetID(label);

	char newLabel[128];

	if (waiting == currentID) {
		strcpy(newLabel, "Waiting for input");

		bool found = false;
		for (ImGuiKey imguikey = ImGuiKey_Tab; imguikey != ImGuiKey_KeypadEqual; imguikey = static_cast<ImGuiKey>(static_cast<int>(imguikey) + 1)) {
			if (ImGui::IsKeyDown(static_cast<ImGuiKey>(imguikey))) {
				waiting.reset();
				std::optional<ImGuiKey> newKey;
				if (imguikey == ImGuiKey_Escape) // TODO Have a lengthy conversation about if escape should be possible and you just get a "Unset"-button inside the right-click context menu
					newKey = std::nullopt; // Unset
				else
					newKey = imguikey;
				if (input.IsAllowed(Input::Method::KEY, input.type, newKey)) {
					input.method = Input::Method::KEY;
					input.key = newKey;
					input.ResetActive();
				}
				found = true;
				break;
			}
		}
		if (!found) {
			// Maybe a mouse button?
			for (ImGuiMouseButton imguibtn = ImGuiMouseButton_Left; imguibtn < ImGuiMouseButton_COUNT; imguibtn++) {
				if (ImGui::IsMouseDown(imguibtn)) {
					waiting.reset();
					if (input.IsAllowed(Input::Method::MOUSE, input.type, imguibtn)) {
						input.method = Input::Method::MOUSE;
						input.key = imguibtn;
						input.ResetActive();
					}
					break;
				}
			}
		}
	} else {
		char keyName[128];

		if (!input.key.has_value()) {
			strcpy(keyName, "Unset");
		} else if (input.method == Input::Method::KEY) {
			strcpy(keyName, ImGui::GetKeyName(static_cast<ImGuiKey>(input.key.value())));
		} else if (input.method == Input::Method::MOUSE) {
			strcpy(keyName, "Mouse");
			keyName[5] = (char)(48 + input.key.value()); // 48 = '0' in ascii
			keyName[6] = '\0';
		}

		sprintf(newLabel, label, keyName);
	}

	bool clicked = false;
	if (ImGui::Button(newLabel, size)) {
		waiting = currentID;
		clicked = true;
	}

	bool canHaveOtherType = input.IsAllowed(input.method, static_cast<Input::Type>(1 - static_cast<int>(input.type)), input.key);

	if (canHaveOtherType && ImGui::BeginPopupContextItem()) {
		static const char* types[]{ "Hold", "Toggle" };
		ImGui::Combo("Type", reinterpret_cast<int*>(&input.type), types, IM_ARRAYSIZE(types));
		ImGui::EndPopup();
	}

	return clicked;
}
