#include "../Glow.hpp"

#include "imgui.h"

#include "../../../../GUI/Elements/ClickableColorButton.hpp"
#include "../../../../GUI/Elements/Popup.hpp"

#include "../../../../SDK/GameClass/CGlowObjectManager.hpp"
#include <cstring>

bool Glow::GlowSettings::apply(int entindex, CBaseEntity* entity) const
{
	if (!enabled)
		return false;

	GlowObjectDefinition* glowObject = Memory::glowObjectManager->GetGlowEffect(entity);
	GlowObjectDefinition newGlowObject;
	newGlowObject.m_pEntity = entity;
	newGlowObject.m_vGlowColor = { color.Value.x, color.Value.y, color.Value.z };
	newGlowObject.m_flGlowAlpha = color.Value.w;

	newGlowObject.m_bGlowAlphaCappedByRenderAlpha = false; // Makes a difference, but not with players and guns, from what I can tell
	newGlowObject.m_flGlowAlphaFunctionOfMaxVelocity = 0; // Invisible when > 0 and doesn't change anything when < 0
	newGlowObject.m_flGlowAlphaMax = 1.0f; // We don't need 2 alpha sliders
	newGlowObject.m_flGlowPulseOverdrive = 1.0f; // No difference?
	newGlowObject.m_bRenderWhenOccluded = true; // One of these have to be enabled in order for the glow to be visible
	newGlowObject.m_bRenderWhenUnoccluded = false; // It seems like these don't change anything either, despite their name
	newGlowObject.m_bFullBloomRender = style == 0 && filled;
	newGlowObject.m_nFullBloomStencilTestValue = 0; // This seems to change nothing
	newGlowObject.m_nRenderStyle = style;
	newGlowObject.m_nSplitScreenSlot = -1;
	if (glowObject) {
		newGlowObject.m_nNextFreeSlot = glowObject->m_nNextFreeSlot;
		*glowObject = newGlowObject;
		return true;
	}

	glow.customGlows[entindex] = Memory::glowObjectManager->RegisterGlowObject(newGlowObject);
	return true;
}

void Glow::GlowSettings::setupGUI()
{
	if (ImGui::Popup("Copy from", "Copy from")) {
		if (ImGui::BeginMenu("Players")) {
			if (ImGui::Selectable("Teammate")) {
				*this = glow.players.teammate;
			}
			if (ImGui::Selectable("Enemy")) {
				*this = glow.players.enemy;
			}
			if (ImGui::Selectable("Local")) {
				*this = glow.players.local;
			}
			ImGui::EndMenu();
		}
		if (ImGui::Selectable("Weapons")) {
			*this = glow.weapons;
		}
		if (ImGui::Selectable("Ragdolls")) {
			*this = glow.ragdolls;
		}
		ImGui::EndPopup();
	}

	ImGui::Checkbox("Enabled", &enabled);
	ImGui::ClickableColorButton("Color", color);
	ImGui::Combo("Style", &style, "Default\0RimGlow3D\0Edge Highlight\0Edge Highlight Pulse\0");
	ImGui::Checkbox("Filled", &filled); // Only makes a difference like this
}

SCOPED_SERIALIZER(Glow::GlowSettings)
{
	SERIALIZE("Enabled", enabled);
	SERIALIZE_VECTOR4D("Color", color.Value);
	SERIALIZE("Style", style);
	SERIALIZE("Filled", filled);
}