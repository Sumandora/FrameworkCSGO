#ifndef FEATURES_MISC_PARTYSCRIPTS
#define FEATURES_MISC_PARTYSCRIPTS

#include "../Features.hpp"

#include "imgui.h"

inline class PartyScripts : public Feature {
public:
	PartyScripts()
		: Feature("Misc", "Party scripts")
	{
	}

	int popupAmount = 1;
	int errorAmount = 1;
	int selectedError = 0;

	bool autoClosePopups = false;
	float popupInterval = 1.0f;

	bool autoErrorCreation = false;
	float errorInterval = 1.0f;
	bool randomErrors = false;

	bool autoCancelMatchmaking = false;
	float cancelInterval = 1.0f;
	bool onlyWhenSearching = false;

	void performPartyScripts();

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} partyScripts;

#endif