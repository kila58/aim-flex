#pragma once

#include <windows.h>

#include "definitions.hpp"

class hook
{
private:
	ptr* table;
	int index;
	DWORD original_protect;
	DWORD original_function;
	ptr* new_function;

public:
	hook::hook(ptr* vtable, int i, ptr* function)
	{
		table = vtable;
		index = i;
		new_function = function;
	}

	ptr ReplaceVirtual()
	{
		VirtualProtect(&table[index], 4, PAGE_READWRITE, &original_protect);

		original_function = (ptr)table[index];
		table[index] = (ptr)new_function;

		VirtualProtect(&table[index], 4, original_protect, NULL);

		return original_function;
	}

	void RevertVirtual()
	{
		VirtualProtect(&table[index], 4, PAGE_READWRITE, &original_protect);

		table[index] = (ptr)original_function;

		VirtualProtect(&table[index], 4, original_protect, NULL);
	}
};