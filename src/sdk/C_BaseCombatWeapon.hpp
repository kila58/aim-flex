#pragma once

class CCSWeaponData;
enum ItemDefinitionIndex : short;

class C_BaseCombatWeapon : public C_BaseEntity
{
public:
	CCSWeaponData* GetCSWpnData()
	{
		return getvfunc<CCSWeaponData*(__thiscall*)(void*)>(this, 448)(this);
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
		return getvfunc<void(__thiscall*)(void*)>(this, 472)(this);
	}
	float GetInaccuracy()
	{
		return getvfunc<float(__thiscall*)(void*)>(this, 471)(this);
	}
	float GetSpread()
	{
		return getvfunc<float(__thiscall*)(void*)>(this, 439)(this);
	}
	ItemDefinitionIndex GetItemDefinitionIndex()
	{
		return GetNetVar<ItemDefinitionIndex>("m_iItemDefinitionIndex");
	}
};

enum ItemDefinitionIndex : short
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER
};

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

/*
class CCSWeaponData
{
public:

	virtual ~CCSWeaponData() {};

	char*        consoleName;            // 0x0004
	char        pad_0008[12];            // 0x0008
	int            iMaxClip1;                // 0x0014
	int            iMaxClip2;                // 0x0018
	int            iDefaultClip1;            // 0x001C
	int            iDefaultClip2;            // 0x0020
	char        pad_0024[8];            // 0x0024
	char*        szWorldModel;            // 0x002C
	char*        szViewModel;            // 0x0030
	char*        szDroppedModel;            // 0x0034
	char        pad_0038[4];            // 0x0038
	char*        N0000023E;                // 0x003C
	char        pad_0040[56];            // 0x0040
	char*        szEmptySound;            // 0x0078
	char        pad_007C[4];            // 0x007C
	char*        szBulletType;            // 0x0080
	char        pad_0084[4];            // 0x0084
	char*        szHudName;                // 0x0088
	char*        szWeaponName;            // 0x008C
	char        pad_0090[56];            // 0x0090
	CSWeaponType   WeaponType;                // 0x00C8
	int            iWeaponPrice;            // 0x00CC
	int            iKillAward;                // 0x00D0
	char*        szAnimationPrefix;        // 0x00D4
	float        flCycleTime;            // 0x00D8
	float        flCycleTimeAlt;            // 0x00DC
	float        flTimeToIdle;            // 0x00E0
	float        flIdleInterval;            // 0x00E4
	bool        bFullAuto;                // 0x00E8
	char        pad_0x00E5[3];            // 0x00E9
	int            iDamage;                // 0x00EC
	float        flArmorRatio;            // 0x00F0
	int            iBullets;                // 0x00F4
	float        flPenetration;            // 0x00F8
	float        flFlinchVelocityModifierLarge;    // 0x00FC
	float        flFlinchVelocityModifierSmall;    // 0x0100
	float        flRange;                // 0x0104
	float        flRangeModifier;        // 0x0108
	float        flThrowVelocity;        // 0x010C
	char        pad_0x010C[12];            // 0x0110
	bool        bHasSilencer;            // 0x011C
	char        pad_0x0119[3];            // 0x011D
	char*        pSilencerModel;            // 0x0120
	int            iCrosshairMinDistance;    // 0x0124
	int            iCrosshairDeltaDistance;// 0x0128 - iTeam?
	float        flMaxPlayerSpeed;        // 0x012C
	float        flMaxPlayerSpeedAlt;    // 0x0130
	float        flSpread;                // 0x0134
	float        flSpreadAlt;            // 0x0138
	float        flInaccuracyCrouch;        // 0x013C
	float        flInaccuracyCrouchAlt;    // 0x0140
	float        flInaccuracyStand;        // 0x0144
	float        flInaccuracyStandAlt;    // 0x0148
	float        flInaccuracyJumpInitial;// 0x014C
	float        flInaccuracyJump;        // 0x0150
	float        flInaccuracyJumpAlt;    // 0x0154
	float        flInaccuracyLand;        // 0x0158
	float        flInaccuracyLandAlt;    // 0x015C
	float        flInaccuracyLadder;        // 0x0160
	float        flInaccuracyLadderAlt;    // 0x0164
	float        flInaccuracyFire;        // 0x0168
	float        flInaccuracyFireAlt;    // 0x016C
	float        flInaccuracyMove;        // 0x0170
	float        flInaccuracyMoveAlt;    // 0x0174
	float        flInaccuracyReload;        // 0x0178
	int            iRecoilSeed;            // 0x017C
	float        flRecoilAngle;            // 0x0180
	float        flRecoilAngleAlt;        // 0x0184
	float        flRecoilAngleVariance;    // 0x0188
	float        flRecoilAngleVarianceAlt;    // 0x018C
	float        flRecoilMagnitude;        // 0x0190
	float        flRecoilMagnitudeAlt;    // 0x0194
	float        flRecoilMagnitudeVariance;    // 0x0198
	float        flRecoilMagnitudeVarianceAlt;    // 0x019C
	float        flRecoveryTimeCrouch;    // 0x01A0
	float        flRecoveryTimeStand;    // 0x01A4
	float        flRecoveryTimeCrouchFinal;    // 0x01A8
	float        flRecoveryTimeStandFinal;    // 0x01AC
	int            iRecoveryTransitionStartBullet;// 0x01B0
	int            iRecoveryTransitionEndBullet;    // 0x01B4
	bool        bUnzoomAfterShot;        // 0x01B8
	bool        bHideViewModelZoomed;    // 0x01B9
	char        pad_0x01B5[2];            // 0x01BA
	char        iZoomLevels[3];            // 0x01BC
	int            iZoomFOV[2];            // 0x01C0
	float        fZoomTime[3];            // 0x01C4
	char*        szWeaponClass;            // 0x01D4
	float        flAddonScale;            // 0x01D8
	char        pad_0x01DC[4];            // 0x01DC
	char*        szEjectBrassEffect;        // 0x01E0
	char*        szTracerEffect;            // 0x01E4
	int            iTracerFrequency;        // 0x01E8
	int            iTracerFrequencyAlt;    // 0x01EC
	char*        szMuzzleFlashEffect_1stPerson; // 0x01F0
	char        pad_0x01F4[4];             // 0x01F4
	char*        szMuzzleFlashEffect_3rdPerson; // 0x01F8
	char        pad_0x01FC[4];            // 0x01FC
	char*        szMuzzleSmokeEffect;    // 0x0200
	float        flHeatPerShot;            // 0x0204
	char*        szZoomInSound;            // 0x0208
	char*        szZoomOutSound;            // 0x020C
	float        flInaccuracyPitchShift;    // 0x0210
	float        flInaccuracySoundThreshold;    // 0x0214
	float        flBotAudibleRange;        // 0x0218
	char        pad_0x0218[8];            // 0x0220
	char*        pWrongTeamMsg;            // 0x0224
	bool        bHasBurstMode;            // 0x0228
	char        pad_0x0225[3];            // 0x0229
	bool        bIsRevolver;            // 0x022C
	bool        bCannotShootUnderwater;    // 0x0230
};
*/

class CCSWeaponData {
public:
	virtual ~CCSWeaponData() {};

	char*        consoleName;            // 0x0004
	char        pad_0008[12];            // 0x0008
	int            iMaxClip1;                // 0x0014
	int            iMaxClip2;                // 0x0018
	int            iDefaultClip1;            // 0x001C
	int            iDefaultClip2;            // 0x0020
	char        pad_0024[8];            // 0x0024
	char*        szWorldModel;            // 0x002C
	char*        szViewModel;            // 0x0030
	char*        szDroppedModel;            // 0x0034
	char        pad_0038[4];            // 0x0038
	char*        N0000023E;                // 0x003C
	char        pad_0040[56];            // 0x0040
	char*        szEmptySound;            // 0x0078
	char        pad_007C[4];            // 0x007C
	char*        szBulletType;            // 0x0080
	char        pad_0084[4];            // 0x0084
	char*        szHudName;                // 0x0088
	char*        szWeaponName;            // 0x008C
	char        pad_0090[56];            // 0x0090
	CSWeaponType   WeaponType;                // 0x00C8
	char __pad4[0x4];
	int price;
	int reward;
	char __pad5[0x14];
	uint8_t full_auto;
	char __pad6[0x3];
	int iDamage;
	float flArmorRatio;
	int bullets;
	float flPenetration;
	char __pad7[0x8];
	float flRange;
	float flRangeModifier;
	char __pad8[0x10];
	uint8_t silencer;
	char __pad9[0xF];
	float max_speed;
	float max_speed_alt;
	char __pad10[0x4C];
	int recoil_seed;
	char __pad11[0x20];
};
