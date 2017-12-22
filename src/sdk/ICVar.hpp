#pragma once

class ConVar;

class ICVar
{
public:
	ConVar* FindVar(const char* var)
	{
		return getvfunc<ConVar*(__thiscall*)(void*, const char*)>(this, 16)(this, var);
	}
	template<typename... Args>
	void ConsoleColorPrintf(const std::string& str, const Args&... args)
	{
		const Color white(255, 255, 255);

		getvfunc<void(__cdecl*)(void*, const Color&, const char*, ...)>(this, 25)(this, white, str.c_str(), args...);
	}
};

#define FCVAR_NONE				0 

// Command to ConVars and ConCommands
// ConVar Systems
#define FCVAR_UNREGISTERED		(1<<0)	// If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY	(1<<1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL			(1<<2)	// defined by the game DLL
#define FCVAR_CLIENTDLL			(1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN			(1<<4)	// Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only
#define FCVAR_PROTECTED			(1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY			(1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define	FCVAR_ARCHIVE			(1<<7)	// set to cause it to be saved to vars.rc
#define	FCVAR_NOTIFY			(1<<8)	// notifies players when changed
#define	FCVAR_USERINFO			(1<<9)	// changes the client's info string
#define FCVAR_CHEAT				(1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats

#define FCVAR_PRINTABLEONLY		(1<<10)  // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED			(1<<11)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING	(1<<12)  // never try to print that cvar

// It's a ConVar that's shared between the client and the server.
// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
//  client, of course )
// If a change is requested it must come from the console (i.e., no remote client changes)
// If a value is changed while a server is active, it's replicated to all connected clients
#define FCVAR_REPLICATED		(1<<13)	// server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_DEMO				(1<<16)  // record this cvar when starting a demo file
#define FCVAR_DONTRECORD		(1<<17)  // don't record these command in demofiles
#define FCVAR_RELOAD_MATERIALS	(1<<20)	// If this cvar changes, it forces a material reload
#define FCVAR_RELOAD_TEXTURES	(1<<21)	// If this cvar changes, if forces a texture reload

#define FCVAR_NOT_CONNECTED		(1<<22)	// cvar cannot be changed by a client that is connected to a server
#define FCVAR_MATERIAL_SYSTEM_THREAD (1<<23)	// Indicates this cvar is read from the material system thread
#define FCVAR_ARCHIVE_XBOX		(1<<24) // cvar written to config.cfg on the Xbox

#define FCVAR_ACCESSIBLE_FROM_THREADS	(1<<25)	// used as a debugging tool necessary to check material system thread convars

#define FCVAR_SERVER_CAN_EXECUTE	(1<<28)// the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_SERVER_CANNOT_QUERY	(1<<29)// If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_CLIENTCMD_CAN_EXECUTE	(1<<30)	// IVEngineClient::ClientCmd is allowed to execute this command. 
// Note: IVEngineClient::ClientCmd_Unrestricted can run any client command.

#define FCVAR_MATERIAL_THREAD_MASK ( FCVAR_RELOAD_MATERIALS | FCVAR_RELOAD_TEXTURES | FCVAR_MATERIAL_SYSTEM_THREAD )	

class ConVar;
class IConVar;
class CCommand;
class ConCommand;
class ConCommandBase;

class IConVar
{
public:
	// Value set
	virtual void SetValue(const char *pValue) = 0;
	virtual void SetValue(float flValue) = 0;
	virtual void SetValue(int nValue) = 0;

	// Return name of command
	virtual const char *GetName(void)const = 0;

	// Accessors.. not as efficient as using GetState()/GetInfo()
	// if you call these methods multiple times on the same IConVar
	virtual bool IsFlagSet(int nFlag) const = 0;
};

typedef void(*FnChangeCallback_t)(IConVar *var, const char *pOldValue, float flOldValue);

class IConCommandBaseAccessor
{
public:
	virtual bool RegisterConCommandBase(ConCommandBase *pVar) = 0;
};

class ConCommandBase
{
public:
	virtual						~ConCommandBase(void) = 0;
	virtual	bool				IsCommand(void) = 0;
	virtual bool					IsFlagSet(int flag) = 0;
	virtual void					AddFlags(int flags) = 0;
	virtual void					RemoveFlags(int flags) = 0;
	virtual int					GetFlags() = 0;
	virtual const char*			GetName(void) = 0;
	virtual const char*			GetHelpText(void) = 0;
	virtual bool					IsRegistered(void) = 0;
	virtual int					GetDLLIdentifier() = 0;
	virtual void					Create(const char *pName, const char *pHelpString = 0, int flags = 0) = 0;
	virtual void					Init() = 0;

public:
	ConCommandBase * m_pNext;
	bool							m_bRegistered;
	const char*						m_pszName;
	const char*						m_pszHelpString;
	int								m_nFlags;

public:
	static ConCommandBase*			s_pConCommandBases;
	static IConCommandBaseAccessor*	s_pAccessor;
};

class ConVar : public ConCommandBase, public IConVar
{
public:
	typedef ConCommandBase BaseClass;

	virtual							~ConVar(void) = 0;

	virtual bool					IsFlagSet(int flag) = 0;
	virtual const char*				GetHelpText(void) = 0;
	virtual bool					IsRegistered(void) = 0;
	virtual const char*				GetName(void) = 0;
	virtual const char*				GetBaseName(void) = 0;
	virtual int						GetSplitScreenPlayerSlot() = 0;
	virtual void					AddFlags(int flags) = 0;
	virtual int						GetFlags() = 0;
	virtual	bool					IsCommand(void) = 0;
	virtual float                   GetFloat(void) = 0;
	virtual int						GetInt(void) = 0;
	virtual void					SetValue(const char *value) = 0;
	virtual void					SetValue(float value) = 0;
	virtual void					SetValue(int value) = 0;
	virtual void					SetValue(Color value) = 0;

	struct CVValue_t
	{
		char*	m_pszString;
		int		m_StringLength;
		float	m_fValue;
		int		m_nValue;
	};

	virtual void					InternalSetValue(const char *value) = 0;
	virtual void					InternalSetFloatValue(float fNewValue) = 0;
	virtual void					InternalSetIntValue(int nValue) = 0;
	virtual void					InternalSetColorValue(Color value) = 0;
	virtual bool					ClampValue(float& value) = 0;
	virtual void					ChangeStringValue(const char *tempVal, float flOldValue) = 0;
	virtual void					Create(const char *pName, const char *pDefaultValue, int flags = 0,
		const char *pHelpString = 0, bool bMin = false, float fMin = 0.0,
		bool bMax = false, float fMax = false, FnChangeCallback_t callback = 0) = 0;
	virtual void					Init() = 0;

	ConVar*					m_pParent;
	const char*				m_pszDefaultValue;
	CVValue_t				m_Value;
	bool					m_bHasMin;
	float					m_fMinVal;
	bool					m_bHasMax;
	float					m_fMaxVal;
	FnChangeCallback_t		m_fnChangeCallback;

	float xor_value_float(void) const
	{
		auto temp = *(int*)(&m_Value.m_fValue);
		auto temp_result = (int)(temp ^ (DWORD)this);

		return *(float*)(&temp_result);
	}

	int xor_value_int(void) const
	{
		auto temp = *(int*)(&m_Value.m_nValue);
		auto temp_result = (int)(temp ^ (DWORD)this);

		return *(int*)(&temp_result);
	}
};
