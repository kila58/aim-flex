#pragma once

class IClientNetworkable
{
public:
	virtual void*			 GetIClientUnknown() = 0;
	virtual void             Release() = 0;
	virtual ClientClass*     GetClientClass() = 0;
	virtual void             NotifyShouldTransmit(int state) = 0;
	virtual void             OnPreDataChanged(int updateType) = 0;
	virtual void             OnDataChanged(int updateType) = 0;
	virtual void             PreDataUpdate(int updateType) = 0;
	virtual void             PostDataUpdate(int updateType) = 0;
	virtual void             __unkn(void) = 0;
	virtual bool             IsDormant(void) = 0;
	virtual int              EntIndex(void) const = 0;
	virtual void             ReceiveMessage(int classID, void* msg) = 0;
	virtual void*            GetDataTableBasePtr() = 0;
	virtual void			 SetDestroyedOnRecreateEntities(void) = 0;
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
		Set(bones, 0x26A4);
	}
	// todo: change this from hardcoded
	VMatrix* GetBoneArrayForWrite()
	{
		return Get<VMatrix*>(0x26A4);
	}
}; 

class C_BaseCombatWeapon;
class IClientEntityList;
class IPlayerAnimState;

extern IClientEntityList* entitylist;
extern CEngineClient* engineclient;
extern CGlobalVarsBase* globals;

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
	void SetNetVar(const char* name, const T& what)
	{
		*(T*)(this + GetOffset(name)) = what;
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
		// I think this is just (unsigned long)&this?
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
		static Vector empty_vec(0, 0, 0);

		if (!this)
		{
			MessageBoxA(NULL, "fokin naughty", "mate", NULL);

			return empty_vec;
		}

		static auto getabsorigin = getvfunc(this, 10);
		static auto getabsorigin_func = (Vector&(__thiscall*)(void*))getabsorigin;

		return getabsorigin_func(this);

		//auto lol = (Vector&(__thiscall*)(void*))(getabsorigin)(this);

		//return getvfunc<Vector&(__thiscall*)(void*)>(this, 10)(this);
	}
	Angle& GetAbsAngles()
	{
		return getvfunc<Angle&(__thiscall*)(void*)>(this, 11)(this);
	}
	PoseArray GetPoseParameters()
	{
		return GetNetVar<PoseArray>("m_flPoseParameter");
	}
	AnimLayerVec GetAnimLayers()
	{
		static auto m_bSuppressAnimSounds = GetOffset("m_bSuppressAnimSounds");

		return Get<AnimLayerVec>(m_bSuppressAnimSounds + 0x36);
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
			SetAbsOriginFn = reinterpret_cast<SetAbsOriginType>(SigScan("55 8B EC 83 E4 F8 51 53 56 57 8B F1", "client_panorama.dll"));

		SetAbsOriginFn(this, vec);
	}
	using SetAbsAnglesType = void(__thiscall*)(C_BaseEntity*, const Angle&);
	void SetAbsAngles(const Angle& ang)
	{
		static SetAbsAnglesType SetAbsAnglesFn = nullptr;
		if (!SetAbsAnglesFn)
			SetAbsAnglesFn = (SetAbsAnglesType)(SigScan("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1", "client_panorama.dll"));

		SetAbsAnglesFn(this, ang);
	}
	CCSGOPlayerAnimState* GetAnimationState()
	{
		static auto m_bIsScoped = GetOffset("m_bIsScoped");

		return Get<CCSGOPlayerAnimState*>(m_bIsScoped - 0xA);
	}
	void UpdateClientSideAnimation()
	{
		return getvfunc<void(__thiscall*)(void*)>(this, 218)(this);
	}
	void SetClientSideAnimation(bool what)
	{
		static auto m_bClientSideAnimation = GetOffset("m_bClientSideAnimation");

		Set<bool>(what, m_bClientSideAnimation);
	}
	void SetPoseParameters(PoseArray poses)
	{
		SetNetVar<PoseArray>("m_flPoseParameter", poses);
	}
	void SetAnimLayers(AnimLayerVec& memes)
	{
		GetAnimLayers() = memes;
	}
	void StandardBlendingRules(CStudioHdr* hdr, Vector* pos, Quaternion* q, float time, int bonemask)
	{
 		getvfunc<void(__thiscall*)(void*, CStudioHdr*, Vector*, Quaternion*, float, int)>(this, 201)(this, hdr, pos, q, time, bonemask);
	}
	void BuildTransformations(CStudioHdr* hdr, Vector* pos, Quaternion* q, const VMatrix& cameratransform, int bonemask, byte* computed)
	{
		return getvfunc<void(__thiscall*)(void*, CStudioHdr*, Vector*, Quaternion*, const VMatrix&, int, byte*)>(this, 185)(this, hdr, pos, q, cameratransform, bonemask, computed);
	}
	CStudioHdr* GetModelPtr()
	{
		static auto m_bSuppressAnimSounds = GetOffset("m_bSuppressAnimSounds");

		return Get<CStudioHdr*>(m_bSuppressAnimSounds + 0x2);
	}
	// this is being goofy for some reason
	bool IsDormant()
	{
		return Get<bool>(0xED);

		/*
		auto networkable = GetNetworkable();
	
		if (!networkable)
		{
			MessageBoxA(NULL, "!networkable", "!networkable", NULL);

			return true;
		}

		return GetNetworkable()->IsDormant();
		*/
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
	int GetArmor()
	{
		return GetNetVar<int>("m_iArmor");
	}
	bool IsEnemy(C_BaseEntity* g_LocalPlayer)
	{
		/*
		if (InDangerzone())
		{
			return this->m_nSurvivalTeam() != g_LocalPlayer->m_nSurvivalTeam() || g_LocalPlayer->m_nSurvivalTeam() == -1;
		}
		else
		{
		*/
			return GetTeam() != g_LocalPlayer->GetTeam();
		//}
	}
	bool HasHelmet()
	{
		return GetNetVar<bool>("m_bHasHelmet");
	}
	bool IsOnGround()
	{
		return GetNetVar<int>("m_fFlags") & FL_ONGROUND;
	}
	Vector OBBMins()
	{
		return GetNetVar<Vector>("m_vecMins");
	}
	Vector OBBMaxs()
	{
		return GetNetVar<Vector>("m_vecMaxs");
	}
	int GetTickBase()
	{
		return GetNetVar<int>("m_nTickBase");
	}
	//Vector GetEyeOffset()
	//{
	//	return GetNetVar<Vector>("m_vecViewOffset[0]");
	//}
	Vector GetEyePos()
	{
		Vector ret;
		getvfunc<void(__thiscall*)(void*, Vector&)>(this, 279)(this, ret);

		return ret;
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
		static auto deadflag = GetOffset("deadflag");

		Set<Angle>(ang, deadflag + 0x4);
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
	float GetOldSimulationTime()
	{
		static auto m_flSimulationTime = GetOffset("m_flSimulationTime");

		return Get<float>(m_flSimulationTime + 0x4);
	}
	// todo: change this from hardcoded
	void ClearOcclusionFlags()
	{
		Set<int>(globals->framecount, 0xA30);
		Set<int>(0, 0xA28);
	}
	// todo: change this from hardcoded
	float GetSpawnTime()
	{
		return Get<float>(0xA290);
	}
	inline bool SetupBones(VMatrix* bones, float time = 0.f, int _bones = MAXSTUDIOBONES, int _flag = BONE_USED_BY_HITBOX)
	{
		auto renderable = GetRenderable();
		if (!renderable)
			return false;

		return renderable->SetupBones(bones, time);
	}
	inline const model_t* GetModel()
	{
		auto renderable = GetRenderable();
		if (!renderable)
			return nullptr;

		return renderable->GetModel();
	}
	inline void DrawModel()
	{
		auto renderable = GetRenderable();
		if (!renderable)
			return;

		return getvfunc<void(__thiscall*)(void*, int, void*)>(renderable, 9)(renderable, 1, 0);
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
		static auto m_angEyeAngles = GetOffset("m_angEyeAngles[0]");

		Set(ang, m_angEyeAngles);
	}
	float GetStepSize()
	{
		return GetNetVar<float>("m_flStepSize");
	}
	float GetMaxSpeed()
	{
		return GetNetVar<float>("m_flMaxSpeed");
	}
	int GetShotsFired()
	{
		return GetNetVar<int>("m_iShotsFired");
	}
};
