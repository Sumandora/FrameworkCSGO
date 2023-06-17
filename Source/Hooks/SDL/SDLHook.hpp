#ifndef HOOKS_SDL_SDLHOOK
#define HOOKS_SDL_SDLHOOK

class SDLHook {
	void** jumpTarget;

public:
	void* proxy;

	SDLHook() = delete;
	SDLHook(void* handle, const char* name, void* hook);
	~SDLHook();
};

#endif
