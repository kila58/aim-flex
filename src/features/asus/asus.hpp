#pragma once

#include "../features.hpp"

class Asus : public BaseFeature
{
public:
	Asus() : BaseFeature(FRAMESTAGENOTIFY, 3u) {}

	void Init();
	void Invoke();
	void Destroy();

	bool first = true;
};

extern Asus asus;
