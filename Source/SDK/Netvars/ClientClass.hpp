#ifndef SDK_NETVARS_CLIENTCLASS
#define SDK_NETVARS_CLIENTCLASS

#include "RecvTable.hpp"

class ClientClass
{
public:
	void* m_pCreateFn;
	void* m_pCreateEventFn;
	char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassID;
};

#endif
