#ifndef INTERFACES
#define INTERFACES

#include "SDK/GameClass/CClientEntityList.hpp"
#include "SDK/GameClass/CEngineClient.hpp"

namespace Interfaces {
	inline void* baseClient;
	inline CClientEntityList* entityList;
	inline CEngineClient* engine;
	
	void* GetInterface(const char* file, const char* name);
}

#endif
