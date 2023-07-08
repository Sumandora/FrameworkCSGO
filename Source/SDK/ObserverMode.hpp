#ifndef SDK_OBSERVERMODE
#define SDK_OBSERVERMODE

#include <cstring>
#include <map>

enum class ObserverMode : int {
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM,
	OBS_MODE_FREEZECAM,
	OBS_MODE_FIXED,
	OBS_MODE_IN_EYE,
	OBS_MODE_CHASE,
	OBS_MODE_ROAMING
};

static std::map<ObserverMode, const char*> observerModeLocalization{
	{ ObserverMode::OBS_MODE_NONE, "None" },
	{ ObserverMode::OBS_MODE_DEATHCAM, "Deathcam" },
	{ ObserverMode::OBS_MODE_FREEZECAM, "Freezecam" },
	{ ObserverMode::OBS_MODE_FIXED, "Fixed" },
	{ ObserverMode::OBS_MODE_IN_EYE, "In Eye" },
	{ ObserverMode::OBS_MODE_CHASE, "Chase" },
	{ ObserverMode::OBS_MODE_ROAMING, "Roaming" },
};

inline const char* LocalizeObserverMode(ObserverMode observerMode)
{
	if (!observerModeLocalization.contains(observerMode))
		return nullptr;
	return observerModeLocalization[observerMode];
}

#endif
