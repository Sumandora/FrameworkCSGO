#ifndef FEATURES_VISUALS
#define FEATURES_VISUALS

#include "../SDK/GameClass/CUserCmd.hpp"
#include "../SDK/GameClass/CViewSetup.hpp"
#include "../SDK/Math/Matrix4x4.hpp"
#include "../Serialization/Serializer.hpp"
#include "Visuals/ESP/ESPStructure.hpp"

namespace Features::Visuals {
	namespace Esp {
		// Settings
		extern PlayerSettings players;

		void UpdateVisibility();
		void DrawEntity(ImDrawList* drawList, CBaseEntity* entity, CBasePlayer* localPlayer, CBaseEntity* spectatorEntity, Vector viewangles, Matrix4x4 matrix);
		void ImGuiRender(ImDrawList* drawList);

		bool WorldToScreen(Matrix4x4& matrix, const Vector& worldPosition, ImVec2& screenPosition);

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
