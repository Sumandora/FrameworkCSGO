#ifndef FEATURES_MISC_VOTEREVEALER
#define FEATURES_MISC_VOTEREVEALER

#include "../Features.hpp"

#include "../../SDK/GameClass/CGameEvent.hpp"

#include "imgui.h"

inline class VoteRevealer : public Feature {
public:
	VoteRevealer()
		: Feature("Misc", "Vote revealer")
	{
	}

	// TODO Reveal opponent votes

	bool revealVoters;

	void fireEvent(CGameEvent* gameEvent);

	virtual void setupGUI() override;
	virtual SERIALIZER() override;
} voteRevealer;

#endif