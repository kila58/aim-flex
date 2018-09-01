#pragma once

#include "../features.hpp"

class C_BaseEntity;
class IMaterial;

class Chams : public BaseFeature
{
public:
	Chams() : BaseFeature(SCENEEND, 1u) {}

	void Init();
	bool InvalidPlayerChams(int i, C_BaseEntity* p, C_BaseEntity* lp);
	void Invoke();
	void Destroy();
public:
	IMaterial * mat;
	bool drawing = false;
};

extern Chams chams;
