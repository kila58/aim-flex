#pragma once

#include "../features.hpp"

class Input : public BaseFeature
{
private:
	std::unordered_map<int, bool> keys;
public:
	Input() : BaseFeature(HookType::WINDOWPROC) {}

	void Init();
	void Invoke();
	bool KeyDown(int key);
	void Destroy();
};

extern Input input;
