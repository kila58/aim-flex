#pragma once

#include "../features.hpp"

class IMoveHelper;

class Prediction : public BaseFeature
{
private:
	float old_time;
	float old_frame;
public:
	Prediction() : BaseFeature(CREATEMOVE, 2u) {}

	void Init();
	void Invoke();
	void Destroy();

	IMoveHelper* movehelper;
	float pred_time;
};

extern Prediction predict;
