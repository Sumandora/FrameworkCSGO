#ifndef UTILS_PLATFORM_FUNCTIONINVOKER
#define UTILS_PLATFORM_FUNCTIONINVOKER

#include "../../Memory.hpp"
#include "RetAddrSpoofer.hpp"

#ifdef __clang__

template <typename Ret, typename... Args>
inline auto InvokeFunction(void* method, Args... args) -> Ret
{
	return reinterpret_cast<Ret (*)(Args...)>(method)(args...);
}

#else

template <typename Ret, typename... Args>
inline auto InvokeFunction(void* method, Args... args) -> Ret
{
	return RetAddrSpoofer::Invoke<Ret, Args...>(method, args...);
}

#endif

#endif