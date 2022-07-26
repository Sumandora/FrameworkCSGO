#ifndef INTERFACES_HPP
#define INTERFACES_HPP

namespace Interfaces {
	inline void* baseClient;
	inline void* entityList;
	inline void* engine;
	
	void* GetInterface(const char* file, const char* name);
}

#endif
