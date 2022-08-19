#include "Room.h"

void Room::tick()
{
	game.tick();
}

Json::Value Room::getStateJson() const
{
	return game.getStateJson();
}

void Room::subscribe(drogon::SubscriberID subId)
{
	game.addPlayer(subId);
	++subscribers;
}

void Room::unsubscribe(drogon::SubscriberID subId)
{
	game.removePlayer(subId);
	--subscribers;
}

void Room::receiveMsg(drogon::SubscriberID subId, const Json::Value& msg)
{
	if (msg.isMember("type") && msg["type"].isString())
	{
		std::string type = msg["type"].asString();

		if (type == "changeDirection")
		{
			if (!msg.isMember("dirX") || !msg["dirX"].isInt() || !msg.isMember("dirY") || !msg["dirX"].isInt()) return;

			game.changePlayerDirection(subId, msg["dirX"].asInt(), msg["dirY"].asInt());
		}
	}
}