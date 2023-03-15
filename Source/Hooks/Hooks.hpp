#ifndef HOOKS
#define HOOKS

class Hook {
	void* original;
	int len;

public:
	void* proxy;

	Hook() = delete;
	Hook(void* original, void* hook, int len);
	~Hook();
};

namespace Hooks {
	void InstallHooks();
	void UninstallHooks();
}

#endif
