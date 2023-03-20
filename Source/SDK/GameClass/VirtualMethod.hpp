#ifndef SDK_VIRTUALMETHOD
#define SDK_VIRTUALMETHOD

#include "../../Memory.hpp"
#include "../../Utils/VMT.hpp"

#include "../../Utils/Platform/FunctionInvoker.hpp"

// Inspired by danielkrupinski/Osiris

namespace VirtualMethod {
	template <typename Ret, unsigned long Index, typename... Args>
	auto invoke(void* gameClass, Args... args) -> Ret
	{
		return invokeFunction<Ret, void*, Args...>(Utils::GetVTable(gameClass)[Index], gameClass, args...);
	}
}

#define VIRTUAL_METHOD(index, name, returnType, argsType, argsCall) \
	inline returnType name argsType                                 \
	{                                                               \
		return VirtualMethod::invoke<returnType, index> argsCall;   \
	}

#endif
