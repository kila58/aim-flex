#pragma once

#include "../features.hpp"

#include "../../aim-flex.hpp"

class ESP : public BaseFeature
{
private:
	int x, y, w, h;
	C_BaseEntity* lp;
	C_BaseEntity* p;
	int font, font2;
	player_info_t info;
public:
	ESP() : BaseFeature(PAINT) {}

	void Init();
	void Invoke();
	bool InvalidPlayerESP(int i, C_BaseEntity* p, C_BaseEntity* lp);
	bool CreateBox(C_BaseEntity* p, int& x, int& y, int& w, int& h);
	void PlayerBox();
	void PlayerHealthBar();
	void PlayerText();
	void PlayerWeapon();
	void PlayerBacktrack();
	void Destroy();
};

extern ESP esp;
