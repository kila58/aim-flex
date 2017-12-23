#pragma once

struct FileWeaponInfo_t;

class C_BaseCombatWeapon : public C_BaseEntity
{

public:
	FileWeaponInfo_t* GetCSWpnData()
	{
		return getvfunc<FileWeaponInfo_t*(__thiscall*)(void*)>(this, 446)(this);
	}
	int GetAmmo()
	{
		return GetNetVar<int>("m_iClip1");
	}
	Activity GetActivity()
	{
		return *(Activity*)(this + GetOffset("m_iClip1") + 0x30);
	}
};

struct FileWeaponInfo_t
{
public:
	char pad001[4];
	char* szClassName;
};
