#include "settings.hpp"

#include "../../aim-flex.hpp"

Settings::Settings() : BaseFeature(PAINT)
{
	jsettings =
	{
		{"bhop_enabled", true},
		{"esp_enabled", true},
		{"esp_antiaiminfo", true},
		{"legit_enabled", false},
		{"rage_enabled", true},
		// todo: we should be able to fit multiple values inside an object, add to Get
		{"rage_hitchance", true},
		{"rage_hitchance_value", 100.f},
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
