#ifndef UTILS_WEAPONCONFIG
#define UTILS_WEAPONCONFIG

#include "../../GUI/Elements/Popup.hpp"
#include "../../Serialization/Serializer.hpp"
#include "imgui.h"
#include "WeaponClasses.hpp"
#include <type_traits>

class BaseWeaponConfig {
public:
	virtual void setupGUI() = 0;
	virtual SERIALIZER() = 0;
};

template <typename T>
class WeaponConfigurator {
	static_assert(std::is_base_of_v<BaseWeaponConfig, T>, "T is not a BaseWeaponConfig");

	struct OverridableConfig {
		bool overridden = false;
		T config;

		SERIALIZER()
		{
			SERIALIZE("Overridden", overridden);
			SERIALIZE_STRUCT("Config", config);
		}
	};

	T sharedConfig;
	std::map<WeaponClass, OverridableConfig> classConfigs;
	std::map<WeaponID, OverridableConfig> weaponConfigs;

public:
	explicit WeaponConfigurator()
	{
		// Initialize maps with default values for the serialization works
		for (int classIndex = static_cast<int>(WeaponClass::PISTOL); classIndex <= static_cast<int>(WeaponClass::SHOTGUN); classIndex++) {
			auto weaponClass = static_cast<WeaponClass>(classIndex);
			classConfigs[weaponClass] = {};
			for (WeaponID weaponId : weaponClassification[weaponClass]) {
				weaponConfigs[weaponId] = {};
			}
		}
	}

	void buildMenu(T& dest)
	{
		if (ImGui::Popup("Copy from", "Copy from")) {
			if (ImGui::Selectable("Shared")) {
				dest = sharedConfig;
			}

			for (int classIndex = static_cast<int>(WeaponClass::PISTOL); classIndex <= static_cast<int>(WeaponClass::SHOTGUN); classIndex++) {
				auto weaponClass = static_cast<WeaponClass>(classIndex);
				const char* localization = weaponClassLocalization[weaponClass];

				if (ImGui::BeginMenu(localization)) {
					if (ImGui::MenuItem("Shared")) {
						dest = classConfigs[weaponClass].config;
					}
					for (WeaponID weaponId : weaponClassification[weaponClass]) {
						if (ImGui::MenuItem(weaponLocalization[weaponId])) {
							dest = weaponConfigs[weaponId].config;
						}
					}
					ImGui::EndMenu();
				}
			}
			ImGui::EndPopup();
		}
	}

	void showConfig(OverridableConfig& overridableConfig)
	{
		ImGui::Checkbox("Overridden", &overridableConfig.overridden);
		ImGui::SameLine();
		if (overridableConfig.overridden) {
			T& weaponConfig = overridableConfig.config;
			buildMenu(weaponConfig);
			weaponConfig.setupGUI();
		}
	}

	void setupGUI()
	{
		if (ImGui::BeginTabBar("#Weapon config"), ImGuiTabBarFlags_Reorderable) {
			if (ImGui::BeginTabItem("Shared")) {
				sharedConfig.setupGUI();
				ImGui::EndTabItem();
			}

			for (int classIndex = static_cast<int>(WeaponClass::PISTOL); classIndex <= static_cast<int>(WeaponClass::SHOTGUN); classIndex++) {
				auto weaponClass = static_cast<WeaponClass>(classIndex);
				const char* localization = weaponClassLocalization[weaponClass];

				if (ImGui::BeginTabItem(localization)) {
					char str[256] = "#Config for ";
					if (ImGui::BeginTabBar(strcat(str, localization)), ImGuiTabBarFlags_Reorderable) {
						if (ImGui::BeginTabItem("Shared")) {
							showConfig(classConfigs[weaponClass]);
							ImGui::EndTabItem();
						}
						for (WeaponID weaponId : weaponClassification[weaponClass]) {
							if (ImGui::BeginTabItem(weaponLocalization[weaponId])) {
								showConfig(weaponConfigs[weaponId]);
								ImGui::EndTabItem();
							}
						}
						ImGui::EndTabBar();
					}
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
	}

	T* getConfig(WeaponID weaponId)
	{
		if (weaponConfigs.contains(weaponId)) {
			OverridableConfig& weaponConfig = weaponConfigs[weaponId];
			if (weaponConfig.overridden)
				return &weaponConfig.config;
		}

		WeaponClass weaponClass = weaponClassByID(weaponId);
		if (weaponClass == WeaponClass::NONE)
			return nullptr; // This shouldn't happen...

		if (classConfigs.contains(weaponClass)) {
			OverridableConfig& classConfig = classConfigs[weaponClass];
			if (classConfig.overridden)
				return &classConfig.config;
		}

		return getSharedConfig();
	}

	T* getSharedConfig()
	{
		return &sharedConfig;
	}

	SERIALIZER()
	{
		SERIALIZE_STRUCT("Shared", sharedConfig);
		for (int classIndex = static_cast<int>(WeaponClass::PISTOL); classIndex <= static_cast<int>(WeaponClass::SHOTGUN); classIndex++) {
			auto weaponClass = static_cast<WeaponClass>(classIndex);
			SERIALIZE_STRUCT(weaponClassLocalization[weaponClass], classConfigs[weaponClass]);

			for (WeaponID weaponId : weaponClassification[weaponClass]) {
				SERIALIZE_STRUCT(weaponLocalization[weaponId], weaponConfigs[weaponId]);
			}
		}
	}
};

#endif