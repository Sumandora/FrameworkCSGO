#ifndef SDK_GAMECLASS_CGLOBALVARS
#define SDK_GAMECLASS_CGLOBALVARS

class CGlobalVars {
public:
	float realtime;
	int framecount;
	float absoluteframetime;
	float absoluteframestarttimestddev;
	float curtime;
	float frametime;
	int maxClients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
	int simTicksThisFrame;
	int network_protocol;
	void* pSaveData;
	bool m_bClient;
	bool m_bRemoteClient;

private:
	int nTimestampNetworkingBase;
	int nTimestampRandomizeWindow;
};

// TODO Calculate accurate curtime using tickbase

#endif
