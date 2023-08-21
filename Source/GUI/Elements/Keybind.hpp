#ifndef GUI_ELEMENTS_KEYBIND
#define GUI_ELEMENTS_KEYBIND

#include "imgui.h"
#include <memory>
#include <optional>
#include <vector>

#include "../../Serialization/Serializer.hpp"

class Input;
inline std::vector<Input*> inputs;

class Input {
	bool active = false;

public:
	enum class Method : int {
		KEY = 0,
		MOUSE = 1
	};

	enum class Type : int {
		HOLD = 0,
		TOGGLE = 1
	};

	Method method;
	Type type;
	std::optional<unsigned int> key;

private:
	bool menuKey; // Should the key take effect inside the menu?

public:
	inline Input(Method method, Type type, std::optional<unsigned int> key, bool menuKey = false)
		: method(method)
		, type(type)
		, key(key)
		, menuKey(menuKey)
	{
		inputs.push_back(this);
	}
	inline Input(bool menuKey)
		: Input{ Method::KEY, Type::HOLD, std::nullopt, menuKey }
	{
	}
	inline Input()
		: Input{ false } {};
	inline ~Input() { std::erase(inputs, this); }

	inline bool isSet() const { return key.has_value(); }
	bool isActive() const;
	void updateState();
	void resetActive() { active = false; } // We don't keep old states, when changing keys

	SERIALIZER();

	virtual bool isAllowed(Method method, Type type, std::optional<unsigned int> key) { return true; }
	virtual void onChange() { }
};

namespace ImGui {
	IMGUI_API bool InputSelector(const char* label, Input& input, const ImVec2& size = ImVec2(0, 0));
}

#endif
