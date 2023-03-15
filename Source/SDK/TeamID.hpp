#ifndef SDK_TEAMID
#define SDK_TEAMID

enum class TeamID : int {
	TEAM_UNASSIGNED,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST
};

inline constexpr bool IsParticipatingTeam(TeamID team)
{
	return team == TeamID::TEAM_TERRORIST || team == TeamID::TEAM_COUNTER_TERRORIST;
}

#endif
