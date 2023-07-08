#include "Configuration.hpp"

#include <cstring>
#include <ctime>
#include <string>
#include <vector>

#include <dirent.h>

#include "imgui.h"

#include "../../../Features/General/EventLog.hpp"

#include "../../../Serialization/Serialization.hpp"

// TODO Remake GUI

static std::string GetConfigFile(std::string filename)
{
	return Serialization::GetConfigDirectory() + '/' + filename;
}

static bool GatherConfigs(std::vector<std::string>& configs)
{
	DIR* dir = opendir(Serialization::GetConfigDirectory().c_str());
	if (dir == nullptr)
		return false;

	dirent* ent;

	while ((ent = readdir(dir))) {
		if (ent->d_type != DT_REG)
			continue;

		configs.emplace_back(ent->d_name);
	}

	closedir(dir);

	return true;
}
void Gui::Windows::Configuration()
{
	static int configSelected;
	static std::vector<std::string> configs;
	static long lastRefresh = 0L;

	static char name[256];

	bool addedConfig = false;

	if (ImGui::Button("Save") && name[0] != '\0') {
		auto* cfgName = name;
		if (!std::string(cfgName).ends_with(".json"))
			cfgName = strcat(cfgName, ".json");

		if (Serialization::Save(GetConfigFile(cfgName)))
			eventLog.CreateReport("Saved config '%s'", cfgName);
		else
			eventLog.CreateReport("Failed to saved config '%s'", cfgName);

		addedConfig = true;
		name[0] = '\0';
	}
	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::SameLine();
	ImGui::InputText("##Name", name, IM_ARRAYSIZE(name));
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
		configsArray[i] = configs[i].c_str();
	}

	if (ImGui::ListBox("##Configs", &configSelected, configsArray, (int)configs.size(), (int)configs.size())) {
		if (configSelected >= 0 && configSelected < (int)configs.size()) {
			const char* configName = configs[configSelected].c_str();
			if (Serialization::Load(GetConfigFile(configName)))
				eventLog.CreateReport("Loaded config '%s'", configName);
			else
				eventLog.CreateReport("Failed to load config '%s'", configName);
		}
	}
	ImGui::PopItemWidth();
	if (configSelected >= 0 && configSelected < (int)configs.size()) {
		const char* configName = configs[configSelected].c_str();
		if (ImGui::Button("Overwrite config")) {
			if (Serialization::Save(GetConfigFile(configName)))
				eventLog.CreateReport("Saved config '%s'", configName);
			else
				eventLog.CreateReport("Failed to saved config '%s'", configName);
		}
	}
}
