#ifndef NETVARS
#define NETVARS

#define NETVAR_FUNCTION(returnType, functionName, table, name) \
returnType* functionName () { \
	return reinterpret_cast<returnType*>(reinterpret_cast<char*>(this) + Netvars::GetOffset(table, name)); \
}

namespace Netvars {
	void DumpNetvars();
	int GetOffset(const char* table, const char* name);
}

#endif
