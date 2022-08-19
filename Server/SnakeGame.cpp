#include "SnakeGame.h"

const std::array<char, SnakeGame::HEX_CHAR_COUNT> SnakeGame::hexChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

SnakeGame::SnakeGame(const int32_t mapWidth, const int32_t mapHeight) : mapWidth(mapWidth), mapHeight(mapHeight)
{
	widthDistribution = std::uniform_int_distribution(0, mapWidth - 1);
	heightDistribution = std::uniform_int_distribution(0, mapHeight - 1);
	hexDistribution = std::uniform_int_distribution(MIN_HEX_CHAR, HEX_CHAR_COUNT - 1);
	
	std::random_device randomDevice;
	randEngine.seed(randomDevice());
}

void SnakeGame::tick()
{
	for (auto& pair : snakes)
	{
		Snake& snake = pair.second;
		snake.move(mapWidth, mapHeight);

		Point snakePos = snake.getHeadPos();

		if (apples.contains(snakePos))
		{
			apples.erase(snakePos);
			snake.addSegment();
		}

		drogon::SubscriberID subId = pair.first;

		// Check this snakes new position against every other snake's segments.
		for (auto& otherPair : snakes)
		{
			bool isDead = false;

			drogon::SubscriberID otherSubId = otherPair.first;
			Snake& otherSnake = otherPair.second;

			const auto& otherSegments = otherSnake.getSegments();
			for (int i = 0; i < otherSegments.size(); ++i)
			{
				// Make sure snakes can't collide with their own heads.
				if (i == 0 && subId == otherSubId) continue;

				const Point& otherSeg = otherSegments[i];

				if (snakePos == otherSeg)
				{
					Point p = getRandPoint();
					snake.respawn(p.x, p.y);
					break;
				}
			}

			if (isDead) break;
		}
	}

	size_t targetAppleCount = snakes.size() / 2;
	if (targetAppleCount < 1) targetAppleCount = 1;

	while (apples.size() < targetAppleCount)
	{
		Point p = getRandPoint();
		apples.insert(Point{ p.x, p.y });
	}
}

void SnakeGame::addPlayer(drogon::SubscriberID subId)
{
	Point p = getRandPoint();
	snakes.insert(std::make_pair(subId, Snake(subId, getRandHexColor(), p.x, p.y)));
}

void SnakeGame::removePlayer(drogon::SubscriberID subId)
{
	snakes.erase(subId);
}

void SnakeGame::changePlayerDirection(drogon::SubscriberID subId, const int32_t dirX, const int32_t dirY)
{
	snakes[subId].changeDirection(dirX, dirY);
}

Json::Value SnakeGame::getStateJson() const
{
	Json::Value state;
	Json::Value& stateSnakes = state["snakes"];

	int32_t i = 0;
	for (const auto& pair : snakes)
	{
		const Snake& snake = pair.second;
		stateSnakes[i] = snake.getStateJson();
		++i;
	}

	Json::Value& stateApples = state["apples"];

	i = 0;
	for (const auto& apple : apples)
	{
		stateApples[i] = apple.getStateJson();
		++i;
	}

	return state;
}

Point SnakeGame::getRandPoint()
{
	int32_t x = widthDistribution(randEngine);
	int32_t y = heightDistribution(randEngine);

	return Point{ x, y };
}

std::string SnakeGame::getRandHexColor()
{
	std::stringstream colorStream;
	colorStream << "#";

	for (int i = 0; i < 6; i++)
	{
		int32_t index = hexDistribution(randEngine);
		colorStream << hexChars[index];
	}

	return colorStream.str();
}