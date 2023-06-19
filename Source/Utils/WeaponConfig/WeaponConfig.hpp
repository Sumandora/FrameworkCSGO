#ifndef UTILS_WEAPONCONFIG
#define UTILS_WEAPONCONFIG

#include "../../GUI/Elements/Popup.hpp"
#include "../../GUI/ImGuiMacros.hpp"
#include "../../Serialization/Serializer.hpp"
#include "WeaponClasses.hpp"
#include <type_traits>

class BaseWeaponConfig {
public:
	virtual void SetupGUI() = 0;
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
			SERIALIZE(xorstr_("Overridden"), overridden);
			SERIALIZE_STRUCT(xorstr_("Config"), config);
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

	void BuildMenu(T& dest)
	{
		if (ImGui::Popup(xorstr_("Copy from"), xorstr_("Copy from"))) {
			if (ImGui::Selectable(xorstr_("Shared"))) {
				dest = sharedConfig;
			}

			for (int classIndex = static_cast<int>(WeaponClass::PISTOL); classIndex <= static_cast<int>(WeaponClass::SHOTGUN); classIndex++) {
				auto weaponClass = static_cast<WeaponClass>(classIndex);
				const char* localization = weaponClassLocalization[weaponClass];

				if (ImGui::BeginMenu(localization)) {
					if (ImGui::MenuItem(xorstr_("Shared"))) {
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

	void ShowConfig(OverridableConfig& overridableConfig)
	{
		ImGui::Checkbox(xorstr_("Overridden"), &overridableConfig.overridden);
		ImGui::SameLine();
		if (overridableConfig.overridden) {
			T& weaponConfig = overridableConfig.config;
			BuildMenu(weaponConfig);
			weaponConfig.SetupGUI();
		}
	}

	void SetupGUI(){
		TABBAR(xorstr_("#Weapon config"), [&]() {
			TABITEM(xorstr_("Shared"), [&]() {
				sharedConfig.SetupGUI();
			})

			for (int classIndex = static_cast<int>(WeaponClass::PISTOL); classIndex <= static_cast<int>(WeaponClass::SHOTGUN); classIndex++) {
				auto weaponClass = static_cast<WeaponClass>(classIndex);
				const char* localization = weaponClassLocalization[weaponClass];

				TABITEM(localization, [&]() {
					TABBAR(strcat(xorstr_("#Config for "), localization), [&]() {
						TABITEM(xorstr_("Shared"), [&]() {
							ShowConfig(classConfigs[weaponClass]);
						})
						for (WeaponID weaponId : weaponClassification[weaponClass]) {
							TABITEM(weaponLocalization[weaponId], [&]() {
								ShowConfig(weaponConfigs[weaponId]);
							})
						}
					})
				})
			}
		})
	}

	T* GetConfig(WeaponID weaponId)
	{
		if (weaponConfigs.contains(weaponId)) {
			OverridableConfig& weaponConfig = weaponConfigs[weaponId];
			if (weaponConfig.overridden)
				return &weaponConfig.config;
		}

		WeaponClass weaponClass = WeaponClassByID(weaponId);
		if (weaponClass == WeaponClass::NONE)
			return nullptr; // This shouldn't happen...

		if (classConfigs.contains(weaponClass)) {
			OverridableConfig& classConfig = classConfigs[weaponClass];
			if (classConfig.overridden)
				return &classConfig.config;
		}

		return GetSharedConfig();
	}

	T* GetSharedConfig()
	{
		return &sharedConfig;
	}

	SERIALIZER()
	{
		SERIALIZE_STRUCT(xorstr_("Shared"), sharedConfig);
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