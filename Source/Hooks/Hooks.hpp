#ifndef HOOKS
#define HOOKS

class Hook {
	void* original;
	int len;

public:
	Hook(void* original, void* hook, int len);
	~Hook();

	void* proxy;
};

namespace Hooks {
	void InstallHooks();
	void UninstallHooks();
}

#endif
