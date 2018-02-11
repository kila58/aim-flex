#pragma once

void ESP::PlayerBacktrack()
{
	auto& player = playermanager.GetPlayer(p);

	Vector lastscreen;
	bool prev = false;

	for (auto& tick : player.backtrackinfo.ticks)
	{
		Vector center = tick.hitboxinfo.head;
		Vector screen;

		if (WorldToScreen(center, screen) && prev)
		{
			matsystemsurface->SetDrawColor(Color(255, 255, 255));
			matsystemsurface->DrawLine(lastscreen.x, lastscreen.y, screen.x, screen.y);
		}

		lastscreen = screen;

		prev = true;
	}
}
