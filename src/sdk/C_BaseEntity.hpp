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
protected:
	template <typename T>
	T Get(uint offset)
	{
		return *(T*)(this + offset);
	}
	template <typename T>
	void Set(const T& what, uint offset)
	{
		*(T*)(this + offset) = what;
	}
public:
	const model_t* GetModel()
	{
		return getvfunc<const model_t*(__thiscall*)(void*)>(this, 8)(this);
	}
	bool SetupBones(VMatrix* bones, float time, int _bones = 128, int _flag = 256)
	{
		return getvfunc<bool(__thiscall*)(void*, VMatrix*, int bones, int flag, float time)>(this, 13)(this, bones, _bones, _flag, time);
	}
	void GetRenderBounds(Vector& mins, Vector& maxs)
	{
		return getvfunc<void(__thiscall*)(void*, Vector&, Vector&)>(this, 17)(this, mins, maxs);
	}
	// todo: change this from hardcoded
	void SetBoneArrayForWrite(VMatrix* bones)
	{
		Set(bones, 0x2694);
	}
	// todo: change this from hardcoded
	VMatrix* GetBoneArrayForWrite()
	{
		return Get<VMatrix*>(0x2694);
	}
};

class C_BaseCombatWeapon;
class IClientEntityList;
class IPlayerAnimState;

extern IClientEntityList* entitylist;
extern CEngineClient* engineclient;

class C_BaseEntity
{
protected:
	int GetOffset(const char* name)
	{
		return netvars.Get(name, GetNetworkable()->GetClientClass()->m_pRecvTable);
	}
	template <typename T>
	T GetNetVar(const char* name)
	{
		return *(T*)(this + GetOffset(name));
	}
	template <typename T>
	T Get(uint offset)
	{
		return *(T*)(this + offset);
	}
	template <typename T>
	void Set(const T& what, int offset)
	{
		*(T*)(this + offset) = what;
	}
	template <typename T, typename B>
	void Set(const T& what, int offset, uint size)
	{
		std::memcpy(this + offset, what, sizeof(B) * size);
	}
	template <typename T, typename B>
	void Get(const T& what, int offset, uint size)
	{
		std::memcpy(what, this + offset, sizeof(B) * size);
	}
public:
	const unsigned long& GetRefEHandle()
	{
		return getvfunc<const unsigned long&(__thiscall*)(void*)>(this, 2)(this);
	}
	inline IClientRenderable* GetRenderable()
	{
		return (IClientRenderable*)(this + 4);
	}
	inline IClientNetworkable* GetNetworkable()
	{
		return (IClientNetworkable*)(this + 8);
	}
	Vector GetOrigin()
	{
		return GetNetVar<Vector>("m_vecOrigin");
	}
	Vector& GetAbsOrigin()
	{
		return getvfunc<Vector&(__thiscall*)(void*)>(this, 10)(this);
	}
	Angle& GetAbsAngles()
	{
		return getvfunc<Angle&(__thiscall*)(void*)>(this, 11)(this);
	}
	void GetPoseParameters(float* poses)
	{
		//auto hdr = GetModelPtr()->studio;

		Get<float*, float>(poses, GetOffset("m_flPoseParameter"), 24/*hdr->numlocalposeparameters*/);
	}
	CUtlVector<C_AnimationLayer>& GetAnimLayers()
	{
		return *Get<CUtlVector<C_AnimationLayer>*>(GetOffset("m_bSuppressAnimSounds") + 0x36);
	}
	bool IsDucked()
	{
		return GetNetVar<int>("m_fFlags") & IN_DUCK;
	}
	using SetAbsOriginType = void(__thiscall*)(C_BaseEntity*, const Vector&);
	void SetAbsOrigin(const Vector& vec)
	{
		static SetAbsOriginType SetAbsOriginFn = nullptr;
		if (!SetAbsOriginFn)
			SetAbsOriginFn = reinterpret_cast<SetAbsOriginType>(SigScan("55 8B EC 83 E4 F8 51 53 56 57 8B F1", "client.dll"));

		SetAbsOriginFn(this, vec);
	}
	using SetAbsAnglesType = void(__thiscall*)(C_BaseEntity*, const Angle&);
	void SetAbsAngles(const Angle& ang)
	{
		static SetAbsAnglesType SetAbsAnglesFn = nullptr;
		if (!SetAbsAnglesFn)
			SetAbsAnglesFn = (SetAbsAnglesType)(SigScan("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1", "client.dll"));

		SetAbsAnglesFn(this, ang);
	}
	void SetPoseParameters(float* poses)
	{
		//auto hdr = GetModelPtr()->studio;

		Set<float*, float>(poses, GetOffset("m_flPoseParameter"), 24/*hdr->numlocalposeparameters*/);
	}
	void SetAnimLayers(CUtlVector<C_AnimationLayer>& memes)
	{
		GetAnimLayers() = memes;
	}
	void StandardBlendingRules(CStudioHdr* hdr, Vector* pos, Quaternion* q, float time, int bonemask)
	{
 		getvfunc<void(__thiscall*)(void*, CStudioHdr*, Vector*, Quaternion*, float, int)>(this, 200)(this, hdr, pos, q, time, bonemask);
	}
	void BuildTransformations(CStudioHdr* hdr, Vector* pos, Quaternion* q, const VMatrix& cameratransform, int bonemask, byte* computed)
	{
		ZeroMemory(computed, 0x100);

		return getvfunc<void(__thiscall*)(void*, CStudioHdr*, Vector*, Quaternion*, const VMatrix&, int, byte*)>(this, 184)(this, hdr, pos, q, cameratransform, bonemask, computed);
	}
	CStudioHdr* GetModelPtr()
	{
		return Get<CStudioHdr*>(GetOffset("m_bSuppressAnimSounds") + 0x2);
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
	Vector GetEyeOffset()
	{
		return GetNetVar<Vector>("m_vecViewOffset[0]");
	}
	Angle GetAimPunch()
	{
		return GetNetVar<Angle>("m_aimPunchAngle");
	}
	Vector GetVelocity()
	{
		return GetNetVar<Vector>("m_vecVelocity[0]");
	}
	void SetViewAngle(const Angle& ang)
	{
		Set<Angle>(ang, GetOffset("deadflag") + 0x4);
	}
	C_BaseCombatWeapon* GetWeapon()
	{
		return (C_BaseCombatWeapon*)entitylist->GetClientEntityFromHandle(GetNetVar<unsigned long>("m_hActiveWeapon"));
	}
	float GetNextPrimaryAttack(C_BaseCombatWeapon* weapon)
	{
		return ((C_BaseEntity*)weapon)->GetNetVar<float>("m_flNextPrimaryAttack");
	}
	float GetSimulationTime()
	{
		return GetNetVar<float>("m_flSimulationTime");
	}
	// todo: change this from hardcoded
	inline ptr GetPVSFlag()
	{
		return Get<ptr>(0x270);
	}
	// todo: change this from hardcoded
	inline void SetPVSFlag(int flag)
	{
		Set<ptr>(flag, 0x270);
	}
	inline bool SetupBones(VMatrix* bones, float time = 0.f, int _bones = MAXSTUDIOBONES, int _flag = BONE_USED_BY_HITBOX)
	{
		auto old = GetPVSFlag();
		SetPVSFlag(0);

		auto ret = GetRenderable()->SetupBones(bones, time);

		SetPVSFlag(old);

		return ret;
	}
	inline const model_t* GetModel()
	{
		return GetRenderable()->GetModel();
	}
	int GetEntryIndex()
	{
		return GetRefEHandle() & 0xFFF;
	}
	// only use for debugging, slow
	std::string GetName()
	{
		player_info_t info;
		//info = *(player_info_t*)(this + GetOffset("m_szLastPlaceName") + 64 + 0x4); CPlayerInfo->GetUserID might be faster? or access m_pUserInfoTable directly in clientstate
		if (!engineclient->GetPlayerInfo(GetEntryIndex(), &info))
			return NULL;

		return info.name;
	}
	// todo: cache this
	int GetUserID()
	{
		player_info_t info;
		//info = *(player_info_t*)(this + GetOffset("m_szLastPlaceName") + 64 + 0x4); CPlayerInfo->GetUserID might be faster? or access m_pUserInfoTable directly in clientstate
		if (!engineclient->GetPlayerInfo(GetEntryIndex(), &info))
			return NULL;

		return info.userID;
	}
	float LowerBodyYaw()
	{
		return GetNetVar<float>("m_flLowerBodyYawTarget");
	}
	Angle GetEyeAngle()
	{
		return GetNetVar<Angle>("m_angEyeAngles[0]");
	}
	void SetEyeAngle(const Angle& ang)
	{
		Set(ang, GetOffset("m_angEyeAngles[0]"));
	}
};
