#include "netvar.hpp"

#include <string>

void NetVar::Init()
{

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

void NetVar::Destroy()
{

}

NetVar netvars;
