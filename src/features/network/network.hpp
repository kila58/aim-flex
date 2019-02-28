#pragma once

#include "../features.hpp"

#include "enet/enet.h"

#include "enetpp/client.h"
#include "enetpp/server.h"

#include "../../json.hpp"
#include "../../fifo_map.hpp"

template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;

using json = nlohmann::basic_json<my_workaround_fifo_map>;

static const int port = 3030;

struct NetworkCallback
{
	NetworkCallback(std::string name, std::function<void(json&)> callback) : name(name), callback(callback) {}

	std::string name;
	std::function<void(json&)> callback;
};

enum ChannelType
{
	channel_invalid = -1,
	channel_all = 0,
	channel_loader = 1,
	channel_cheat = 2
};

class Network : public BaseFeature
{
public:
	enetpp::trace_handler trace_handler;
	enetpp::client client;
	std::mutex mutex;
	// my queue > internal queue
	std::deque<json> queue;
	bool connected = false;
	std::deque<NetworkCallback> callbacks;
	std::string jwt_token;
	HANDLE thread;
public:
	Network() : BaseFeature(/*FRAMESTAGENOTIFY, 4u*/) {}

	void Init();
	void DoStuff();
	void Invoke();
	void Connect(const std::string& hostname, enet_uint16 port);
	void Disconnect();
	void Send(const std::string& to, const json& message);
	void Send(const std::string & to, const std::string& message);
	void Consume();
	void On(const std::string& name, std::function<void(json&)> function);
	void OnSync(const std::string& name, std::function<void(json&)> function);
	void Destroy();

	bool ShouldSkipInit()
	{
		return true;
	}

	~Network()
	{
		MessageBoxA(NULL, "network destructor", "", NULL);
	}
};

extern Network network;
 