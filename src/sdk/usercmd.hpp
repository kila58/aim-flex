#pragma once

class CUserCmd
{
public:
	virtual ~CUserCmd();

	int		command_number;
	int		tick_count;
	Vector	viewangles; //make angle type
	Vector	aimdirection;
	Vector	move;
	int		buttons;
	byte    impulse;
	int		weaponselect;
	int		weaponsubtype;
	int		random_seed;
	short	mousedx;
	short	mousedy;
	bool	hasbeenpredicted;
};
