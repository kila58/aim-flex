#pragma once

class IGameEvent;

class IGameEventListener2
{
public:
	virtual    ~IGameEventListener2(void) {};

	virtual void FireGameEvent(IGameEvent *event) = 0;
	virtual int  GetEventDebugID(void) = 0;
};

class IGameEvent
{
public:
	std::string GetName()
	{
		return std::string(getvfunc<const char*(__thiscall *)(void*)>(this, 1)(this));
	}
	int GetInt(const char* key)
	{
		return getvfunc<int(__thiscall *)(void*, const char*, int)>(this, 6)(this, key, 0);
	}
	std::string GetString(const char* key)
	{
		return std::string(getvfunc<const char*(__thiscall *)(void*, const char*, int)>(this, 9)(this, key, 0));
	}
};
