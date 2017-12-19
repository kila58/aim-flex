#pragma once

#include "../features.hpp"

class Legit : public BaseFeature
{
public:
	Legit() : BaseFeature() {}

	void Init();
	void Invoke();
	void Destroy();
};

extern Legit legit;
