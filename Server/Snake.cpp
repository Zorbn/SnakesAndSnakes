#include "Snake.h"

Snake::Snake(drogon::SubscriberID subId, std::string color, const int32_t x, const int32_t y, const int32_t segCount) : subId(subId), color(color)
{
	reset(x, y, segCount);
}

void Snake::move(const int32_t mapWidth, const int32_t mapHeight)
{
	for (size_t i = segments.size() - 1; i >= 0; --i)
	{
		Point& seg = segments[i];

		if (i == 0)
		{
			seg.x = (seg.x + dirX) % mapWidth;
			seg.y = (seg.y + dirY) % mapHeight;

			if (seg.x < 0) seg.x = mapWidth - 1;
			if (seg.y < 0) seg.y = mapHeight - 1;

			break;
		}

		Point& nextSeg = segments[i - 1];
		seg.x = nextSeg.x;
		seg.y = nextSeg.y;
	}
}

void Snake::changeDirection(const int32_t dirX, const int32_t dirY)
{
	bool isMagnitudeOutOfRange = dirX > 1 || dirX < -1 || dirY > 1 || dirY < -1;
	bool isTryingToStop = dirX == 0 && dirY == 0;
	bool isTryingToMoveDiagonally = dirX != 0 && dirY != 0;
	bool isTryingToTurn180Degrees = dirX == -this->dirX && dirY == -this->dirY;

	if (isMagnitudeOutOfRange || isTryingToStop || isTryingToMoveDiagonally || isTryingToTurn180Degrees) return;

	this->dirX = dirX;
	this->dirY = dirY;
}

void Snake::addSegment()
{
	size_t lastI = segments.size() - 1;
	segments.push_back(segments[lastI]);
}

void Snake::respawn(const int32_t x, const int32_t y, const int32_t segCount)
{
	reset(x, y, segCount);
}

Json::Value Snake::getStateJson() const
{
	Json::Value snake;

	Json::Value& stateSegments = snake["segments"];

	for (int32_t i = 0; i < segments.size(); ++i)
	{
		stateSegments[i] = segments[i].getStateJson();
	}

	snake["color"] = color;
	snake["id"] = subId;

	return snake;
}

Point Snake::getHeadPos() const
{
	size_t segmentCount = segments.size();

	if (segmentCount == 0) return Point{ 0, 0 };

	return Point{ segments[0].x, segments[0].y };
}

const std::vector<Point>& Snake::getSegments() const
{
	return segments;
}

void Snake::reset(const int32_t x, const int32_t y, const int32_t segCount)
{
	segments.clear();

	for (size_t i = 0; i < segCount; ++i)
	{
		segments.push_back(Point{ x, y });
	}
}