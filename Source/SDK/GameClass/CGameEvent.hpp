#ifndef SDK_GAMECLASS_CGAMEEVENT
#define SDK_GAMECLASS_CGAMEEVENT

#include "VirtualMethod.hpp"
#include <cstdint>

class CGameEvent {
public:
	VIRTUAL_METHOD(2, GetName, const char*, (), (this))

	VIRTUAL_METHOD(6, GetBool, bool, (const char* keyName, bool defaultValue = false), (this, keyName, defaultValue))
	VIRTUAL_METHOD(7, GetInt, int, (const char* keyName, int defaultValue = 0), (this, keyName, defaultValue))
	VIRTUAL_METHOD(8, GetUint64, std::uint64_t, (const char* keyName, std::uint64_t defaultValue = 0), (this, keyName, defaultValue))
	VIRTUAL_METHOD(9, GetFloat, float, (const char* keyName, float defaultValue = 0.0f), (this, keyName, defaultValue))
	VIRTUAL_METHOD(10, GetString, const char*, (const char* keyName, const char* defaultValue = nullptr), (this, keyName, defaultValue))
	VIRTUAL_METHOD(11, GetWString, const wchar_t*, (const char* keyName, const wchar_t* defaultValue = nullptr), (this, keyName, defaultValue))
	VIRTUAL_METHOD(12, GetPtr, void*, (const char* keyName, void* defaultValue = nullptr), (this, keyName, defaultValue))

	VIRTUAL_METHOD(13, SetBool, bool, (const char* keyName, bool value), (this, keyName, value))
	VIRTUAL_METHOD(14, SetInt, int, (const char* keyName, int value), (this, keyName, value))
	VIRTUAL_METHOD(15, SetUint64, std::uint64_t, (const char* keyName, std::uint64_t value), (this, keyName, value))
	VIRTUAL_METHOD(16, SetFloat, float, (const char* keyName, float value), (this, keyName, value))
	VIRTUAL_METHOD(17, SetString, const char*, (const char* keyName, const char* value), (this, keyName, value))
	VIRTUAL_METHOD(18, SetWString, const wchar_t*, (const char* keyName, const wchar_t* value), (this, keyName, value))
	VIRTUAL_METHOD(19, SetPtr, void*, (const char* keyName, void* value), (this, keyName, value))
};

#endif
