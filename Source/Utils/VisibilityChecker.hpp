#ifndef UTILS_VISIBILITYCHECKER
#define UTILS_VISIBILITYCHECKER

#include "../SDK/GameClass/Entities/CBasePlayer.hpp"
#include "../Serialization/Serializer.hpp"

class VisibilityChecker {
	bool considerSpottedEntitiesAsVisible = false;
	bool considerSmokedOffEntitiesAsOccluded = true;
	bool considerEveryoneVisibleWhenDead = false;

public:
	bool IsVisible(CBasePlayer* localPlayer, CBasePlayer* otherPlayer);
	void SetupGUI();
	SERIALIZER();
};

#endif