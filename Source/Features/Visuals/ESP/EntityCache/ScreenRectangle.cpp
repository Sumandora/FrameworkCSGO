#include "EntityCache.hpp"

#include "../../../../Utils/Projection.hpp"
#include "../../../../Utils/Trigonometry.hpp"
#include "../ESP.hpp"

bool ScreenRectangle::CalculateScreenRectangle(ImVec4& rectangle)
{
	const std::optional<BoundingBox>& boundingBox = this->boundingBox;

	if (!boundingBox.has_value())
		return false;

	const Vector min = origin + boundingBox->mins;
	const Vector max = origin + boundingBox->maxs;

	const Vector points[] = {
		// Lower
		Vector(min.x, min.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, min.y, max.z),
		Vector(min.x, min.y, max.z),
		// Higher
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z)
	};

	rectangle = { FLT_MAX, FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (const auto& point : points) {
		ImVec2 point2D;

		if (!Utils::Project(point, point2D)) {
			return false;
		}

		if (point2D.x < rectangle.x)
			rectangle.x = point2D.x;
		else if (point2D.x > rectangle.z)
			rectangle.z = point2D.x;

		if (point2D.y < rectangle.y)
			rectangle.y = point2D.y;
		else if (point2D.y > rectangle.w)
			rectangle.w = point2D.y;
	}

	if (!std::isfinite(rectangle.x) || !std::isfinite(rectangle.y) || !std::isfinite(rectangle.z) || !std::isfinite(rectangle.w)) {
		return false;
	}

	if (esp.alignBoundingBox) {
		rectangle.x = std::round(rectangle.x);
		rectangle.y = std::round(rectangle.y);
		rectangle.z = std::round(rectangle.z);
		rectangle.w = std::round(rectangle.w);
	}

	return true;
}

bool ScreenRectangle::HandleOutOfView(const Vector& localOrigin, const Vector& viewangles, ImVec4& rectangle) const
{
	if (esp.outOfView) {
		const Vector delta = origin - localOrigin;
		const float angle = (float)DEG2RAD(viewangles.y - 90.0f) - atan2f(delta.y, delta.x);

		const ImVec2 display = ImGui::GetIO().DisplaySize;
		const ImVec2 direction(cosf(angle), sinf(angle));

		const ImVec2 screenPosition(
			(float)display.x / 2.0f + direction.x * esp.outOfViewDistance,
			(float)display.y / 2.0f + direction.y * esp.outOfViewDistance);

		rectangle = ImVec4(
			screenPosition.x - esp.outOfViewSize,
			screenPosition.y - esp.outOfViewSize,

			screenPosition.x + esp.outOfViewSize,
			screenPosition.y + esp.outOfViewSize);

		return true;
	}
	return false;
}

void ScreenRectangle::Update(const Vector& origin, const std::optional<BoundingBox>& boundingBox)
{
	this->origin = origin;
	this->boundingBox = boundingBox;
}

std::optional<ImVec4> ScreenRectangle::Get()
{
	std::optional<LocalPlayer> localPlayer = EntityCache::localPlayer;

	if (!localPlayer.has_value())
		return std::nullopt;

	ImVec4 rectangle;
	bool visible = CalculateScreenRectangle(rectangle);

	if (!visible && HandleOutOfView(localPlayer->origin, localPlayer->viewangles, rectangle)) // TODO Buy menu makes oov flicker
		visible = true; // We just made them visible ^^

	if (!visible)
		return std::nullopt;

	return rectangle;
}