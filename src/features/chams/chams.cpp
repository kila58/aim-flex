#include "chams.hpp"

#include "../../aim-flex.hpp"

#include "../../features/playermanager/playermanager.hpp"
#include "../../features/settings/settings.hpp"

/*
void Chams::Init()
{
	//if (!mat)
	if (false)
	{
		mat = materialsystem->FindMaterial("vgui/white_additive", "Model texture");

		if (mat)
		{
			mat->IncrementReferenceCount();

			mat->SetMaterialVarFlag(MATERIAL_VAR_ADDITIVE, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXCOLOR, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXALPHA, false);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NOFOG, true);
			mat->SetMaterialVarFlag(MATERIAL_VAR_FLAT, true);
		}
	}
}
*/

void Chams::Init()
{
	if (true)
		return;

	mat = materialsystem->FindMaterial("vgui/white_additive", "Model texture");

	mat->SetMaterialVarFlag(MATERIAL_VAR_ADDITIVE, false);
	mat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXCOLOR, false);
	mat->SetMaterialVarFlag(MATERIAL_VAR_VERTEXALPHA, false);
	mat->SetMaterialVarFlag(MATERIAL_VAR_NOFOG, true);
	mat->SetMaterialVarFlag(MATERIAL_VAR_FLAT, true);
}

bool Chams::InvalidPlayerChams(int i, C_BaseEntity* p, C_BaseEntity* lp)
{
	if (!p)
		return true;

	if (p == lp)
		return true;

	static player_info_t info;

	if (!engineclient->GetPlayerInfo(i, &info))
		return true;

	return false;
}

void Chams::Invoke()
{
	if (true)
		return;
	
	//Init();

	drawing = true;

	if (settings.Get<bool>("visuals_enabled") && settings.Get<bool>("visuals_chams_enabled"))
	//if (false)
	{
		if (mat)
		{
			modelrender->ForcedMaterialOverride(mat);

			C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
			if (!lp)
				return;

			for (int i = 1; i <= globals->maxClients; i++)
			{
				C_BaseEntity* p = entitylist->GetClientEntity(i);

				if (InvalidPlayerChams(i, p, lp))
					continue;

				Vector old_origin = p->GetAbsOrigin();
				Angle old_eyeangle = p->GetAbsAngles();

				auto& player = playermanager.GetPlayer(p);
				if (player)
				{
					if (player.backtrackinfo.ticks.size() > 0)
					{
						// todo: do a proper restore
						p->SetAbsOrigin(player.backtrackinfo.ticks.front().absorigin);
						p->SetAbsAngles(player.backtrackinfo.ticks.front().absangle);

						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

						renderview->SetBlend(0.9f);
						renderview->SetColor(0.f, 0.f, 0.f);

						p->DrawModel();
					}
				}

				p->SetAbsOrigin(old_origin);
				p->SetAbsAngles(old_eyeangle);

				bool team = p->GetTeam() == lp->GetTeam();
				renderview->SetBlend(1.f);

				// non visible
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

				if (team)
					renderview->SetColor(0.f, 100.f, 255.f);
				else
					renderview->SetColor(255.f, 46.f, 0.f);

				p->DrawModel();

				// visible
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

				if (team)
					renderview->SetColor(46.f, 255.f, 46.f);
				else
					renderview->SetColor(255.f, 201.f, 0.f);

				p->DrawModel();
			}

			modelrender->ForcedMaterialOverride(0);
		}
	}

	drawing = false;
}

void Chams::Destroy()
{
	// need to use g_pMemAlloc
	delete mat;
}

Chams chams;
