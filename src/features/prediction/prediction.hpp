#pragma once

#include "../features.hpp"

class IMoveHelper;

class Prediction : public BaseFeature
{
public:
	Prediction() : BaseFeature(CREATEMOVE, 2u) {}

	void Init();
	void Invoke();
	void Destroy();

	IMoveHelper* movehelper;
};

extern Prediction predict;
