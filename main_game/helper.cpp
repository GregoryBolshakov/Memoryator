#include "helper.h"
#include "math_constants.h"

std::pair<float, float> helper::solveSqr(const float a, const float b, const float c)
{
	const auto discr = b * b - 4 * a * c;
	auto firstRoot = (-b + sqrt(discr)) / (2 * a);
	auto secondRoot = (-b - sqrt(discr)) / (2 * a);

	if (discr < 0)
		return std::make_pair(-0.1111122f, -0.1111122f);
	return std::make_pair(firstRoot, secondRoot);
}

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

bool helper::check_signs(const float a, const float b)
{
	return a > 0 && b > 0 || a < 0 && b < 0;
}

sf::RectangleShape helper::make_line(sf::Vector2f point1, sf::Vector2f point2, sf::Color color)
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
