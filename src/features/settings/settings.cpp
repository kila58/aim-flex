#include "settings.hpp"

#include "../../aim-flex.hpp"

Settings::Settings() : BaseFeature(HookType::PAINT)
{
	jsettings =
	{
		{"bhop_enabled", true},
		{"esp_enabled", true},
		{"legit_enabled", false},
		{"rage_enabled", true}
	};
}

void Settings::Init()
{

}

void Settings::Invoke()
{

}

void Settings::Destroy()
{

}

Settings settings;