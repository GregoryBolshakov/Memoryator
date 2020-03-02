#include "input_box.h"

#include "helper.h"

using namespace sf;

input_box::input_box() = default;

input_box::input_box(const FloatRect rect)
{
	init(rect);
}

input_box::~input_box() = default;

void input_box::init(const FloatRect rect)
{
	this->rect = FloatRect(rect);
	body.setSize(Vector2f(rect.width, rect.height));
	body.setPosition(rect.left, rect.top);
	body.setFillColor(outFill);
	innerPart.setSize(Vector2f(rect.width - microOffset, rect.height - microOffset));
	innerPart.setPosition(rect.left + microOffset / 2.0f, rect.top + microOffset / 2.0f);
	innerPart.setFillColor(inFill);
	cursor.setSize(Vector2f(microOffset / 1.5f, innerPart.getSize().y - 1.5f * microOffset));
	cursor.setPosition(innerPart.getPosition().x + microOffset, innerPart.getPosition().y + microOffset * 3.0f / 4);
	cursor.setFillColor(textColor);

	text.setFont(text_system::fonts.at(font_name::console_font));
	text.setCharacterSize(unsigned int(ceil(text_system::characterSize)));
	text.setFillColor(textColor);
	text.setPosition(innerPart.getPosition().x + microOffset * 2, innerPart.getPosition().y - microOffset / 2.0f);
}

void input_box::resetCursor(const bool toEnd)
{
	if (toEnd)
	{
		cursorPos = line.size();
		setCursorToPos();
	}
	else cursorPos = 0;
}

void input_box::setCursorToPos()
{
	text.setString(line.substr(0, cursorPos));
	cursor.setPosition(text.getGlobalBounds().left + text.getGlobalBounds().width + microOffset / 2.0f, cursor.getPosition().y);
}

void input_box::draw(sf::RenderWindow & window) const
{
	window.draw(body);
	window.draw(innerPart);
	text_system::drawString(line, font_name::console_font, characterSize, innerPart.getPosition().x + microOffset * 2.0f, innerPart.getPosition().y - microOffset / 2.0f, window, textColor);
	window.draw(cursor);
}

void input_box::animateCursor()
{
	if (cursor.getFillColor() == textColor)
		cursor.setFillColor(sf::Color(textColor.r, textColor.g, textColor.b, 10));
	else
		cursor.setFillColor(textColor);
}

void input_box::onMouseRelease()
{
	mousePressed = false;
	float minDist = helper::GetScreenSize().x + helper::GetScreenSize().y;
	int newPos = 0;
	for (auto i = 0u; i < line.size(); i++)
	{
		text.setString(line.substr(0, i));
		const float dist = abs(text.getGlobalBounds().left + text.getGlobalBounds().width - Mouse::getPosition().x);
		if (dist < minDist)
		{
			minDist = dist;
			newPos = i;
		}
	}
	cursorPos = newPos;
}

void input_box::interact(const long long elapsedTime)
{
	setCursorToPos();
	if (helper::isIntersects(Vector2f(Mouse::getPosition()), FloatRect(body.getPosition().x, body.getPosition().y, body.getSize().x, body.getSize().y)))
		innerPart.setFillColor(inFillSelected);
	else
		innerPart.setFillColor(inFill);

	if (Mouse::isButtonPressed(Mouse::Left))
	{
		mousePressed = true;
		mouseSelectVector.second = float(Mouse::getPosition().x);
	}

	timeAfterCursorAnimation += elapsedTime;
	if (timeAfterCursorAnimation >= timeForCursorAnimation)
	{
		timeAfterCursorAnimation = 0;
		animateCursor();
	}
}

void input_box::handleEvents(const Event event)
{
	if (event.type == Event::MouseButtonReleased)
		onMouseRelease();

	if (event.type == Event::TextEntered)
	{
		if (event.text.unicode == 8)
		{
			if (!line.empty() && cursorPos != 0)
			{
				line.erase(line.begin() + cursorPos - 1);
				cursorPos--;
			}
		}
		else
			if (text.getGlobalBounds().left + text.getGlobalBounds().width <= innerPart.getGlobalBounds().left + innerPart.getGlobalBounds().width &&
				!(Keyboard::isKeyPressed(Keyboard::Tilde) || Keyboard::isKeyPressed(Keyboard::Escape) || Keyboard::isKeyPressed(Keyboard::Enter)))
			{
				/*if (event.text.unicode >= 65 && event.text.unicode <= 90)
					event.text.unicode += 32;*/
				if (line.size() >= cursorPos)
					line.insert(cursorPos, 1, char(event.text.unicode));
				cursorPos++;
			}
	}

	if (event.type == Event::KeyPressed)
	{
		if (Keyboard::isKeyPressed(Keyboard::Delete) && cursorPos < line.size())
			line.erase(line.begin() + cursorPos);
		if (Keyboard::isKeyPressed(Keyboard::Left) && cursorPos > 0)
			cursorPos--;
		if (Keyboard::isKeyPressed(Keyboard::Right) && cursorPos < line.size())
			cursorPos++;
	}
}
