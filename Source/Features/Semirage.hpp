#ifndef FEATURES_SEMIRAGE
#define FEATURES_SEMIRAGE

#include "../SDK/GameClass/CUserCmd.hpp"

#include "../Serialization/Serializer.hpp"

namespace Features::Semirage {
	namespace Aimbot {
		bool CreateMove(CUserCmd* cmd);
		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace Backtrack {
		void CreateMove(CUserCmd* cmd);
		void FrameStageNotify();
		void ImGuiRender(ImDrawList* drawList);

		// GUI
		void SetupGUI();

		// Serialization
		DECLARE_SERIALIZER(Serializer)
	}
	namespace RecoilAssistance {
		bool CreateMove(CUserCmd* cmd);

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
