#ifndef SDK_GAMECLASS_CGLOWOBJECTDEFINITION
#define SDK_GAMECLASS_CGLOWOBJECTDEFINITION

#include "GameClass/Entities/CBaseEntity.hpp"

struct GlowObjectDefinition {
	int m_nNextFreeSlot;

	CBaseEntity* m_pEntity;
	Vector m_vGlowColor;
	float m_flGlowAlpha;

	bool m_bGlowAlphaCappedByRenderAlpha;
	float m_flGlowAlphaFunctionOfMaxVelocity;
	float m_flGlowAlphaMax;
	float m_flGlowPulseOverdrive;
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloomRender;
	int m_nFullBloomStencilTestValue;
	int m_nRenderStyle;
	int m_nSplitScreenSlot;

	static constexpr int END_OF_FREE_LIST = -1;
	static constexpr int ENTRY_IN_USE = -2;

	inline bool IsUnused() const
	{
		return m_nNextFreeSlot != ENTRY_IN_USE;
	}
};

#endif
