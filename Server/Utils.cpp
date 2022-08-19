#include "Utils.h"

std::unique_ptr<Json::StreamWriter> Utils::MakeJsonWriter()
{
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "";

	return std::unique_ptr<Json::StreamWriter>(builder.newStreamWriter());
}