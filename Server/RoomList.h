#pragma once

#include "Room.h"
#include "Utils.h"
#include "json/json.h"
#include "drogon/WebSocketClient.h"
#include "drogon/PubSubService.h"

#include <thread>
#include <mutex>

class RoomList
{
public:
	static const int32_t tickTimeMs = 100;

	RoomList();

	drogon::SubscriberID subscribe(std::string roomName, const drogon::WebSocketConnectionPtr& conn);
	void unsubscribe(std::string roomName, drogon::SubscriberID subId);
	void receiveMsg(std::string roomName, drogon::SubscriberID subId, const Json::Value& msg);
	void tick();

private:
	drogon::PubSubService<std::string> pubSub;
	std::unordered_map<std::string, Room> rooms;
	std::mutex roomsMutex;
	std::thread roomTicker;
};
