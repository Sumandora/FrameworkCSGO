#ifndef HOOKS_GAMEFUNCTIONS_GAMEHOOK
#define HOOKS_GAMEFUNCTIONS_GAMEHOOK

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