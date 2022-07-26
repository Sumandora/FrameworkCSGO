#ifndef UTILS_VMT
#define UTILS_VMT

namespace Utils {
	// This really only exists, because it makes code more readable
	static inline void** GetVTable(void* gameClass) {
		return *reinterpret_cast<void***>(gameClass);
	}
}

#endif
