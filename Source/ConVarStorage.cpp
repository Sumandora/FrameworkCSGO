#include "ConVarStorage.hpp"

#include "Interfaces.hpp"
#include <unordered_map>

ConVar* ConVarStorage::getConVar(const char* name)
{
	static std::unordered_map<std::string, ConVar*> conVarMap{};

	if (!conVarMap.contains(name))
		conVarMap[name] = Interfaces::icvar->FindVar(name);

	return conVarMap[name];
}
