#pragma once

#include "../features.hpp"

#include "../../json.hpp"

// 'using' in a header, sue me
using json = nlohmann::json;

class Settings : public BaseFeature
{
private:
	json jsettings;
public:
	Settings();

	void Init();
	void Invoke();
	template <typename T> T Get(const std::string& name);
	void Destroy();
};

template<typename T>
inline T Settings::Get(const std::string& name)
{
	return (T)jsettings[name];
}

extern Settings settings;
