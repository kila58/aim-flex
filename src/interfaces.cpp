#include "interfaces.hpp"

#include "aim-flex.hpp"

bool Interfaces::Init()
{
	if (!(g_EngineClient = GetInterface<CEngineClient*>("engine.dll", "VEngineClient")))
		return false;

	if (!(g_EngineVGui = GetInterface<CEngineVGui*>("engine.dll", "VEngineVGui")))
		return false;

	if (!(g_MatSystemSurface = GetInterface<CMatSystemSurface*>("vguimatsurface.dll", "VGUI_Surface")))
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

template<class T>
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
