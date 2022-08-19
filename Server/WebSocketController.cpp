#include "WebSocketController.h"

WebSocketController::WebSocketController() : writer(Utils::MakeJsonWriter()) {}

void WebSocketController::handleNewMessage(const drogon::WebSocketConnectionPtr& conn, std::string&& message, const drogon::WebSocketMessageType& type)
{
	if (type == drogon::WebSocketMessageType::Text)
	{
		Subscriber& s = conn->getContextRef<Subscriber>();

		Json::Value msg;
		Json::Reader reader;

		if (reader.parse(message, msg))
		{
			roomList.receiveMsg(s.roomName, s.id, msg);
		}
	}
}

void WebSocketController::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn)
{
	Subscriber& s = conn->getContextRef<Subscriber>();

	LOG_INFO << "Player disconnected to room " << s.roomName << " with id of \"" << s.id << "\"";

	roomList.unsubscribe(s.roomName, s.id);
}

void WebSocketController::handleNewConnection(const drogon::HttpRequestPtr& req, const drogon::WebSocketConnectionPtr& conn)
{
	const std::string& roomName = req->getParameter("room_name");
	Subscriber s;
	s.roomName = roomName;
	s.id = roomList.subscribe(roomName, conn);

	LOG_INFO << "Player connected to room " << s.roomName << " with id of \"" << s.id << "\"";

	conn->setContext(std::make_shared<Subscriber>(std::move(s)));

	Json::Value init;
	init["type"] = "init";
	init["localId"] = s.id;

	writer->write(init, &sendStream);
	conn->send(sendStream.str());
	sendStream.str("");
}