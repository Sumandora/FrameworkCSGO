#ifndef SDK_NETVARS_RECVPROP
#define SDK_NETVARS_RECVPROP

struct RecvTable; // Used to prevent dependency cycle

struct RecvProp {
	char*		m_pVarName;
	int			m_RecvType;
	int			m_Flags;
	int			m_StringBufferSize;
	bool		m_bInsideArray;
	const void* m_pExtraData;
	RecvProp*	m_pArrayProp;
	void*		m_ArrayLengthProxy;
	void*		m_ProxyFn;
	void*		m_DataTableProxyFn;
	RecvTable*	m_pDataTable;
	int			m_Offset;
	int			m_ElementStride;
	int			m_nElements;
	const char* m_pParentArrayPropName;
};

#endif
