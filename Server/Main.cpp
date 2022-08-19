#include "Main.h"

int main()
{
	const std::string ip = "localhost";
	const int32_t port = 3000;

	LOG_INFO << "Hosting on '" << ip << ":" << port << "'";

	drogon::app()
		.addListener(ip, port)
		.run();
	
	return 0;
}
