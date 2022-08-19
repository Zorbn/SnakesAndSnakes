#pragma once

#include "json/json.h"

class Utils
{
public:
	static std::unique_ptr<Json::StreamWriter> MakeJsonWriter();
};
