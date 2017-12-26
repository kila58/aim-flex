#pragma once

class CUserCmd
{
public:
	char constructor[4];

	int command_number;
	int tick_count;
	Angle viewangles;
	Vector aimdirection;
	Vector move;
	int buttons;
	byte impulse;
	int weaponselect;
	int weaponsubtype;
	int random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
};
