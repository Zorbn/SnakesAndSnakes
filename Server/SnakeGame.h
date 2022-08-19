#pragma once

#include "Snake.h"
#include "drogon/PubSubService.h"
#include "json/json.h"

#include <vector>
#include <unordered_map>
#include <set>
#include <random>
#include <sstream>

class SnakeGame
{
public:
	SnakeGame(const int32_t mapWidth, const int32_t mapHeight);

	void tick();
	void addPlayer(drogon::SubscriberID subId);
	void removePlayer(drogon::SubscriberID subId);
	void changePlayerDirection(drogon::SubscriberID subId, const int32_t dirX, const int32_t dirY);
	Json::Value getStateJson() const;

private:
	static const int32_t HEX_CHAR_COUNT = 16;
	static const int32_t MIN_HEX_CHAR = 5;
	static const std::array<char, HEX_CHAR_COUNT> hexChars;

	int32_t mapWidth;
	int32_t mapHeight;
	std::unordered_map<drogon::SubscriberID, Snake> snakes;
	std::set<Point> apples;

	std::minstd_rand randEngine;
	std::uniform_int_distribution<int32_t> widthDistribution;
	std::uniform_int_distribution<int32_t> heightDistribution;
	std::uniform_int_distribution<int32_t> hexDistribution;


	Point getRandPoint();
	std::string getRandHexColor();
};