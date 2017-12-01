#pragma once

#include <windows.h>

#include "definitions.hpp"

class Hook
{
private:
	void** table;
	uint index;
	void* new_function;
	void* original_function;
public:
	Hook(void* object, uint i, void* function) : index(i), table(*(void***)object), new_function(function) {}

<<<<<<< HEAD
	void* ReplaceVirtual()
=======
	void ReplaceVirtual()
>>>>>>> 1ca4338319b59e3df8f6fdd429fca15760e6e890
	{
		DWORD original_protect;

		void* function = &table[index];
		VirtualProtect(function, sizeof(function), PAGE_READWRITE, &original_protect);

		original_function = table[index];
		table[index] = new_function;

		VirtualProtect(&table[index], sizeof(function), original_protect, NULL);
<<<<<<< HEAD

		return original_function;
=======
>>>>>>> 1ca4338319b59e3df8f6fdd429fca15760e6e890
	}
	void RevertVirtual()
	{
		DWORD original_protect;

		void* function = &table[index];
		VirtualProtect(function, sizeof(function), PAGE_READWRITE, &original_protect);

		table[index] = original_function;

		VirtualProtect(&table[index], sizeof(function), original_protect, NULL);
	}
};
