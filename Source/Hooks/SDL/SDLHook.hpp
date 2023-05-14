#ifndef HOOKS_SDL_SDLHOOK
#define HOOKS_SDL_SDLHOOK

class SDLHook {
	void** jumpAddress;

public:
	void* proxy;

	SDLHook() = delete;
	SDLHook(const char* name, void* hook);
	~SDLHook();
};

#endif
