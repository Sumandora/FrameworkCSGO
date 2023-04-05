#ifndef HOOKS
#define HOOKS

class Hook {
	void* original;
	unsigned int len;

public:
	void* proxy;

	Hook() = delete;
	Hook(void* original, void* hook);
	~Hook();
};

namespace Hooks {
	void InstallHooks();
	void UninstallHooks();
}

#endif
