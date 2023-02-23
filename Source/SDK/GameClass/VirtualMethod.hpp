#ifndef SDK_VIRTUALMETHOD
#define SDK_VIRTUALMETHOD

#include "ReturnAddr/ReturnAddr.hpp"

#include "../../Utils/VMT.hpp"
#include "../../Memory.hpp"

// Inspired by danielkrupinski/Osiris

namespace VirtualMethod {
	extern void* ret_instruction_addr;
	
	template<typename Ret, std::size_t Index, typename... Args>
	auto invoke(void* gameClass, Args... args) -> Ret {
		// Dont uncomment the first line, unless you know what you are doing / debugging
		//return reinterpret_cast<Ret(*)(void*, Args...)>(Utils::GetVTable(gameClass)[Index])(gameClass, args...);
		return Framework::ReturnAddr::invoke<Ret, void*, Args...>(Utils::GetVTable(gameClass)[Index], Memory::ret_instruction_addr, gameClass, args...);
	}
}


#define VIRTUAL_METHOD(index, name, returnType, argsType, argsCall) \
returnType name argsType {\
	return VirtualMethod::invoke<returnType, index>argsCall; \
}

#endif
