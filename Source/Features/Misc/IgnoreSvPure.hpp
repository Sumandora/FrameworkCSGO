#ifndef FEATURES_MISC_IGNORESVPURE
#define FEATURES_MISC_IGNORESVPURE

#include "../Features.hpp"

inline class IgnoreSvPure : public Feature {
public:
	IgnoreSvPure()
		: Feature("Misc", "Ignore sv_pure")
	{
	}

	bool cancelNetMessage = false;
	bool allowThirdPartyFiles = false;

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} ignoreSvPure;

#endif