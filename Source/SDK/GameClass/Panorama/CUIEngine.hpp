#ifndef SDK_GAMECLASS_PANORAMA_CUIENGINE
#define SDK_GAMECLASS_PANORAMA_CUIENGINE

#include "../Padding.hpp"
#include "../VirtualMethod.hpp"

#include "CUIPanel.hpp"

#include <functional>

/*
 * When executing scripts in games (map loaded) the following error occurs:
 *	JS Exception! *** Skipping rest of script ***
 *
 *	undefined
 *	line:0, col:-1
 *
 *	>>
 *
 *	RangeError: Maximum call stack size exceeded
 *
 * TODO Is there a way to fix this?
 */

class PanelArrayEntry { // Make sure this is exactly 0x20 bytes long, the game uses `panelArray + index * 0x20` in its code
	PADDING(4);
	CUIPanel* panel;
	PADDING(20);
};

class CUIEngine {
public:
	// LWSS (McDota/Fuzion) was a big help coding this
	// however a lot of his stuff was outdated, basically all offsets/vtable indexes/structs were outdated

	static int32_t panelArrayOffset;

	static constexpr size_t isValidPanelPointerIndex = 37;
	VIRTUAL_METHOD(isValidPanelPointerIndex, IsValidPanelPointer, bool, (CUIPanel * panel), (this, panel))
	VIRTUAL_METHOD(114, RunScript, void, (CUIPanel * panel, const char* script, const char* context = "panorama/layout/base.xml", int i1 = 8, int i2 = 10, bool compiled = false), (this, panel, script, context, i1, i2, compiled))

	inline PanelArrayEntry* GetPanelArray()
	{
		return *reinterpret_cast<PanelArrayEntry**>(reinterpret_cast<char*>(this) + panelArrayOffset);
	}

	// https://github.com/LWSS/McDota/blob/5d577e41a3ce31202f40c0afac5426a0ac7f59c9/src/GUI/Gui.cpp#L32C42-L33C39
	inline void ForEachPanel(std::function<void(CUIPanel*)> callback)
	{
#ifdef DEBUG
		for (int i = 0; i < 4096; i++) {
			CUIPanel* panel = GetPanelArray()[i].panel;
			while (panel && IsValidPanelPointer(panel)) {
				callback(panel);
				panel = panel->GetParent();
			}
		}
#endif
	}

	inline CUIPanel* FindPanel(const char* id)
	{
		for (int i = 0; i < 4096; i++) {
			CUIPanel* panel = GetPanelArray()[i].panel;
			while (panel && IsValidPanelPointer(panel)) {
				if (std::strcmp(panel->GetID(), id) == 0)
					return panel;
				panel = panel->GetParent();
			}
		}
		return nullptr;
	}

	inline void RunScript(const char* panel, const char* source)
	{
		CUIPanel* uiPanel = FindPanel(panel);
		RunScript(uiPanel, source);
	}
};

#endif