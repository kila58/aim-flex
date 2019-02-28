#pragma once

#include <windows.h>

#include "definitions.hpp"

//#include "ia32/asm.h"
//#include "detours/detours.h"

#define PAGE_SIZE       4096
#define PAGE_ALIGN(Va) ((PVOID)((ULONG_PTR)(Va) & ~(PAGE_SIZE - 1)))

/*
BOOL
ForEachLogicalCore(
	void(*CallbackFunction)(void*),
	void* Context
)
{
	GROUP_AFFINITY OriginalGroupAffinity;
	if (!GetThreadGroupAffinity(GetCurrentThread(), &OriginalGroupAffinity))
	{
		return FALSE;
	}

	BOOL Result = FALSE;
	WORD GroupCount = GetActiveProcessorGroupCount();
	for (WORD GroupNumber = 0; GroupNumber < GroupCount; ++GroupNumber)
	{
		DWORD ProcessorCount = GetActiveProcessorCount(GroupNumber);

		for (DWORD ProcessorNumber = 0; ProcessorNumber < ProcessorCount; ++ProcessorNumber)
		{
			GROUP_AFFINITY GroupAffinity = { 0 };
			GroupAffinity.Mask = (KAFFINITY)(1) << ProcessorNumber;
			GroupAffinity.Group = GroupNumber;
			if (!SetThreadGroupAffinity(GetCurrentThread(), &GroupAffinity, NULL))
			{
				goto exit;
			}

			CallbackFunction(Context);
		}
	}

	Result = TRUE;

exit:
	SetThreadGroupAffinity(GetCurrentThread(), &OriginalGroupAffinity, NULL);
	return Result;
}

auto Hide = [](void* PageRead, void* PageExecute)
{
	struct CONTEXT { void* PageRead; void* PageExecute; } Context{ PageRead, PageExecute };
	ForEachLogicalCore([](void* ContextPtr) {
		CONTEXT* Context = (CONTEXT*)ContextPtr;
		ia32_asm_vmx_vmcall(0xc1, (uint64_t)Context->PageRead, (uint64_t)Context->PageExecute, 0);
	}, &Context);
};

auto Unhide = []()
{
	ForEachLogicalCore([](void*) { ia32_asm_vmx_vmcall(0xc2, 0, 0, 0); }, nullptr);
};
*/

/*
#define ushort_max (unsigned short(-1))

typedef char *vtindex;
#ifndef offset
#define offset(x,y) ((char *)(x) - (char *)(y))
#endif

class Hook
{
private:
	vtindex* vt;
	uint index;
	void* func;

	PVOID OriginalFunction;
	PVOID OriginalFunctionAligned;
	PVOID OriginalFunctionBackup;
	PVOID OriginalFunctionBackupAligned;
	PVOID HookedFunction;

	void HookFunction(void** OriginalFunction, void* HookedFunction)
	{
	32434
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(OriginalFunction, HookedFunction);
		DetourTransactionCommit();
	32434
	};
	void UnhookFunction(void** OriginalFunction, void* HookedFunction)
	{
		32434
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(OriginalFunction, HookedFunction);
		DetourTransactionCommit();
		32434
	};
public:
	Hook(void* object, uint i, void* func)
	{
		vt = *(vtindex**)object;
		index = i;
		this->func = func;
	}
	void* ReplaceVirtual()
	{
		OriginalFunction = (PVOID)vt[index];
		OriginalFunctionAligned = PAGE_ALIGN(OriginalFunction);

		OriginalFunctionBackup = malloc(PAGE_SIZE * 2);
		OriginalFunctionBackupAligned = PAGE_ALIGN((ULONG_PTR)OriginalFunctionBackup + PAGE_SIZE);
		memcpy(OriginalFunctionBackupAligned, OriginalFunctionAligned, PAGE_SIZE);

		VirtualLock(OriginalFunctionAligned, PAGE_SIZE);
		VirtualLock(OriginalFunctionBackupAligned, PAGE_SIZE);

		PVOID HookedFunction = (PVOID)&func;

		VirtualLock(OriginalFunctionAligned, PAGE_SIZE);
		VirtualLock(OriginalFunctionBackupAligned, PAGE_SIZE);

		HookFunction(&OriginalFunction, HookedFunction);
		//Hide(OriginalFunctionBackupAligned, OriginalFunctionAligned);

		//get(index) = (vtindex)func;

		//return getold(index);

		return OriginalFunction;
	}
	void RevertVirtual()
	{
		UnhookFunction(&OriginalFunction, HookedFunction);

		VirtualUnlock(OriginalFunctionAligned, PAGE_SIZE);
		VirtualUnlock(OriginalFunctionBackupAligned, PAGE_SIZE);
		free(OriginalFunctionBackup);

		//Unhide();
	}
};

*/

// meep's hook

#define ushort_max (unsigned short(-1))

typedef char *vtindex; // sizeof(pointer) with ability to add numbers and shit 
#ifndef offset
#define offset(x,y) ((char *)(x) - (char *)(y))
#endif

class Hook
{
private:
	vtindex *original_vt;
	vtindex *new_vt;
	void **hooked;
	uint index;
	void* func;
public:
	Hook(void* object, uint i, void* func) : index(i), func(func)
	{
		original_vt = *(vtindex **)object;
		vtindex *last_index = original_vt;
		while (*last_index++);

		unsigned int size = offset(last_index, original_vt) / sizeof(*last_index);

		new_vt = new vtindex[size];
		while (--last_index >= original_vt)
			new_vt[offset(last_index, original_vt) / sizeof(*last_index)] = *last_index;

		*(vtindex **)object = new_vt;

		hooked = (void **)object;
	}
	vtindex &getold(unsigned short index) { return original_vt[index]; }
	vtindex &get(unsigned short index) { return new_vt[index]; }
	void* ReplaceVirtual()
	{
		get(index) = (vtindex)func;

		return getold(index);
	}
	void RevertVirtual()
	{
		get(index) = getold(index);
	}
	~Hook()
	{
		*hooked = original_vt;
		delete[] new_vt;
	}
};
