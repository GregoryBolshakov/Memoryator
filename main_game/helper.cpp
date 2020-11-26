#include "helper.h"
#include "math_constants.h"
#include "tags.h"
#include "terrain_object.h"

#include <windows.h>

sf::Vector2f helper::GetScreenSize()
{
	sf::Vector2f screenSize;
	DEVMODEA lpDevMode;

	if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &lpDevMode))
	{
		screenSize.x = float(lpDevMode.dmPelsWidth);
		screenSize.y = float(lpDevMode.dmPelsHeight);
	}
	return screenSize;
}

int helper::getFps()
{
	DEVMODEA lpDevMode;

	if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &lpDevMode))
	{
		return lpDevMode.dmDisplayFrequency;
	}
	return 0;
}

std::pair<float, float> helper::solveSqr(const float a, const float b, const float c)
{
	const auto discr = b * b - 4 * a * c;
	auto firstRoot = (-b + sqrt(discr)) / (2 * a);
	auto secondRoot = (-b - sqrt(discr)) / (2 * a);

	if (discr < 0)
		return std::make_pair(-0.1111122f, -0.1111122f);
	return std::make_pair(firstRoot, secondRoot);
}

//
//void Helper::drawText(std::string text, int size, int Posx, int Posy, RenderWindow *window)
//{
//	Font font;
//	font.loadFromFile("fonts/normal.ttf");
//	Text result(text, font, size);
//	result.setFillColor(Color::White);
//	sf::Vector2f pos(static_cast<float>(Posx),static_cast<float>(Posy));
//	result.setPosition(sf::Vector2f(pos));
//	window->draw(result);
//}
//
//void Helper::drawTextWithSettings(std::string text, int size, int Posx, int Posy, sf::Color color, RenderWindow *window)
//{
//	Font font;
//	font.loadFromFile("fonts/normal.ttf");
//	Text result(text, font, size);
//	result.setFillColor(color);
//	sf::Vector2f pos(static_cast<float>(Posx), static_cast<float>(Posy));
//	result.setPosition(sf::Vector2f(pos));
//	window->draw(result);
//}

std::string helper::withoutNums(std::string s)
{
	bool isFinish;
	while (true)
	{
		isFinish = true;
		for (auto i = 0; i < s.length(); i++)
		{
			if (s[i] >= '0' && s[i] <= '9')
			{
				s.erase(i, 1);
				isFinish = false;
			}
		}
		if (isFinish)
			break;
	}
	return s;
}

std::string helper::getObjectName(const std::string& s)
{
	std::string answer;

	for (auto i : s)
	{
		if (i != '.')
			answer += i;
		else
			break;
	}

	return answer;
}

//std::string Helper::getSpriteName(std::string s)
//{
//	for (int i = s.length() - 1; i >= 0; i--)
//	{
//		if (s[i] >= '0' && s[i] <= '9')
//			s.erase(i, 1);
//		else
//			return s;
//	}
//}

bool helper::isIntersects(const sf::Vector2f pos, const sf::FloatRect shape)
{
	if (pos.x >= shape.left && pos.x <= shape.left + shape.width && pos.y >= shape.top && pos.y <= shape.top + shape.height)
		return true;
	return false;
}

bool helper::isIntersects(const sf::Vector2f pos, const sf::Vector2f circlePos, const float radius)
{
	return getDist(pos, circlePos) <= radius;
}

bool helper::isIntersectTerrain(const sf::Vector2f position, terrain_object& terrain, const float radius)
{
	const auto f1 = terrain.get_focus1();
	const auto f2 = terrain.get_focus2();
	return sqrt((position.x - f1.x) * (position.x - f1.x) + (position.y - f1.y) * (position.y - f1.y)) +
		sqrt((position.x - f2.x) * (position.x - f2.x) + (position.y - f2.y) * (position.y - f2.y))/* - dynamic.radius*/ <= terrain.get_ellipse_size() + radius;
}

side helper::getSide(const sf::Vector2f position, const sf::Vector2f anotherPosition)
{
	auto answer = side::undefined;
	const auto alpha = atan((float(anotherPosition.y) - position.y) / (float(anotherPosition.x) - position.x)) * 180 / pi;

	if (position.y >= anotherPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
		answer = side::up;
	else
		if (position.x <= anotherPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
			answer = side::right;
		else
			if (position.y <= anotherPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
				answer = side::down;
			else
				if (position.x >= anotherPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
					answer = side::left;
	return answer;
}

float helper::getDist(const sf::Vector2f a, const sf::Vector2f b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

float helper::triangleArea(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3)
{
	return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

bool helper::checkSigns(const float a, const float b)
{
	return a > 0 && b > 0 || a < 0 && b < 0;
}

sf::RectangleShape helper::makeLine(sf::Vector2f point1, sf::Vector2f point2, sf::Color color)
{
	if (point1.y >= point2.y)
		std::swap(point1, point2);

	point1.y -= 4;

	sf::RectangleShape result;
	result.setPosition(point1);
	result.setSize(sf::Vector2f(sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2)), 4));

	if (point1.x >= point2.x)
		result.rotate(float(acos(abs(point1.y - point2.y) / sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2))) / pi * 180) + 90);
	else
		result.rotate(float(acos(abs(point1.x - point2.x) / sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2))) / pi * 180));
	result.setFillColor(color);
	return result;
}

std::vector<std::string> helper::split(std::string line, char delimiter)
{
	std::vector<std::string> commands;
	auto temp = std::move(line);
	if (temp[temp.size() - 1] != delimiter)
		temp.push_back(delimiter);
	while (!temp.empty())
	{
		auto token = temp.substr(0, temp.find(delimiter));
		commands.push_back(token);
		temp.erase(0, token.size() + 1);
	}
	return commands;
}

sf::Vector2f helper::rotate(const sf::Vector2f point, const float angle_deg)
{
	return rotate(point, angle_deg, point / 2.0f);
}

sf::Vector2f helper::rotate(const sf::Vector2f point, const float angle_deg, const sf::Vector2f origin)
{
	sf::Transform rotation;
	rotation.rotate(angle_deg, origin.x, origin.y);
	return rotation.transformPoint(point);
}

sf::FloatRect helper::rotate(const sf::FloatRect rect, const float angle_deg)
{
	return rotate(rect, angle_deg, sf::Vector2f(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f));
}

sf::FloatRect helper::rotate(const sf::FloatRect rect, const float angle_deg, const sf::Vector2f origin)
{
	sf::Transform rotation;
	rotation.rotate(angle_deg, origin.x, origin.y);
	return rotation.transformRect(rect);
}
