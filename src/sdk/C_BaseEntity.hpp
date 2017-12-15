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
	inline IClientNetworkable* GetNetworkable()
	{
		return (IClientNetworkable*)(this + 0x8);
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
	C_BaseCombatWeapon* GetWeapon()
	{
		return (C_BaseCombatWeapon*)entitylist->GetClientEntityFromHandle(GetNetVar<unsigned long>("m_hActiveWeapon"));
	}
};
