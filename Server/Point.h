#pragma once

#include "json/json.h"

#include <cinttypes>

struct Point
{
	int32_t x;
	int32_t y;

	Json::Value getStateJson() const
	{
		Json::Value point;

		point["x"] = x;
		point["y"] = y;

		return point;
	}

	bool operator<(const Point& other) const
	{
		return std::tie(x, y) < std::tie(other.x, other.y);
	}

	bool operator==(const Point& other) const
	{
		return x == other.x && y == other.y;
	}
};