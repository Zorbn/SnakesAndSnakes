#pragma once

#include "SnakeGame.h"
#include "RoomList.h"
#include "Utils.h"
#include "drogon/WebSocketController.h"
#include "drogon/PubSubService.h"
#include "json/json.h"

#include <cinttypes>
#include <map>
#include <chrono>
#include <unordered_map>
#include <mutex>
#include <thread>

class WebSocketController : public drogon::WebSocketController<WebSocketController>
{
public:
	WebSocketController();

	void handleNewMessage(const drogon::WebSocketConnectionPtr& wsConnPtr, std::string&& message, const drogon::WebSocketMessageType& type) override;
	void handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn) override;
	void handleNewConnection(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn) override;

	WS_PATH_LIST_BEGIN
		WS_PATH_ADD("/chat", drogon::Get);
	WS_PATH_LIST_END

private:
	RoomList roomList;
	std::unique_ptr<Json::StreamWriter> writer;
	std::ostringstream sendStream;
};

struct Subscriber
{
	std::string roomName;
	drogon::SubscriberID id;
};