#pragma once

typedef uintptr_t ptr;
typedef unsigned int uint;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;

enum HookType
{
	INVALID = -1,
	PAINT = 0,
	CREATEMOVE = 1
};

enum PaintMode_t
{
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
};

#define	LIFE_ALIVE 0
