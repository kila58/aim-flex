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

	void ReplaceVirtual()
	{
		DWORD original_protect;

		void* function = &table[index];
		VirtualProtect(function, sizeof(function), PAGE_READWRITE, &original_protect);

		original_function = table[index];
		table[index] = new_function;

		VirtualProtect(&table[index], sizeof(function), original_protect, NULL);
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
