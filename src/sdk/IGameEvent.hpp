#pragma once

#undef CreateEvent

#define EVENT_DEBUG_ID_INIT 42

class IGameEvent
{
public:
	virtual ~IGameEvent() {};
	virtual const char* GetName() const = 0;

	virtual bool IsReliable() const = 0;
	virtual bool IsLocal() const = 0;
	virtual bool IsEmpty(const char* keyname = NULL) = 0;

	virtual bool GetBool(const char* keyname = NULL, bool default_value = false) = 0;
	virtual int GetInt(const char* keyname = NULL, int default_value = 0) = 0;
	virtual uint64_t GetUint64(const char* keyname = NULL, uint64_t default_value = 0) = 0;
	virtual float GetFloat(const char* keyname = NULL, float default_value = 0.0f) = 0;
	virtual const char* GetString(const char* keyname = NULL, const char* default_value = "") = 0;
	virtual const wchar_t* GetWString(const char* keyname = NULL, const wchar_t* default_value = L"") = 0;
	virtual const void* GetPtr(const char* keyname = NULL, const void* default_values = NULL) = 0;

	virtual void SetBool(const char* keyname, bool value) = 0;
	virtual void SetInt(const char* keyname, int value) = 0;
	virtual void SetUint64(const char* keyname, uint64_t value) = 0;
	virtual void SetFloat(const char* keyname, float value) = 0;
	virtual void SetString(const char* keyname, const char* value) = 0;
	virtual void SetWString(const char* keyname, const wchar_t* value) = 0;
	virtual void SetPtr(const char* keyname, const void* value) = 0;
};

class IGameEventListener2 
{
public:
	virtual ~IGameEventListener2(void) {};
	virtual void FireGameEvent(IGameEvent* event) {};
	virtual int GetEventDebugID(void) { return EVENT_DEBUG_ID_INIT; }
};

class IGameEventManager2 
{
public:
	virtual ~IGameEventManager2(void) {};
	virtual int LoadEventsFromFile(const char* filename) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener(IGameEventListener2* listener, const char* name, bool serverside = false) = 0;
	virtual bool FindListener(IGameEventListener2* listener, const char* name) = 0;
	virtual void RemoveListener(IGameEventListener2* listener) = 0;
	virtual void AddListenerGlobal(IGameEventListener2* listener, bool serverside) = 0;
	virtual IGameEvent* CreateEvent(const char* name, bool force = false, int* cookie = nullptr) = 0;
	virtual bool FireEvent(IGameEvent* event, bool bDontBroadcast = false) = 0;
	virtual bool FireEventClientSide(IGameEvent* event) = 0;
	virtual IGameEvent* DuplicateEvent(IGameEvent *event) = 0;
	virtual void FreeEvent(IGameEvent *event) = 0;
	virtual bool SerializeEvent(IGameEvent *event, void* buffer) = 0;
	virtual IGameEvent* UnserializeEvent(void* buffer) = 0;
	virtual void* GetEventDataTypes(IGameEvent* event) = 0;
};

extern IGameEventManager2* eventmanager;

class EventListener : public IGameEventListener2 
{
public:
	EventListener(const char* name) 
	{
		eventmanager->AddListener(this, name, false);
	};

	~EventListener() 
	{
		eventmanager->RemoveListener(this);
	};
};

/*
class IGameEvent;
class ICVar;

extern ICVar* cvar;

class IGameEventListener2
{
public:
	virtual ~IGameEventListener2() {};

	virtual void FireGameEvent(IGameEvent* event) { cvar->ConsoleColorPrintf("hey\n"); };
	virtual int GetEventDebugID() { return 42; }

	bool AddListener(IGameEventListener2* listener, const char* name, bool serverside = false)
	{
		return getvfunc<bool(__thiscall*)(void*, IGameEventListener2*, const char*, bool)>(this, 3)(this, listener, name, serverside);
	}
	void RemoveListener(IGameEventListener2* listener)
	{
		return getvfunc<void(__thiscall*)(void*, IGameEventListener2*)>(this, 5)(this, listener);
	}
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
		return getvfunc<int(__thiscall*)(void*, const char*, int)>(this, 6)(this, key, 0);
	}
	float GetFloat(const char* key)
	{
		return getvfunc<float(__thiscall*)(void*, const char*, int)>(this, 8)(this, key, 0);
	}
	std::string GetString(const char* key)
	{
		return std::string(getvfunc<const char*(__thiscall*)(void*, const char*, int)>(this, 9)(this, key, 0));
	}
};
*/
