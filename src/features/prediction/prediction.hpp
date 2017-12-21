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
	void End();
	void Destroy();

	IMoveHelper* movehelper;
};

extern Prediction predict;
