#ifndef SDK_GAMECLASS_VIRTUALMETHOD
#define SDK_GAMECLASS_VIRTUALMETHOD

#include "../../Utils/VMT.hpp"

#include "../../Utils/Platform/FunctionInvoker.hpp"

// Inspired by danielkrupinski/Osiris

namespace VirtualMethod {
	template <typename Ret, unsigned long Index, typename... Args>
	auto Invoke(void* gameClass, Args... args) -> Ret
	{
		return InvokeFunction<Ret, void*, Args...>(Utils::GetVTable(gameClass)[Index], gameClass, args...);
	}
}

#define VIRTUAL_METHOD(index, name, returnType, argsType, argsCall) \
	inline returnType name argsType                                 \
	{                                                               \
		return VirtualMethod::Invoke<returnType, index> argsCall;   \
	}

#endif
