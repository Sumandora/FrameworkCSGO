#include "Serialization.hpp"

#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>

#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include "imgui.h"
#include "xorstr.hpp"

#include "../Features/General.hpp"

char* Serialization::GetConfigDirectory()
{
	return strcat(getpwuid(getuid())->pw_dir, xorstr_("/.config/Framework"));
}

char* GetConfigFile(const char* filename)
{
	return strcat(strcat(Serialization::GetConfigDirectory(), "/"), filename);
}

bool GatherConfigs(std::vector<std::string>& configs)
{
	DIR* dir = opendir(Serialization::GetConfigDirectory());
	if (dir == nullptr)
		return false;

	dirent* ent {};

	while ((ent = readdir(dir))) {
		if (ent->d_type != DT_REG)
			continue;

		configs.emplace_back(std::string(ent->d_name));
	}

	closedir(dir);

	return true;
}
void Serialization::SetupGUI()
{
	static int configSelected;
	static std::vector<std::string> configs;
	static long lastRefresh = 0L;

	static char name[256];

	bool addedConfig = false;

	if (ImGui::Button(xorstr_("Save")) && name[0] != '\0') {
		auto* cfgName = name;
		if (!std::string(cfgName).ends_with(".ini"))
			cfgName = strcat(cfgName, ".ini");

		if (Save(GetConfigFile(cfgName)))
			Features::General::EventLog::CreateReport(xorstr_("Saved config '%s'"), cfgName);
		else
			Features::General::EventLog::CreateReport(xorstr_("Failed to saved config '%s'"), cfgName);

		addedConfig = true;
		name[0] = '\0';
	}
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::SameLine();
	ImGui::InputText(xorstr_("##Name"), name, IM_ARRAYSIZE(name));
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
	const long currTime = time(nullptr);

	if (currTime - lastRefresh > 1 || addedConfig) {
		configs.clear();
		GatherConfigs(configs);
		lastRefresh = currTime;
	}

	const char* configsArray[configs.size()];
	for (size_t i = 0; i < configs.size(); i++) {
		configsArray[i] = configs.at(i).c_str();
	}

	if (ImGui::ListBox(xorstr_("##Configs"), &configSelected, configsArray, configs.size(), configs.size())) {
		if (configSelected >= 0 && configSelected < static_cast<int>(configs.size())) {
			const char* configName = configs.at(configSelected).c_str();
			if (Load(GetConfigFile(configName)))
				Features::General::EventLog::CreateReport(xorstr_("Loaded config '%s'"), configName);
			else
				Features::General::EventLog::CreateReport(xorstr_("Failed to load config '%s'"), configName);
		}
	}
	ImGui::PopItemWidth();
}