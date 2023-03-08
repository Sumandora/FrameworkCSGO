#ifndef SDK_NETVARS_RECVTABLE
#define SDK_NETVARS_RECVTABLE

#include "RecvProp.hpp"

struct RecvTable {
	RecvProp* m_pProps;
	int m_nProps;
	void* m_pDecoder;
	char* m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

#endif
