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
	void UpdateAccuracyPenalty()
	{
		return getvfunc<void(__thiscall*)(void*)>(this, 470)(this);
	}
	float GetInaccuracy()
	{
		return getvfunc<float(__thiscall*)(void*)>(this, 469)(this);
	}
	float GetSpread()
	{
		return getvfunc<float(__thiscall*)(void*)>(this, 439)(this);
	}
};

struct FileWeaponInfo_t
{
public:
	char pad1[4];
	char* szClassName; //0x0004
	char _0x0000[228];
	__int32 m_iDamage; //0x00EC
	float m_flArmorRatio; //0x00F0
	char _0x00F4[4];
	float m_flPenetration; //0x00F8
	char _0x00FC[8];
	float m_flRange; //0x0104
	float m_flRangeModifier; //0x0108
	char _0x010C[52];
};
