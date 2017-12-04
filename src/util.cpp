#include "util.hpp"

#include "aim-flex.hpp"

bool ScreenTransform(const Vector& point, Vector& screen)
{
	const VMatrix& matrix = g_EngineClient->WorldToScreenMatrix();
	float w;

	screen.x = matrix[0][0] * point[0] + matrix[0][1] * point[1] + matrix[0][2] * point[2] + matrix[0][3];
	screen.y = matrix[1][0] * point[0] + matrix[1][1] * point[1] + matrix[1][2] * point[2] + matrix[1][3];

	w = matrix[3][0] * point[0] + matrix[3][1] * point[1] + matrix[3][2] * point[2] + matrix[3][3];

	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f)
	{
		behind = true;

		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		float invw = 1.0f / w;

		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

bool WorldToScreen(const Vector& point, Vector& screen)
{
	if (!ScreenTransform(point, screen))
	{
		int w, h;
		g_EngineClient->GetScreenSize(&w, &h);

		screen.x = (w * 0.5f) + (screen.x * w) * 0.5f;
		screen.y = (h * 0.5f) - (screen.y * h) * 0.5f;

		return true;
	}

	return false;
}
