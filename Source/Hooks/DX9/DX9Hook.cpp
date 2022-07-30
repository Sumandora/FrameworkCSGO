#include "Hooking/Hooking.hpp"

#include "DX9Hook.hpp"

#include <dlfcn.h>
#include <cstdio>

long Hooks::DX9::IDirect3DDevice9_Present(void* thisptr, void* pSourceRect, void* pDestRect, void* hDestWindowOverride, void* pDirtyRegion) {
	device = thisptr; // grab the device
	printf("Found device: %p\n", device);
	return reinterpret_cast<long(*)(void*, void*, void*, void*, void*)>(proxy)(thisptr, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

void Hooks::DX9::Hook() {
	void* present = dlsym(RTLD_NEXT, "_ZN16IDirect3DDevice97PresentEPK5_RECTS2_PvPK7RGNDATA"); // this is the mangled name

	printf("%p found present\n", present);
	Framework::Hooking::detour(present, reinterpret_cast<void*>(IDirect3DDevice9_Present), 5);
}
