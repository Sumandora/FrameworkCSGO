#ifndef HOOKS_GAMEFUNCTIONS
#define HOOKS_GAMEFUNCTIONS

#include "DetourHooking.hpp"

class GameHook {
	Hook* backingHook;

public:
	void* proxy;

	GameHook() = delete;
	GameHook(void* original, void* hook);
	~GameHook();
};

#endif