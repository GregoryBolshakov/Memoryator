#include "InputBox.h"
#include "Helper.h"

using namespace sf;

InputBox::InputBox() = default;

InputBox::InputBox(IntRect rect)
{
	init(rect);
}

InputBox::~InputBox() = default;

void InputBox::init(IntRect rect)
{
	this->rect = rect;
	body.setSize(Vector2f(rect.width, rect.height));
	body.setPosition(rect.left, rect.top);
	body.setFillColor(outFill);
	innerPart.setSize(Vector2f(rect.width - microOffset, rect.height - microOffset));
	innerPart.setPosition(rect.left + microOffset / 2.0f, rect.top + microOffset / 2.0f);
	innerPart.setFillColor(inFill);
	cursor.setSize(Vector2f(microOffset / 1.5f, innerPart.getSize().y - 1.5 * microOffset));
	cursor.setPosition(innerPart.getPosition().x + microOffset, innerPart.getPosition().y + microOffset * 3.0f / 4);
	cursor.setFillColor(textColor);

	text.setFont(TextWriter::fonts.at(ConsoleFont));
	text.setCharacterSize(TextWriter::characterSize);
	text.setFillColor(textColor);
	text.setPosition(innerPart.getPosition().x + microOffset * 2, innerPart.getPosition().y - microOffset / 2.0f);
}

void InputBox::resetCursor(bool toEnd)
{
	if (toEnd)
	{
		cursorPos = line.size();
		setCursorToPos(); 
	}
	else cursorPos = 0;
}

void InputBox::setCursorToPos()
{
	text.setString(line.substr(0, cursorPos));
	cursor.setPosition(text.getGlobalBounds().left + text.getGlobalBounds().width + microOffset / 2.0f, cursor.getPosition().y);
}

void InputBox::draw(sf::RenderWindow& window)
{
	window.draw(body);
	window.draw(innerPart);
	TextWriter::drawString(line, ConsoleFont, characterSize, innerPart.getPosition().x + microOffset * 2, innerPart.getPosition().y - microOffset / 2.0f, &window, textColor);
	window.draw(cursor);
}

void InputBox::animateCursor()
{
	if (cursor.getFillColor() == textColor)
		cursor.setFillColor(Color(textColor.r, textColor.g, textColor.b, 10));
	else
		cursor.setFillColor(textColor);
}

void InputBox::onMouseRelease()
{
	mousePressed = false;
	float minDist = Helper::GetScreenSize().x + Helper::GetScreenSize().y;
	int newPos = 0;
	for (int i = 0; i < line.size(); i++)
	{
		text.setString(line.substr(0, i));
		const float dist = abs(text.getGlobalBounds().left + text.getGlobalBounds().width- Mouse::getPosition().x);
		if (dist < minDist)
		{
			minDist = dist;
			newPos = i;
		}
	}
	cursorPos = newPos;
}

void InputBox::interact(long long elapsedTime)
{
	if (cursorPos < 0)
		cursorPos = 0;
	setCursorToPos();
	if (Helper::isIntersects(Vector2f(Mouse::getPosition()), IntRect(body.getPosition().x, body.getPosition().y, body.getSize().x, body.getSize().y)))
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

void InputBox::handleEvents(Event event)
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

	if (cursorPos < 0)
		cursorPos = 0;
}


