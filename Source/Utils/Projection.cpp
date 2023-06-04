#include "Projection.hpp"

#include "../Hooks/Game/GameFunctions.hpp"
#include "../SDK/Math/Matrix4x4.hpp"

bool Utils::Project(const Vector& world, ImVec2& screen)
{
	const Matrix4x4& matrix = Hooks::Game::FrameStageNotify::worldToScreenMatrix;

	const float z = matrix[2][0] * world.x + matrix[2][1] * world.y + matrix[2][2] * world.z + matrix[2][3];
	const float w = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];
	if (z <= 0.0f || w <= 0.0f)
		return false;

	screen = ImGui::GetIO().DisplaySize;
	screen.x *= 0.5f;
	screen.y *= 0.5f;

	screen.x *= 1.0f + (matrix[0][0] * world.x + matrix[0][1] * world.y + matrix[0][2] * world.z + matrix[0][3]) / w;
	screen.y *= 1.0f - (matrix[1][0] * world.x + matrix[1][1] * world.y + matrix[1][2] * world.z + matrix[1][3]) / w;
	return true;
}
