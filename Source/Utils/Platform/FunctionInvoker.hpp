#ifndef UTILS_PLATFORM_FUNCTIONINVOKER
#define UTILS_PLATFORM_FUNCTIONINVOKER

#include "RetAddrSpoofer.hpp"

#ifdef __clang__

template <typename Ret, typename... Args>
inline auto invokeFunction(void* method, Args... args) -> Ret
{
	return reinterpret_cast<Ret (*)(Args...)>(method)(args...);
}

#else

template <typename Ret, typename... Args>
inline auto invokeFunction(void* method, Args... args) -> Ret
{
	return RetAddrSpoofer::invoke<Ret, Args...>(method, args...);
}

#endif

#endif
