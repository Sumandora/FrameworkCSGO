#ifndef UTILS_PLATFORM_FUNCTIONINVOKER
#define UTILS_PLATFORM_FUNCTIONINVOKER

#include "../../Memory.hpp"
#include "ReturnAddr/ReturnAddr.hpp"

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
	return Framework::ReturnAddr::invoke<Ret, Args...>(method, Memory::ret_instruction_addr, args...);
}

#endif

#endif