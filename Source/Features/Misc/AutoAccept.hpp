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

	bool sendSystemNotification = false;
	bool autoAccept = false;

	void emitSound(const char* soundEntry) const;

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} autoAccept;

#endif