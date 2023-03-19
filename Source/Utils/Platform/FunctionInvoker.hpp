#ifndef UTILS_FUNCTIONINVOKER
#define UTILS_FUNCTIONINVOKER

#include "../../Memory.hpp"

#ifdef __clang__

// These are included in ReturnAddr.hpp

#include "PatternScan/PatternScan.hpp"
#include "Memory/Memory.hpp"
#include "Assembly/Assembly.hpp"

#include <mutex>
#include <sys/mman.h>
#include <cstring>

template<typename Ret, typename... Args>
static auto invokeFunction(void *method, Args... args) -> Ret {
	return reinterpret_cast<Ret(*)(Args...)>(method)(args...);
}

#else

#include "ReturnAddr/ReturnAddr.hpp"

template<typename Ret, typename... Args>
static auto invokeFunction(void *method, Args... args) -> Ret {
	return Framework::ReturnAddr::invoke<Ret, Args...>(method, Memory::ret_instruction_addr, args...);
}

#endif

#endif