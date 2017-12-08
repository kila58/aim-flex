#pragma once

struct FileWeaponInfo_t;

class C_BaseCombatWeapon
{
public:
	FileWeaponInfo_t* GetCSWpnData()
	{
		return getvfunc<FileWeaponInfo_t*(__thiscall*)(void*)>(this, 446)(this);
	}
};

struct FileWeaponInfo_t
{
public:
	char pad001[4];
	char* szClassName;
};
