#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

class terrain_object;
enum class side;

class helper
{
public:
	static sf::Vector2f GetScreenSize();
	static int getFps();
	// static void drawText(std::string text, int size, int Posx, int Posy, RenderWindow *window);
	// static void drawTextWithSettings(std::string text, int size, int Posx, int Posy, sf::Color color, RenderWindow *window);
	static std::string withoutNums(std::string s);
	static std::string getObjectName(const std::string& s);
	// static std::string getSpriteName(std::string s);
	static bool isIntersects(sf::Vector2f pos, sf::FloatRect shape);
	static bool isIntersects(sf::Vector2f pos, sf::Vector2f circlePos, float radius);
	static bool isIntersectTerrain(sf::Vector2f position, terrain_object& terrain, float radius);
	static side getSide(sf::Vector2f position, sf::Vector2f anotherPosition);
	static float getDist(sf::Vector2f a, sf::Vector2f b);
	static float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3);
	static bool checkSigns(float a, float b);
	static sf::RectangleShape makeLine(sf::Vector2f point1, sf::Vector2f point2, sf::Color color = sf::Color(255, 240, 134, 255));
	static std::pair<float, float> solveSqr(float a, float b, float c);
    static std::vector<std::string> split(std::string line, char delimiter = ' ');
	static sf::Vector2f rotate(sf::Vector2f point, float angle_deg);
	static sf::Vector2f rotate(sf::Vector2f point, float angle_deg, sf::Vector2f origin);
	static sf::FloatRect rotate(sf::FloatRect rect, float angle_deg);
	static sf::FloatRect rotate(sf::FloatRect rect, float angle_deg, sf::Vector2f origin);
};
