#ifndef HOOKS_SDL
#define HOOKS_SDL

class SDLHook {
	void** jumpAddress;

public:
	void* proxy;

	SDLHook() = delete;
	SDLHook(const char* name, void* hook);
	~SDLHook();
};

#endif
