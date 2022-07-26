#ifndef SDK_VIRTUALMETHOD
#define SDK_VIRTUALMETHOD

#include "ReturnAddr/ReturnAddr.hpp"
#include "../../Utils/VMT.hpp"

// Inspired by danielkrupinski/Osiris

namespace VirtualMethod {
	template<typename Ret, std::size_t Index, typename... Args>
	auto invoke(void* gameClass, Args... args) -> Ret {
		return Framework::ReturnAddr::invoke<Ret, void*, Args...>(Utils::GetVTable(gameClass)[Index], gameClass, args...);
	}
}


#define VIRTUAL_METHOD(index, name, returnType, argsType, argsCall) \
returnType name argsType {\
	return VirtualMethod::invoke<returnType, index>argsCall; \
}

#endif
