#pragma once

#include "SnakeGame.h"
#include "json/json.h"
#include "drogon/PubSubService.h"

class Room
{
public:
	static const int32_t mapWidth = 40;
	static const int32_t mapHeight = 30;

	Room() : game(SnakeGame(mapWidth, mapHeight)) {}

	void tick();
	void subscribe(drogon::SubscriberID subId);
	void unsubscribe(drogon::SubscriberID subId);
	void receiveMsg(drogon::SubscriberID subId, const Json::Value& msg);
	Json::Value getStateJson() const;

public:
	int32_t subscribers = 0;
private:
	SnakeGame game;
};