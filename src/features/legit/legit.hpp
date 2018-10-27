#pragma once

#include "../features.hpp"

#include "../../angle.hpp"

#include <random>

class CUserCmd;
class C_BaseEntity;

class Legit : public BaseFeature
{
private:
	float smooth_time = 0.f;
	C_BaseEntity* last_target = nullptr;
	int last_shotsfired = 0;

	std::random_device rd;
	std::mt19937 gen;
public:
	Legit() : BaseFeature(), gen(rd()) {}

	void Init();
	bool FindTarget(CUserCmd* cmd, Angle& ang);
	void ResetTime();
	void Invoke();
	void Destroy();
};

extern Legit legit;
