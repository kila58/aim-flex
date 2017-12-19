#pragma once

class CMoveData;

class CGameMovement
{
public:
	void ProcessMovement(C_BaseEntity* player, CMoveData* move)
	{
		return getvfunc<void(__thiscall*)(void*, C_BaseEntity*, CMoveData*)>(this, 1)(this, player, move);
	}
	CMoveData* GetMoveData()
	{
		return *(CMoveData**)(this + 8);
	}
};
