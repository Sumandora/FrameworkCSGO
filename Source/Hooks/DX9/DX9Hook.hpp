#ifndef HOOKS_CREATEMOVE_HOOK
#define HOOKS_CREATEMOVE_HOOK

namespace Hooks {
	namespace DX9 {
		void* device;
		
		inline void* proxy;
		long IDirect3DDevice9_Present(void*, void*, void*, void*, void*);
		void Hook();
	}
}

#endif
