#pragma once

#include "../features.hpp"

#include "../../json.hpp"
#include "../../fifo_map.hpp"

template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;

// 'using' in a header, sue me
using json = nlohmann::basic_json<my_workaround_fifo_map>;

class Settings : public BaseFeature
{
private:
	json jsettings;
public:
	Settings();

	void Init();
	void Invoke();
	//template <typename T> bool Get(json& to_search, const std::string& name, T& ret);
	json& Get(json & to_search, const std::string& name);
	template <typename T> void Set(const std::string& name, T what);
	template <typename T> T Get(const std::string& name);
	json& GetSettings();
	void Destroy();
};

extern json empty_settings_json;

inline json& Settings::Get(json& to_search, const std::string& name)
{
	for (auto& feature : to_search)
	{
		auto is_dropdown = feature[2].is_array();

		if (feature[0] == name)
		{
			return feature;
		}

		if (is_dropdown)
		{
			auto& recursive_find = Get(feature[2], name);

			if (recursive_find != empty_settings_json)
				return recursive_find;
		}
	}

	return empty_settings_json;
}

template<typename T>
inline void Settings::Set(const std::string& name, T what)
{
	for (auto& tabs : jsettings)
	{
		auto& feature = Get(tabs, name);

		if (feature != empty_settings_json)
		{
			if (feature[2].is_array())
				feature[3] = what;
			else
				feature[2] = what;
		}
	}
}

template<typename T>
inline T Settings::Get(const std::string& name)
{
	for (auto& tabs : jsettings)
	{
		auto& feature = Get(tabs, name);

		if (feature != empty_settings_json)
		{
			if (feature[2].is_array())
				return feature[3].get<T>();
			else
				return feature[2].get<T>();
		}
	}

	return T();
}

inline json& Settings::GetSettings()
{
	return jsettings;
}

extern Settings settings;
