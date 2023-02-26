#ifndef INTERFACES
#define INTERFACES

#include "SDK/GameClass/Interfaces/CClientEntityList.hpp"
#include "SDK/GameClass/Interfaces/CEngineClient.hpp"
#include "SDK/GameClass/Interfaces/CEngineTrace.hpp"

namespace Interfaces {
	inline void*				baseClient;
	inline CClientEntityList*	entityList;
	inline CEngineClient*		engine;
	inline CEngineTrace*		engineTrace;

	void* GetInterface(const char* file, const char* name);
}

#endif
