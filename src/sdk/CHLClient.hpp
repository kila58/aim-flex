#pragma once

class CHLClient
{
public:
	ClientClass* GetAllClasses()
	{
		return getvfunc<ClientClass*(__thiscall*)(void*)>(this, 8)(this);
	}
};
