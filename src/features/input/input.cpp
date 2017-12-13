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
		keys[wparam] = true;
		break;
	case WM_KEYUP:
		keys[wparam] = false;
		break;
	}
}

bool Input::KeyDown(int key)
{
	try
	{
		if (bool ret = keys[key])
			return ret;
	}
	catch (const std::exception& e)
	{
		return false;
	}
}

void Input::Destroy()
{

}

Input input;
