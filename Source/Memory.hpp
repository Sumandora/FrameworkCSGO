#ifndef MEMORY
#define MEMORY

#include "SDK/GameClass/CGlobalVars.hpp"
#include <span>
class IMoveHelper;
class CBasePlayer;
struct Vector;

namespace Memory {
	inline void* clientMode;
	inline CGlobalVars* globalVars;
	inline IMoveHelper* moveHelper;

	std::span<std::byte> GetTextSection(const char* name);
	void* RelativeToAbsolute(void* addr);

	void Create();

	bool LineGoesThroughSmoke(const Vector& from, const Vector& to, short _);
}

#endif
