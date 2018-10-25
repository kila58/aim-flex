#pragma once

#include "../features.hpp"

#include <functional>

// note: std::function uses fat memory
using FunctionType = std::function<void()>;
using AnyFunctionType = std::function<void(int)>;

struct DownOnceInfo
{
	bool set = false;
	bool reset = false;
};

class Input : public BaseFeature
{
private:
	std::unordered_map<int, bool> keys;
	std::unordered_map<int, DownOnceInfo> keys_down_once;
	std::unordered_map<int, std::deque<FunctionType>> callbacks;
	std::deque<AnyFunctionType> any_callbacks;
public:
	Input() : BaseFeature(WINDOWPROC) {}

	void Init();
	void Invoke();
	bool KeyDown(int key);
	void OnKey(int key, FunctionType func);
	bool KeyDownOnce(int key);
	void OnAnyKey(AnyFunctionType func);
	void Destroy();
};

extern Input input;
