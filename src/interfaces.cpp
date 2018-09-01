#include "interfaces.hpp"

#include "aim-flex.hpp"

bool Interfaces::Init()
{
	if (!(engineclient = GetInterface<CEngineClient*>("engine.dll", "VEngineClient")))
		return false;

	if (!(enginevgui = GetInterface<CEngineVGui*>("engine.dll", "VEngineVGui")))
		return false;

	if (!(matsystemsurface = GetInterface<CMatSystemSurface*>("vguimatsurface.dll", "VGUI_Surface")))
		return false;

	if (!(entitylist = GetInterface<IClientEntityList*>("client_panorama.dll", "VClientEntityList")))
		return false;

	if (!(client = GetInterface<CHLClient*>("client_panorama.dll", "VClient")))
		return false;

	if (!(cvar = GetInterface<ICVar*>("vstdlib.dll", "VEngineCvar")))
		return false;

	if (!(modelinfo = GetInterface<IVModelInfo*>("engine.dll", "VModelInfoClient")))
		return false;

	if (!(gamemovement = GetInterface<CGameMovement*>("client_panorama.dll", "GameMovement")))
		return false;

	if (!(prediction = GetInterface<IPrediction*>("client_panorama.dll", "VClientPrediction")))
		return false;

	if (!(enginetrace = GetInterface<IEngineTrace*>("engine.dll", "EngineTraceClient")))
		return false;

	if (!(eventmanager = GetInterface<IGameEventManager2*>("engine.dll", "GAMEEVENTSMANAGER002")))
		return false;

	if (!(materialsystem = GetInterface<IMaterialSystem*>("materialsystem.dll", "VMaterialSystem")))
		return false;

	if (!(renderview = GetInterface<IVRenderView*>("engine.dll", "VEngineRenderView")))
		return false;

	if (!(modelrender = GetInterface<IVModelRender*>("engine.dll", "VEngineModel")))
		return false;

	if (!(clientmode = **(ClientModeShared***)(getvfunc(client, 10) + 0x5)))
		return false;

	if (!(globals = **(CGlobalVarsBase***)(getvfunc(client, 11) + 0xA)))
		return false;

	if (!(cmdinput = (CInput*)SigScan("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10", "client_panorama.dll") + 0x1))
		return false;

	//if (!(cmdinput = *(CInput**)(getvfunc(client, 15) + 0x1)))
	//	return false;

	if (!(cl = *(CClientState**)(getvfunc(engineclient, 12) + 0x10)))
		return false;

	if (!(g_pMemAlloc = *(IMemAlloc**)GetProcAddress(GetModuleHandleA("tier0.dll"), "g_pMemAlloc")))
		return false;

	return true;
}

typedef ptr(*InstantiateInterfaceFn)();

class InterfaceReg
{
public:
	InstantiateInterfaceFn func;
	const char* name;
	InterfaceReg* next;
};

template<typename T>
T Interfaces::GetInterface(const char* module, const std::string_view& name)
{
	static ptr CreateInterface = (ptr)GetProcAddress(GetModuleHandle(module), "CreateInterface");

	ptr jmp = (CreateInterface + 0x4);

	static InterfaceReg* interfaces = **(InterfaceReg***)((*(ptr*)(jmp + 0x1) + jmp + 0x5) + 0x6);
	static InterfaceReg* cur;

	bool nonumbers = (std::find_if(name.begin(), name.end(), std::isdigit) == name.end());

	for (cur = interfaces; cur; cur = cur->next)
	{
		std::string compare(cur->name);

		if (nonumbers)
			compare.resize(compare.size() - 3);

		if (compare.compare(name) == 0)
			return (T)cur->func();
	}

	return nullptr;
}

Interfaces interfaces;
