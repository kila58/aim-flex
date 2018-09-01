#include "settings.hpp"

#include "../../aim-flex.hpp"

Settings::Settings() : BaseFeature(PAINT)
{
	//jsettings =
	//{
		// todo: we should be able to fit multiple values inside an object, add to Get
		// also this isn't ordered properly for some reason...
	/*
	{"rage", 
		{
			{"rage_enabled", true},
			{"rage_hitchance", true},
			{"rage_hitchance_value", 78.f}
		}},
		{"legit",
		{
			{"legit_enabled", false}
		}},
		{"esp",
		{
			{"esp_enabled", true},
			{"esp_antiaiminfo", true}
		}},
		{"misc",
		{
			{"bhop_enabled", true},
		}},
	};
	*/

	// important note: {"...", type} is k, v while {a, b, c, ...} is_array() == true

	jsettings["rage"] = 
	{
		{"rage_enabled", "enabled", true},
		{"rage_hitchance", "hit chance", true},
		{"rage_hitchance_value", "hit chance value", 78, 0, 100},
		{"rage_antiaim", "anti aim", 
		{
			{"static", "static", nullptr},
			{"backwards", "backwards", nullptr},
			{"spin", "spin", nullptr}
		}, "static"}
	};

	jsettings["legit"] =
	{
		{"legit_enabled", "enabled", false}
	};

	jsettings["esp"] =
	{
		{"esp_enabled", "enabled", true},
		{"esp_antiaiminfo", "anti-aim info", true},
		{"esp_bighuge", "big huge", true},
		{"esp_absolute_unit", "absolute unit", true}
	};

	jsettings["misc"] =
	{
		{"bhop_enabled", "bhop enabled", true},
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
