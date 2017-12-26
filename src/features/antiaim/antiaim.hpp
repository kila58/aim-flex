#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

class AntiAim : public BaseFeature
{
private:
	Angle real;
	Angle fake;
public:
	AntiAim() {}

	void Init();
	void Invoke();
	void SetThirdPersonAngle();
	void Destroy();
};

extern AntiAim antiaim;
