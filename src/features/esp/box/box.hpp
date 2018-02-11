#pragma once

bool ESP::CreateBox(C_BaseEntity* p, int& x, int& y, int& w, int& h)
{
	static const Vector edges[4] =
	{
		Vector(1.0f, 1.0f, 1.0f),
		Vector(-1.0f, 1.0f, 1.0f),
		Vector(1.0f, -1.0f, 1.0f),
		Vector(-1.0f, -1.0f, 1.0f),
	};

	Vector origin = p->GetAbsOrigin();

	Angle rot = Angle(0, p->GetEyeAngle().y, 0);

	const model_t* model = p->GetModel();
	if (!model)
		return false;

	Vector mins, maxs;
	modelinfo->GetModelRenderBounds(model, mins, maxs);

	if (p->IsDucked())
		maxs.z -= 16.f;

	int screenw, screeh;
	engineclient->GetScreenSize(screenw, screeh);

	x = screenw;
	y = screeh;
	w = -screenw;
	h = -screeh;

	for (int i = 0; i < 4; i++)
	{
		Vector mins2d, maxs2d;

		if (!WorldToScreen(origin + (mins * edges[i].Rotate(rot)), mins2d))
			return false;

		if (!WorldToScreen(origin + (maxs * edges[i].Rotate(rot)), maxs2d))
			return false;

		x = std::min<int>(x, (int)ceil(std::min(mins2d.x, maxs2d.x)));
		y = std::min<int>(y, (int)ceil(std::min(mins2d.y, maxs2d.y)));
		w = std::max<int>(w, (int)ceil(std::max(mins2d.x, maxs2d.x)));
		h = std::max<int>(h, (int)ceil(std::max(mins2d.y, maxs2d.y)));
	}

	w -= x;
	h -= y;

	return true;
}

void ESP::PlayerBox()
{
	if (p->GetTeam() == lp->GetTeam())
		matsystemsurface->SetDrawColor(Color(72, 133, 237));
	else
		matsystemsurface->SetDrawColor(Color(219, 50, 54));

	matsystemsurface->DrawOutlinedRect(x, y, w, h);

	matsystemsurface->SetDrawColor(Color(0, 0, 0));
	matsystemsurface->DrawOutlinedRect(x + 1, y + 1, w - 2, h - 2);
	matsystemsurface->DrawOutlinedRect(x - 1, y - 1, w + 2, h + 2);

	matsystemsurface->DrawFilledRect(x - 5, y - 1, 3, h + 2);
}
