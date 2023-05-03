#include "MouseCorrection.hpp"

void InverseApplyMouse(const Vector& newViewAngles, const Vector& oldViewAngles, float& mouse_x, float& mouse_y)
{
	mouse_x = (oldViewAngles.y - newViewAngles.y) / ConVarStorage::m_yaw()->GetFloat();
	mouse_y = (newViewAngles.y - oldViewAngles.y) / ConVarStorage::m_pitch()->GetFloat();
}

void Utils::CorrectMouseDeltas(CUserCmd* cmd)
{
	if(cmd->viewangles == cmd->viewangles_copy)
		return;

	float mouse_x;
	float mouse_y;

	InverseApplyMouse(cmd->viewangles, cmd->viewangles_copy /* This thing actually comes in handy, thanks valve */, mouse_x, mouse_y);

	cmd->mousedx += (int)mouse_x;
	cmd->mousedy += (int)mouse_y;
}
