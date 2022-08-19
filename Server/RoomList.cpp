#include "RoomList.h"

RoomList::RoomList() : roomTicker(&RoomList::tick, this)
{
	roomTicker.detach();
};

void RoomList::tick()
{
	std::unique_ptr<Json::StreamWriter> writer = Utils::MakeJsonWriter();
	std::ostringstream sendStream;

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(tickTimeMs));

		std::scoped_lock(roomsMutex);

		for (auto& roomPair : rooms)
		{
			const std::string& name = roomPair.first;
			Room& room = roomPair.second;

			room.tick();

			Json::Value state = room.getStateJson();
			state["type"] = "state";

			writer->write(state, &sendStream);
			pubSub.publish(name, sendStream.str());
			sendStream.str("");
		}
	}
}

drogon::SubscriberID RoomList::subscribe(std::string roomName, const drogon::WebSocketConnectionPtr& conn)
{
	std::scoped_lock(roomsMutex);

	drogon::SubscriberID subId = pubSub.subscribe(roomName,
		[conn](const std::string& topic, const std::string& message) {
			conn->send(message);
		});

	if (!rooms.contains(roomName))
	{
		LOG_INFO << "The first player joined room '" << roomName << "', opening it.";

		Room room;
		rooms.emplace(std::make_pair(roomName, room));
	}

	rooms[roomName].subscribe(subId);

	return subId;
}

void RoomList::unsubscribe(std::string roomName, drogon::SubscriberID subId)
{
	std::scoped_lock(roomsMutex);

	pubSub.unsubscribe(roomName, subId);

	Room& room = rooms[roomName];
	room.unsubscribe(subId);

	if (room.subscribers <= 0)
	{
		LOG_INFO << "All players left room '" << roomName << "', closing it.";
		rooms.erase(roomName);
	}
}

void RoomList::receiveMsg(std::string roomName, drogon::SubscriberID subId, const Json::Value& msg)
{
	rooms[roomName].receiveMsg(subId, msg);
}