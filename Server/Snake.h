#pragma once

#include "Point.h"
#include "json/json.h"
#include "drogon/PubSubService.h"

#include <vector>

class Snake
{
public:
	static const int32_t DEFAULT_SEG_COUNT = 3;

	Snake(drogon::SubscriberID subId = 0, std::string color = "#ffffff", const int32_t x = 0, const int32_t y = 0, const int32_t segCount = DEFAULT_SEG_COUNT);

	void move(const int32_t mapWidth, const int32_t mapHeight);
	void changeDirection(const int32_t dirX, const int32_t dirY);
	void addSegment();
	void respawn(const int32_t x = 0, const int32_t y = 0, const int32_t segCount = DEFAULT_SEG_COUNT);
	Json::Value getStateJson() const;
	Point getHeadPos() const;
	const std::vector<Point>& getSegments() const;
	
private:
	std::vector<Point> segments;
	int32_t dirX = 1;
	int32_t dirY = 0;
	std::string color;
	drogon::SubscriberID subId;

	void reset(const int32_t x = 0, const int32_t y = 0, const int32_t segCount = DEFAULT_SEG_COUNT);
};