#ifndef FEATURES_VISUALS
#define FEATURES_VISUALS

#include "../../SDK/GameClass/CUserCmd.hpp"
#include "../../SDK/GameClass/CViewSetup.hpp"
#include "../../SDK/GameClass/IMaterial.hpp"
#include "../../SDK/Math/Matrix4x4.hpp"

#include "../../Serialization/Serializer.hpp"

#include "ESP/ESPStructure.hpp"

namespace Features::Visuals {
	namespace Esp {
		// Settings
		extern bool considerSpottedEntitiesAsVisible;
		extern bool considerSmokedOffEntitiesAsOccluded;
		extern bool alignBoundingBox;
		extern bool outOfView;
		extern float outOfViewSize;
		extern float outOfViewDistance;
		extern PlayerSettings players;

		void Update();
		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace SpectatorList {
		void Update();
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
	namespace FlashReduction {
		bool ViewDrawFade(std::byte* color, IMaterial*& pFadeMaterial);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace Fog {
		void FrameStageNotify();

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
