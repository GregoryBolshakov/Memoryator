#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

#include "dynamic_object.h"
#include "terrain_object.h"

using namespace sf;

class helper
{
public:
	static Vector2f GetScreenSize();
	static int getFps();
	// static void drawText(std::string text, int size, int Posx, int Posy, RenderWindow *window);
	// static void drawTextWithSettings(std::string text, int size, int Posx, int Posy, sf::Color color, RenderWindow *window);
	static std::string withoutNums(std::string s);
	static std::string getObjectName(const std::string& s);
	// static std::string getSpriteName(std::string s);
	static bool isIntersects(Vector2f pos, FloatRect shape);
	static bool isIntersects(Vector2f pos, Vector2f circlePos, float radius);
	static bool isIntersectTerrain(Vector2f position, terrain_object& terrain, float radius);
	static Side getSide(Vector2f position, Vector2f anotherPosition);
	static float getDist(Vector2f a, Vector2f b);
	static float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3);
	static bool checkSigns(float a, float b);
	static RectangleShape makeLine(Vector2f point1, Vector2f point2, sf::Color color = sf::Color(255, 240, 134, 255));
	static std::pair<float, float> solveSqr(float a, float b, float c);
    static std::vector<std::string> split(std::string line, char delimiter = ' ');
};
