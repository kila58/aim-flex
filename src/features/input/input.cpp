#include "input.hpp"

#include "../../aim-flex.hpp"

void Input::Init()
{

}

void Input::Invoke()
{
	auto arguments = GetArguments(WINDOWPROC);
	auto msg = GetArg<UINT>(arguments, 1);
	auto wparam = GetArg<WPARAM>(arguments, 2);

	switch (msg)
	{
	case WM_KEYDOWN:
	{
		keys[wparam] = true;

		for (auto& pair : callbacks)
		{
			for (auto& func : pair.second)
			{
				if (pair.first == wparam)
					func();
			}
		}

		break;
	}
	case WM_KEYUP:
		keys[wparam] = false;
		break;
	}
}

bool Input::KeyDown(int key)
{
	if (keys.find(key) != keys.end())
	{
		return keys[key];
	}

	return false;
}

void Input::OnKey(int key, FunctionType func)
{
	callbacks[key].emplace_back(func);
}

void Input::Destroy()
{

}

Input input;
