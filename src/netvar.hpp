#pragma once

#include <unordered_map>
#include <utility>

#include "definitions.hpp"

struct RecvTable;
class CRecvProxyData;
struct RecvProp;

typedef void(*RecvVarProxyFn)(const CRecvProxyData *pData, void *pStruct, void *pOut);
typedef std::pair<RecvProp*, RecvVarProxyFn> NetPropBackup;

class NetVar
{
private:
	std::unordered_map<const char*, int> offsets;
public:
	bool Init();
	int Get(const char* name, RecvTable* table);
	RecvProp* GetRecvProp(const char* name, RecvTable* table);
	RecvTable* GetTable(const char* tablename);
	bool HookProp(const char* tablename, const char* propname, RecvVarProxyFn fn, NetPropBackup& oldprop);
	void Destroy();

	NetPropBackup old_eyeangles_pitch;
	NetPropBackup old_eyeangles_yaw;
};

struct RecvProp
{
	char* m_pVarName;
	int m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void* m_pExtraData;
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;
	void* m_ProxyFn;
	void* m_DataTableProxyFn;
	RecvTable* m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char* m_pParentArrayPropName;
};

class DVariant
{
public:
	union
	{
		float	m_Float;
		long	m_Int;
		char	*m_pString;
		void	*m_pData;
		float	m_Vector[3];
	};
};

class CRecvProxyData
{
public:
	const RecvProp	*m_pRecvProp;
	char			pad[4];
	DVariant		m_Value;
	int				m_iElement;
	int				m_ObjectID;
};

struct RecvTable
{
	RecvProp* m_pProps;
	int m_nProps;
	void* m_pDecoder;
	char* m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

struct ClientClass
{
	void* m_pCreateFn;
	void* m_pCreateEventFn;
	char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassID;
};

extern NetVar netvars;
