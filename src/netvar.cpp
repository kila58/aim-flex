#include "netvar.hpp"

#include "aim-flex.hpp"

#include "hooks/netprops/m_angEyeAngles.hpp"

bool NetVar::Init()
{
	if (!HookProp("DT_CSPlayer", "m_angEyeAngles[0]", EyeAnglesPitch, old_eyeangles_pitch))
		return false;

	if (!HookProp("DT_CSPlayer", "m_angEyeAngles[1]", EyeAnglesYaw, old_eyeangles_yaw))
		return false;

	return true;
}

int NetVar::Get(const char* name, RecvTable* table)
{
	if (int offset = offsets[name])
	{
		return offset;
	}

	for (int i = 0; i < table->m_nProps; i++)
	{
		RecvProp& prop = table->m_pProps[i];
		RecvTable* child = prop.m_pDataTable;

		if (child && child->m_nProps)
		{
			int offset = Get(name, child);

			if (offset)
			{
				int final = prop.m_Offset + offset;

				offsets[name] = final;

				return final;
			}
		}

		if (!std::strcmp(prop.m_pVarName, name))
		{
			int final = prop.m_Offset;

			offsets[name] = final;

			return final;
		}
	}

	return 0;
}

RecvProp* NetVar::GetRecvProp(const char* name, RecvTable* table)
{
	for (int i = 0; i < table->m_nProps; i++)
	{
		RecvProp& prop = table->m_pProps[i];
		RecvTable* child = prop.m_pDataTable;

		if (child && child->m_nProps)
		{
			int offset = Get(name, child);

			if (offset)
			{
				return &prop;
			}
		}

		if (!std::strcmp(prop.m_pVarName, name))
		{
			return &prop;
		}
	}

	return NULL;
}

RecvTable* NetVar::GetTable(const char* tablename)
{
	RecvTable* table = nullptr;
	ClientClass* clientclass;
	
	for (clientclass = client->GetAllClasses(); clientclass; clientclass = clientclass->m_pNext)
	{
		table = clientclass->m_pRecvTable;

		if (table && std::strcmp(table->m_pNetTableName, tablename) == 0)
			return table;
	}

	return nullptr;
}

bool NetVar::HookProp(const char* tablename, const char* propname, RecvVarProxyFn fn, NetPropBackup& oldprop)
{
	RecvTable* table = GetTable(tablename);
	if (!table)
		return false;

	auto& prop = oldprop.first = GetRecvProp(propname, table);
	if (!prop)
		return false;
	
	oldprop.second = (RecvVarProxyFn)prop->m_ProxyFn;
	prop->m_ProxyFn = fn;
	
	return true;
}

void NetVar::Destroy()
{
	old_eyeangles_pitch.first->m_ProxyFn = old_eyeangles_pitch.second;
	old_eyeangles_yaw.first->m_ProxyFn = old_eyeangles_yaw.second;
}

NetVar netvars;
