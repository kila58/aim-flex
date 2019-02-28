#include "TraceRay.hpp"

#include "../../aim-flex.hpp"

#include "../../features/features.hpp"

using TraceRayType = void(__thiscall*)(void*, const Ray_t& ray, unsigned int mask, ITraceFilter* filter, trace_t* trace);
TraceRayType original_function;

class CAutowallFilter : public CTraceFilter
{
public:
	bool ShouldHitEntity(void* ent, unsigned int mask)
	{
		return ent != lp && ent != weapon;
	}
public:
	void* lp;
	void* weapon;
};

void __fastcall TraceRay(void* instance, void*, const Ray_t& ray, unsigned int mask, ITraceFilter* filter, trace_t* trace)
{
	CAutowallFilter newfilter;

	newfilter.lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	newfilter.weapon = ((C_BaseEntity*)(newfilter.lp))->GetWeapon();

	original_function(instance, ray, mask, &newfilter, trace);

	trace->surface.flags |= 4;
}

void TraceRayHook::Init()
{

}

void TraceRayHook::Hook()
{
	//hook = new ::Hook(enginetrace, 5, &TraceRay);
	//original_function = (TraceRayType)hook->ReplaceVirtual();
}

void TraceRayHook::Destroy()
{
	/*
	MessageBoxA(NULL, "TraceRayHook::Destroy 1\n", "TraceRayHook::Destroy 1\n", NULL);

	if (hook)
	{
		MessageBoxA(NULL, "TraceRayHook::Destroy 2\n", "TraceRayHook::Destroy 2\n", NULL);

		hook->RevertVirtual();

		delete hook;
	}
	*/
}

TraceRayHook traceray_hook;
