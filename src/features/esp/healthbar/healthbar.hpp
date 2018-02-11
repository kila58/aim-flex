#pragma once

void ESP::PlayerHealthBar()
{
	int hp = p->GetHealth();
	int height = (int)((clamp((float)hp / 100.f, 0.f, 100.f)) * h);

	matsystemsurface->SetDrawColor(HSVtoRGB(hp / 100.f * 120.f, 1.f, 1.f));
	matsystemsurface->DrawFilledRect(x - 4, y + (h - height), 1, height);
}
