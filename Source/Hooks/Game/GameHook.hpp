#ifndef HOOKS_GAME_GAMEHOOK
#define HOOKS_GAME_GAMEHOOK

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