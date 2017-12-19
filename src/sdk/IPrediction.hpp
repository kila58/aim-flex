#pragma once

class IPrediction
{
public:
	void SetupMove(C_BaseEntity* ent, CUserCmd* cmd, IMoveHelper* helper, CMoveData* move)
	{
		return getvfunc<void(__thiscall*)(void*, C_BaseEntity*, CUserCmd*, IMoveHelper*, CMoveData*)>(this, 20)(this, ent, cmd, helper, move);
	}
	void FinishMove(C_BaseEntity* ent, CUserCmd* cmd, CMoveData* move)
	{
		return getvfunc<void(__thiscall*)(void*, C_BaseEntity*, CUserCmd*, CMoveData*)>(this, 21)(this, ent, cmd, move);
	}
};
