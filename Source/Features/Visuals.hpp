#ifndef FEATURES_VISUALS
#define FEATURES_VISUALS

#include "../SDK/GameClass/CViewSetup.hpp"
#include "../Serialization/Serializer.hpp"
#include "Visuals/ESP/ESPStructure.hpp"

namespace Features::Visuals {
	namespace Esp {
		// Settings
		extern PlayerSettings players;

		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace SpectatorList {
		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace NoPunch {
		void HidePunch();
		void RestorePunch();

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace FOV {
		void OverrideView(CViewSetup* pSetup);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}

	// GUI
	void SetupGUI();

	// Serialization
	DECLARE_SERIALIZER(Serializer)
}

#endif
