#ifndef FEATURES_GENERAL_EVENTLOG
#define FEATURES_GENERAL_EVENTLOG

#include "../Features.hpp"

#include "imgui.h"
#include "xorstr.hpp"

inline class EventLog : public Feature {
public:
	EventLog()
		: Feature(xorstr_("General"), xorstr_("Event log"))
	{
	}

	bool enabled = true;
	int duration = 5000;

	void ImGuiRender(ImDrawList* drawList);

	void CreateReport(const char* fmt, ...);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} eventLog;

#endif