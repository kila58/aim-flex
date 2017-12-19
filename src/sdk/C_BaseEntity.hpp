#pragma once

class IClientNetworkable
{
public:
	ClientClass* GetClientClass()
	{
		return getvfunc<ClientClass*(__thiscall*)(void*)>(this, 2)(this);
	}
	bool IsDormant()
	{
		return getvfunc<bool(__thiscall*)(void*)>(this, 9)(this);
	}
};

class IClientRenderable
{
public:
	model_t* GetModel()
	{
		return getvfunc<model_t*(__thiscall*)(void*)>(this, 8)(this);
	}
	bool SetupBones(VMatrix* bones, float time, int _bones = 128, int _flag = 256)
	{
		return getvfunc<bool(__thiscall*)(void*, VMatrix*, int bones, int flag, float time)>(this, 13)(this, bones, _bones, _flag, time);
	}
};

class C_BaseCombatWeapon;
class IClientEntityList;

extern IClientEntityList* entitylist;

class C_BaseEntity
{
	template <typename T>
	T GetNetVar(const char* name)
	{
		int offset = netvars.Get(name, GetNetworkable()->GetClientClass()->m_pRecvTable);
		return *(T*)(this + offset);
	}
public:
	inline IClientRenderable* GetRenderable()
	{
		return (IClientRenderable*)(this + 4);
	}
	inline IClientNetworkable* GetNetworkable()
	{
		return (IClientNetworkable*)(this + 8);
	}
	Vector& GetAbsOrigin()
	{
		return getvfunc<Vector&(__thiscall*)(void*)>(this, 10)(this);
	}
	bool IsDormant()
	{
		return GetNetworkable()->IsDormant();
	}
	bool IsAlive()
	{
		return (GetNetVar<int>("m_lifeState") & 0xFF) == LIFE_ALIVE;
	}
	int GetTeam()
	{
		return GetNetVar<int>("m_iTeamNum");
	}
	int GetHealth()
	{
		return GetNetVar<int>("m_iHealth");
	}
	bool IsOnGround()
	{
		return GetNetVar<int>("m_fFlags") & FL_ONGROUND;
	}
	Vector GetMins()
	{
		return GetNetVar<Vector>("m_vecMins");
	}
	Vector GetMaxs()
	{
		return GetNetVar<Vector>("m_vecMaxs");
	}
	int GetTickBase()
	{
		return GetNetVar<int>("m_nTickBase");
	}
	int GetHitboxSet()
	{
		return GetNetVar<int>("m_nHitboxSet");
	}
	Vector GetEyeOffset()
	{
		return GetNetVar<Vector>("m_vecViewOffset[0]");
	}
	Angle GetAimPunch()
	{
		return GetNetVar<Angle>("m_aimPunchAngle");
	}
	C_BaseCombatWeapon* GetWeapon()
	{
		return (C_BaseCombatWeapon*)entitylist->GetClientEntityFromHandle(GetNetVar<unsigned long>("m_hActiveWeapon"));
	}
	inline bool SetupBones(VMatrix* bones, float time = 0.f)
	{
		return GetRenderable()->SetupBones(bones, time);
	}
	inline model_t* GetModel()
	{
		return GetRenderable()->GetModel();
	}
};
