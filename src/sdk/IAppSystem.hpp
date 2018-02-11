#pragma once

class IAppSystem
{
	virtual bool Connect(void* factory) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char* pInterfaceName) = 0;
	virtual int Init() = 0;
	virtual void Shutdown() = 0;
	virtual const void* GetDependencies() = 0;
	virtual int GetTier() = 0;
	virtual void Reconnect(void* factory, const char* pInterfaceName) = 0;
	virtual bool IsSingleton() = 0;
};
