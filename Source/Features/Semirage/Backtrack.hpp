#ifndef FEATURES_SEMIRAGE_BACKTRACK
#define FEATURES_SEMIRAGE_BACKTRACK

#include "../Features.hpp"

#include "imgui.h"

#include "../../SDK/Definitions/Bones.hpp"
#include "../../SDK/GameClass/CUserCmd.hpp"
#include "../../SDK/Math/Matrix3x4.hpp"

inline class Backtrack : public Feature {
public:
	Backtrack()
		: Feature("Semirage", "Backtrack")
	{
	}

	bool enabled = false;
	float scale = 1.0f;
	bool accountForOutgoingPing = false;
	bool friendlyFire = false;
	bool visualize = false;

	struct Tick {
		float simulationTime{};
		int tickCount{};
		Vector origin;
		Matrix3x4 boneMatrix[MAXSTUDIOBONES];
	};

	std::map<int, std::vector<Backtrack::Tick>> ticks;

private:
	bool isTickValid(const Tick& tick) const;

public:
	void createMove(CUserCmd* cmd);
	void frameStageNotify();
	void imGuiRender(ImDrawList* drawList);

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} backtrack;

#endif