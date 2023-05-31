#include "ConVarStorage.hpp"

#include "Interfaces.hpp"

ConVar* ConVarStorage::GetConVar(const char* name)
{
	static std::map<const char*, ConVar*> conVarMap{};

	if (!conVarMap.contains(name))
		conVarMap[name] = Interfaces::icvar->FindVar(name);

	return conVarMap[name];
}
