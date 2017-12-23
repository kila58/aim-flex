#pragma once

#include "../features.hpp"

#include <functional>

using FunctionType = std::function<void()>;

class Input : public BaseFeature
{
private:
	std::unordered_map<int, bool> keys;
	std::unordered_map<int, std::deque<FunctionType>> callbacks;
public:
	Input() : BaseFeature(WINDOWPROC) {}

	void Init();
	void Invoke();
	bool KeyDown(int key);
	void OnKey(int key, FunctionType func);
	void Destroy();
};

extern Input input;
