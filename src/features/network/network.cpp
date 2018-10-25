#include "network.hpp"

#include "../../aim-flex.hpp"

#include "../settings/settings.hpp"

#include <future>

void dostuff()
{
	network.DoStuff();
}

bool FindSetting2(json& tab, json& new_settings, const std::string& key, json& new_setting, json& current_setting)
{
	for (auto it2 : tab.items())
	{
		current_setting = it2.value();
		std::string current_setting_name = current_setting[0];

		auto& settings_in_tab = new_settings[key];

		for (auto& single_setting : settings_in_tab)
		{
			//debug << single_setting << "\n";

			if (current_setting_name == single_setting[0])
			{
				new_setting = single_setting;

				return true;
			}

			if (single_setting[2].is_array())
			{
				//debug << "recursive\n";

				if (FindSetting2(single_setting[2], new_settings, key, new_setting, current_setting))
					return true;
			}
		}
	}
	
	return false;
}

json empty_json;

void FindAndSetSetting(const std::string& key, json& to_be_replaced, json& settings)
{
	for (auto it2 : settings.items())
	{
		auto& new_setting = it2.value();

		// ignore dropdown_option
		if (!new_setting[2].is_null())
		{
			if (new_setting[2].is_array() && new_setting[3].is_null())
			{
				FindAndSetSetting(key, to_be_replaced, new_setting[2]);
			}
			else
			{
				const std::string& new_setting_name = new_setting[0];

				if (new_setting_name == key)
				{
					//debug << (to_be_replaced[2].is_array()) << "\n";
					//debug << to_be_replaced << "\n";

					if (to_be_replaced[2].is_array())
						to_be_replaced[3] = new_setting[3];
					else
						to_be_replaced[2] = new_setting[2];
				}
			}
		}
	}
}

bool SetSettings(json& value, json& new_settings, bool recursive = false)
{
	for (auto it2 : value.items())
	{
		auto& current_setting = it2.value();

		// ignore dropdown_option
		if (!current_setting[2].is_null())
		{
			if (current_setting[2].is_array() && current_setting[3].is_null())
			{	
				const std::string& current_setting_name = current_setting[0];

				//debug << "current_setting_name: " << current_setting_name << "\n";

				SetSettings(current_setting[2], new_settings, true);
			}
			else
			{
				const std::string& current_setting_name = current_setting[0];

				if (recursive)
				{
					FindAndSetSetting(current_setting_name, current_setting, new_settings);
				}
				else
				{
					// todo: why is there duplicates
					for (auto& single_setting : new_settings)
					{
						if (single_setting[0].is_string())
						{
							const std::string& single_setting_name = single_setting[0];

							if (single_setting_name == current_setting_name)
							{
								if (current_setting[2].is_array())
								{
									//debug << current_setting << "\n";

									current_setting[3] = single_setting[3];
								}
								else
									current_setting[2] = single_setting[2];
							}

							//debug << "single_setting_name: " << single_setting_name << "\n";
						}
					}
				}
			}
		}
	}

	return true;
}

void Network::Init()
{
	thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)dostuff, NULL, NULL, NULL);

	network.OnSync("get_settings_response", [&](json& message)
	{
		auto recursion_test_enabled = settings.Get<int>("recursion_test_enabled");

		bool is_set = message[0];
		
		// this POS needs to be cleaned up so I use settings.get instead
		if (is_set)
		{
			json& current_settings = settings.GetSettings();
			json& new_settings = message[1];

			for (auto& tab : current_settings.items())
			{
				SetSettings(tab.value(), new_settings[tab.key()]);
			}

			//FindSetting(current_settings, new_settings);

			//for (auto it : current_settings.items())
			//{
			//	auto& tab = it.value();

				//debug << tab << "\n";

				//json new_setting;
				//json current_setting;

				/*
				if (FindSetting(tab, new_settings, it.key(), new_setting, current_setting))
				{
					if (current_setting[2].is_array())
						current_setting[3] = new_setting[3];
					else
						current_setting[2] = new_setting[2];
				}
				else
					debug << "couldn't find\n";
				*/
			//}

			//debug << "\n";
			//debug << current_settings << "\n";
		}
	});

	//std::thread thread([this]
	//{
		//enetpp::global_state::get().initialize();

		/*
		trace_handler = [&](const std::string& msg)
		{
			std::lock_guard<std::mutex> lock(mutex);

			std::cout << msg << "\n";
		};

		client.set_trace_handler(trace_handler);
		*/

		//network.Connect("localhost", port);
		//network.Send("login", json{ jwt_token });

		//Invoke();
	//});
}

void Network::DoStuff()
{
	//MessageBoxA(NULL, "do stuff", "do stuff", NULL);

	enetpp::global_state::get().initialize();

	network.trace_handler = [&](const std::string& msg)
	{
		std::lock_guard<std::mutex> lock(network.mutex);

		//debug << msg << "\n";

		//std::cout << msg << "\n";
	};

	network.client.set_trace_handler(network.trace_handler);

	std::string str = network.jwt_token;
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\0'), str.end());

	network.Connect("198.100.146.95", port);
	network.Send("login", json{ channel_cheat, str });
	network.Send("get_settings", json());

	network.Invoke();

	// invoke is finished which means we disconnected, time to clean up
	std::terminate();
}

void Network::Connect(const std::string& hostname, enet_uint16 port)
{
	client.connect(enetpp::client_connect_params()
		.set_channel_count(1)
		.set_server_host_name_and_port(hostname.c_str(), port));
}

void Network::Disconnect()
{
	connected = false;

	client.disconnect();
}

void Network::Send(const std::string& to, const json& message)
{
	queue.emplace_back(json{ to, message });
}

void Network::Send(const std::string& to, const std::string& message)
{
	queue.emplace_back(json{ to, message });
}

void Network::Consume()
{
	static auto on_connected = [this]()
	{
		trace_handler("on_connected");

		connected = true;
	};

	static auto on_disconnected = [this]()
	{
		trace_handler("on_disconnected");
	};

	static auto on_data_received = [this](const enet_uint8* data, size_t data_size)
	{
		//MessageBoxA(NULL, "on_data_received", "on_data_received", NULL);

		try
		{
			json j_from_cbor = json::from_cbor(data, data_size);
			//MessageBoxA(NULL, "made it after from_cbor", "made it after from_cbor", NULL);

			std::string callback_name = j_from_cbor[0];
			//MessageBoxA(NULL, callback_name.c_str(), callback_name.c_str(), NULL);

			json message = j_from_cbor[1];

			for (auto& callback : callbacks)
			{
				if (callback.name == callback_name)
					callback.callback(message);
			}
		}
		catch (std::exception& e)
		{
			MessageBoxA(NULL, e.what(), e.what(), NULL);
		}
	};

	client.consume_events(on_connected, on_disconnected, on_data_received);
}

void Network::On(const std::string& name, std::function<void(json&)> function)
{
	callbacks.emplace_back(name, function);
}

// on sync will wait til it receives some sort of response (blocking)
void Network::OnSync(const std::string& name, std::function<void(json&)> function)
{
	std::promise<json> promise;

	On(name, [&](json& message)
	{
		promise.set_value(message);
	});

	std::future<json> future = promise.get_future();

	function(future.get());
}

void Network::Invoke()
{
	while (client.is_connecting_or_connected())
	{
		if (connected)
		{
			for (auto& msg : queue)
			{
				auto cbor = json::to_cbor(msg);

				client.send_packet(0, reinterpret_cast<const enet_uint8*>(cbor.data()), cbor.size(), ENET_PACKET_FLAG_RELIABLE);
			}

			queue.clear();
		}

		Consume();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Network::Destroy()
{
	Disconnect();

	enetpp::global_state::get().deinitialize();

	WaitForSingleObject(thread, INFINITE);
}

Network network;
