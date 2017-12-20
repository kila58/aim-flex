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

	if (!(entitylist = GetInterface<IClientEntityList*>("client.dll", "VClientEntityList")))
		return false;

	if (!(client = GetInterface<CHLClient*>("client.dll", "VClient")))
		return false;

	if (!(cvar = GetInterface<ICVar*>("vstdlib.dll", "VEngineCvar")))
		return false;

	if (!(modelinfo = GetInterface<IVModelInfo*>("engine.dll", "VModelInfoClient")))
		return false;

	if (!(gamemovement = GetInterface<CGameMovement*>("client.dll", "GameMovement")))
		return false;

	if (!(prediction = GetInterface<IPrediction*>("client.dll", "VClientPrediction")))
		return false;

	if (!(enginetrace = GetInterface<IEngineTrace*>("engine.dll", "EngineTraceClient")))
		return false;

	if (!(clientmode = **(ClientModeShared***)(getvfunc(client, 10) + 5)))
		return false;

	if (!(globals = **(CGlobalVarsBase***)(getvfunc(client, 11) + 10)))
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
T Interfaces::GetInterface(const char* module, const std::string& name)
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
