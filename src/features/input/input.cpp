#include "input.hpp"

#include "../../aim-flex.hpp"

void Input::Init()
{

}

void Input::Invoke()
{
	auto arguments = GetArguments(WINDOWPROC);
	auto handle = GetArg<HWND>(arguments, 0);
	auto msg = GetArg<UINT>(arguments, 1);
	auto wparam = GetArg<WPARAM>(arguments, 2);

	if (msg == WM_KILLFOCUS)
	{
		for (auto& key_pair : keys)
		{
			key_pair.second = false;
		}

		return;
	}

	if (handle != GetFocus())
		return;

	if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONUP)
	{
		if (GET_XBUTTON_WPARAM(wparam) == 1)
			wparam = VK_XBUTTON1;
		else if (GET_XBUTTON_WPARAM(wparam) == 2)
			wparam = VK_XBUTTON2;
	}

	if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP)
		wparam = VK_LBUTTON;
	else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP)
		wparam = VK_RBUTTON;

	switch (msg)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		if (handle == GetFocus())
		{
			keys[wparam] = true;
			keys_down_once[wparam].set = true;

			for (auto& pair : callbacks)
			{
				if (pair.first == wparam)
				{
					for (auto& func : pair.second)
					{
						func();
					}
				}
			}

			for (auto& any_callback : any_callbacks)
			{
				any_callback(wparam);
			}

			break;
		}
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{		
		keys[wparam] = false;
		keys_down_once[wparam].set = false;
		keys_down_once[wparam].reset = false;
		break;
	}
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

// not "up" necessarily btw
// also idk wtf I even wrote todo: rewrite this maybe or review it
bool Input::KeyDownOnce(int key)
{
	if (keys_down_once.find(key) != keys_down_once.end())
	{
		if (keys_down_once[key].reset)
		{
			keys_down_once[key].set = false;
		}

		if (keys_down_once[key].set)
		{
			keys_down_once[key].reset = true;
		}

		return keys_down_once[key].set;
	}

	return false;
}

void Input::OnAnyKey(AnyFunctionType func)
{
	any_callbacks.emplace_back(func);
}

void Input::Destroy()
{

}

Input input;
