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
	template <typename T> T Get(const std::string& name);
	json& GetSettings();
	void Destroy();
};

template<typename T>
inline T Settings::Get(const std::string& name)
{
	for (auto& tabs : jsettings)
	{
		for (auto& feature : tabs)
		{
			if (feature[0] == name)
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
