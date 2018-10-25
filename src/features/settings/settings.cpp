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

	{"legit_speed_start", "smooth speed start", 0, 0, 100, "%"}, // a == L"\u2103"
	{"legit_speed", "smooth speed value", 1, 0, 50, ""}, // a == L"\u2103"
	{"legit_speed_max", "smooth speed max", 100, 0, 100, "%"}, // a == L"\u2103"
	*/

	// important note: {"...", type} is k, v while {a, b, c, ...} is_array() == true

	jsettings["rage"] =
	{
		{"rage_enabled", "enabled", false},
		{"rage_silent_enabled", "silent aim", true},
		{"rage_multipoint_enabled", "multi point", false},
		{"rage_hitchance", "hit chance",
		{
			{"rage_hitchance_enabled", "hit chance enabled", true},
			{"rage_hitchance_value", "hit chance value", 75, 0, 100, "%"},
		}},
		{"rage_antiaim", "anti aim",
		{
			{"rage_antiaim_enabled", "anti aim enabled", false},
			{"rage_antiaim_pitch", "anti aim pitch",
			{
				{"off", "off", nullptr},
				{"full down", "full down", nullptr},
				{"half down", "half down", nullptr},
				{"zero", "zero", nullptr}
			}, "off"},
			{"rage_antiaim_real", "anti aim real",
			{
				{"off", "off", nullptr},
				{"static", "static", nullptr},
				{"backwards", "backwards", nullptr},
			}, "off"},
			{"rage_antiaim_fake", "anti aim fake",
			{
				{"off", "off", nullptr},
				{"static", "static", nullptr},
				{"backwards", "backwards", nullptr},
			}, "off"},
		}},
	};

	jsettings["legit"] =
	{
		{"legit_enabled", "enabled", false},
		{"legit_team_enabled", "aim at teammates", false},
		{"legit_aimkey", "aim key", VK_XBUTTON2}, // mouse 5
		{"legit_fov", "fov", 5, 0, 180, " deg"}, // a == L"\u2103"
		{"legit_weapons", "weapons", 
		{
			{"legit_weapons_enabled", "enabled", false},
			{"legit_rifle_speed", "rifle smooth speed", 1, 1, 50, ""},
			{"legit_pistol_speed", "pistol smooth speed", 1, 1, 50, ""},
			{"legit_sniper_speed", "sniper smooth speed", 1, 1, 50, ""},
			{"legit_smg_speed", "smg smooth speed", 1, 1, 50, ""},
			{"legit_shotgun_speed", "shotgun smooth speed", 1, 1, 50, ""},
			{"legit_heavy_speed", "heavy smooth speed", 1, 1, 50, ""},
		}}
	};

	jsettings["esp"] =
	{
		{"esp_enabled", "enabled", true},
		{"esp_toggle_key", "toggle esp key", 0x0}
	};

	jsettings["visuals"] = 
	{
		{"visuals_enabled", "enabled", true},
		{"visuals_chams_enabled", "chams", true}
	};

	jsettings["misc"] =
	{
		{"bhop_enabled", "bhop enabled", true},
		{"bhop_autostrafer", "auto strafer", false},
		{"a", "a", true}
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

json empty_settings_json;

Settings settings;
