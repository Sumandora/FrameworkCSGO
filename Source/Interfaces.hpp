#ifndef INTERFACES
#define INTERFACES

#include "SDK/GameClass/Interfaces/CClientEntityList.hpp"
#include "SDK/GameClass/Interfaces/CEngineClient.hpp"
#include "SDK/GameClass/Interfaces/CEngineTrace.hpp"
#include "SDK/GameClass/Interfaces/ICvar.hpp"
#include "SDK/GameClass/Interfaces/IPrediction.hpp"
#include "SDK/GameClass/Interfaces/CGameMovement.hpp"

namespace Interfaces {
	inline void*				baseClient;
	inline CClientEntityList*	entityList;
	inline CEngineClient*		engine;
	inline CEngineTrace*		engineTrace;
	inline ICvar*				icvar;
	inline IPrediction*			prediction;
	inline CGameMovement*		gameMovement;

	void GetInterfaces();
	void SetupGUI();
}

#endif
