#pragma once

struct player_info_t
{
	unsigned __int64 pad001;
	unsigned __int64 xuid;
	char name[128];
	int userID;
	char guid[33];
	unsigned __int32 friendsID;
	char friendsName[128];
	bool fakeplayer;
	bool ishltv;
	unsigned long customfiles[4];
	unsigned char filesDownloaded;
};

class CEngineClient
{
public:
	void GetScreenSize(int* w, int* h)
	{
		return getvfunc<void(__thiscall*)(void*, int*, int*)>(this, 5)(this, w, h);
	}
	int GetLocalPlayer()
	{
		return getvfunc<int(__thiscall*)(void*)>(this, 12)(this);
	}
	bool GetPlayerInfo(int index, player_info_t* info)
	{
		return getvfunc<bool(__thiscall*)(void*, int, player_info_t*)>(this, 8)(this, index, info);
	}
	bool IsInGame()
	{
		return getvfunc<bool(__thiscall*)(void*)>(this, 26)(this);
	}
	VMatrix& WorldToScreenMatrix()
	{
		return getvfunc<VMatrix&(__thiscall*)(void*)>(this, 37)(this);
	}
};
