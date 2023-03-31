#ifndef SDK_OBSERVERMODE
#define SDK_OBSERVERMODE

enum class ObserverMode : int {
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM,
	OBS_MODE_FREEZECAM,
	OBS_MODE_FIXED,
	OBS_MODE_IN_EYE,
	OBS_MODE_CHASE,
	OBS_MODE_ROAMING
};

static std::map<ObserverMode, const char*> observerModeLocalization {
	{ ObserverMode::OBS_MODE_NONE, strdup(xorstr_("None")) },
	{ ObserverMode::OBS_MODE_DEATHCAM, strdup(xorstr_("Deathcam")) },
	{ ObserverMode::OBS_MODE_FREEZECAM, strdup(xorstr_("Freezecam")) },
	{ ObserverMode::OBS_MODE_FIXED, strdup(xorstr_("Fixed")) },
	{ ObserverMode::OBS_MODE_IN_EYE, strdup(xorstr_("In Eye")) },
	{ ObserverMode::OBS_MODE_CHASE, strdup(xorstr_("Chase")) },
	{ ObserverMode::OBS_MODE_ROAMING, strdup(xorstr_("Roaming")) },
};

inline const char* LocalizeObserverMode(ObserverMode observerMode)
{
	if(!observerModeLocalization.contains(observerMode))
		return xorstr_("Invalid observer mode");
	else
		return observerModeLocalization[observerMode];
}

#endif
