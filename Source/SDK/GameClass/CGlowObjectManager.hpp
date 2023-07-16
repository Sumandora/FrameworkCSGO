#ifndef SDK_GAMECLASS_CGLOWOBJECTMANAGER
#define SDK_GAMECLASS_CGLOWOBJECTMANAGER

#include "../CUtl/Vector.hpp"
#include "../GlowObjectDefinition.hpp"

class CGlowObjectManager {
public:
	CUtlVector<GlowObjectDefinition> m_GlowObjectDefinitions;
	int m_nFirstFreeSlot;

	int RegisterGlowObject(GlowObjectDefinition& glowObjectDefinition)
	{
		int nIndex;
		if (m_nFirstFreeSlot == GlowObjectDefinition::END_OF_FREE_LIST) {
			// TODO: nIndex = m_GlowObjectDefinitions.AddToTail();
			return -1;
		} else {
			nIndex = m_nFirstFreeSlot;
			m_nFirstFreeSlot = m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot;
		}

		glowObjectDefinition.m_nNextFreeSlot = GlowObjectDefinition::ENTRY_IN_USE;
		m_GlowObjectDefinitions[nIndex] = glowObjectDefinition;

		return nIndex;
	}

	void UnregisterGlowObject(int nGlowObjectHandle)
	{
		m_GlowObjectDefinitions[nGlowObjectHandle].m_nNextFreeSlot = m_nFirstFreeSlot;
		m_GlowObjectDefinitions[nGlowObjectHandle].m_pEntity = NULL;
		m_nFirstFreeSlot = nGlowObjectHandle;
	}

	GlowObjectDefinition* GetGlowEffect(CBaseEntity* entity)
	{
		for (int i = 0; i < m_GlowObjectDefinitions.Count(); ++i) {
			if (!m_GlowObjectDefinitions[i].IsUnused() && m_GlowObjectDefinitions[i].m_pEntity == entity) {
				return &m_GlowObjectDefinitions[i];
			}
		}

		return nullptr;
	};
};

#endif
