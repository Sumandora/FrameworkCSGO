#ifndef FEATURES_MISC_AUTOACCEPT
#define FEATURES_MISC_AUTOACCEPT

#include "../Features.hpp"

#include "imgui.h"

inline class AutoAccept : public Feature {
public:
	AutoAccept()
		: Feature("Misc", "Auto accept")
	{
	}

	bool enabled = false;
	bool sendSystemNotification = false;
	bool dontAccept = false;

	void EmitSound(const char* soundEntry);

	virtual void SetupGUI() override;
	virtual SERIALIZER() override;
} autoAccept;

#endif