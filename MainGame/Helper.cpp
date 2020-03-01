#include "Helper.h"

#include <windows.h>

Vector2f Helper::GetScreenSize()
{
	Vector2f screenSize;
	DEVMODEA lpDevMode;

	if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &lpDevMode))
	{
		screenSize.x = float(lpDevMode.dmPelsWidth);
		screenSize.y = float(lpDevMode.dmPelsHeight);
	}
	return screenSize;
}

int Helper::getFps()
{
	DEVMODEA lpDevMode;

	if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &lpDevMode))
	{
		return lpDevMode.dmDisplayFrequency;
	}
	return 0;
}

std::pair<float, float> Helper::solveSqr(const float a, const float b, const float c)
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
//	Vector2f pos(static_cast<float>(Posx),static_cast<float>(Posy));
//	result.setPosition(Vector2f(pos));
//	window->draw(result);
//}
//
//void Helper::drawTextWithSettings(std::string text, int size, int Posx, int Posy, Color color, RenderWindow *window)
//{
//	Font font;
//	font.loadFromFile("fonts/normal.ttf");
//	Text result(text, font, size);
//	result.setFillColor(color);
//	Vector2f pos(static_cast<float>(Posx), static_cast<float>(Posy));
//	result.setPosition(Vector2f(pos));
//	window->draw(result);
//}

std::string Helper::withoutNums(std::string s)
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

std::string Helper::getObjectName(const std::string& s)
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

bool Helper::isIntersects(const Vector2f pos, const FloatRect shape)
{
	if (pos.x >= shape.left && pos.x <= shape.left + shape.width && pos.y >= shape.top && pos.y <= shape.top + shape.height)
		return true;
	return false;
}

bool Helper::isIntersects(const Vector2f pos, const Vector2f circlePos, const float radius)
{
	return getDist(pos, circlePos) <= radius;
}

bool Helper::isIntersectTerrain(const Vector2f position, TerrainObject& terrain, const float radius)
{
	const auto f1 = terrain.getFocus1();
	const auto f2 = terrain.getFocus2();
	return sqrt((position.x - f1.x) * (position.x - f1.x) + (position.y - f1.y) * (position.y - f1.y)) +
		sqrt((position.x - f2.x) * (position.x - f2.x) + (position.y - f2.y) * (position.y - f2.y))/* - dynamic.radius*/ <= terrain.getEllipseSize() + radius;
}

Side Helper::getSide(const Vector2f position, const Vector2f anotherPosition)
{
	auto answer = undefined;
	const auto alpha = atan((float(anotherPosition.y) - position.y) / (float(anotherPosition.x) - position.x)) * 180 / pi;

	if (position.y >= anotherPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
		answer = up;
	else
		if (position.x <= anotherPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
			answer = right;
		else
			if (position.y <= anotherPosition.y && abs(alpha) >= 45 && abs(alpha) <= 90)
				answer = down;
			else
				if (position.x >= anotherPosition.x && abs(alpha) >= 0 && abs(alpha) <= 45)
					answer = left;
	return answer;
}

float Helper::getDist(const Vector2f a, const Vector2f b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

float Helper::triangleArea(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3)
{
	return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}

bool Helper::checkSigns(const float a, const float b)
{
	return a > 0 && b > 0 || a < 0 && b < 0;
}

RectangleShape Helper::makeLine(Vector2f point1, Vector2f point2, sf::Color color)
{
	if (point1.y >= point2.y)
		std::swap(point1, point2);

	point1.y -= 4;

	RectangleShape result;
	result.setPosition(point1);
	result.setSize(Vector2f(sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2)), 4));

	if (point1.x >= point2.x)
		result.rotate(acos(abs(point1.y - point2.y) / sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2))) / pi * 180 + 90);
	else
		result.rotate(acos(abs(point1.x - point2.x) / sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2))) / pi * 180);
	result.setFillColor(color);
	return result;
}

std::vector<std::string> Helper::split(std::string line, char delimiter)
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
