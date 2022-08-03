#ifndef INTERFACES
#define INTERFACES

#include "SDK/GameClass/CClientEntityList.hpp"
#include "SDK/GameClass/CEngineClient.hpp"
#include "SDK/GameClass/CGlobalVars.hpp"

namespace Interfaces {
	inline void* baseClient;
	inline void* clientMode;
	inline CClientEntityList* entityList;
	inline CEngineClient* engine;
	inline CGlobalVars* globalVars;
	
	void* GetInterface(const char* file, const char* name);
}

#endif
