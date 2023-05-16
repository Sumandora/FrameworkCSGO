#ifndef SDK_NETVARS_CLIENTCLASS
#define SDK_NETVARS_CLIENTCLASS

#include "RecvTable.hpp"
#include "SDK/ClientClassIDs.hpp"

class ClientClass {
public:
	void* m_pCreateFn;
	void* m_pCreateEventFn;
	char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	ClientClassID m_ClassID;
};

#endif
