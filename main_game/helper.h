#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

class terrain_object;
enum class side;

class helper
{
public:
	// static void drawText(std::string text, int size, int Posx, int Posy, RenderWindow *window);
	// static void drawTextWithSettings(std::string text, int size, int Posx, int Posy, sf::Color color, RenderWindow *window);
	static std::string withoutNums(std::string s);
	static std::string getObjectName(const std::string& s);
	// static std::string getSpriteName(std::string s);
	static bool check_signs(float a, float b);
	static sf::RectangleShape make_line(sf::Vector2f point1, sf::Vector2f point2, sf::Color color = sf::Color(255, 240, 134, 255));
	static std::pair<float, float> solveSqr(float a, float b, float c);
    static std::vector<std::string> split(std::string line, char delimiter = ' ');
	static sf::Vector2f rotate(sf::Vector2f point, float angle_deg);
	static sf::Vector2f rotate(sf::Vector2f point, float angle_deg, sf::Vector2f origin);
	static sf::FloatRect rotate(sf::FloatRect rect, float angle_deg);
	static sf::FloatRect rotate(sf::FloatRect rect, float angle_deg, sf::Vector2f origin);
};
