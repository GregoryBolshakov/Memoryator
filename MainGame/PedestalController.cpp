#include "PedestalController.h"
#include "Helper.h"
#include <fstream>
#include <iomanip>
#include "ObjectInitializer.h"

PedestalController::PedestalController()
{
	filedFigure.setSize(Vector2f(size));
	filedFigure.setFillColor(Color(255, 255, 255, 80));
	focusFigures.resize(2);
	focusFigures[0].setRadius(size.x);
	focusFigures[0].setFillColor(Color(255, 71, 71, 180));
	focusFigures[1].setRadius(size.x);
	focusFigures[1].setFillColor(Color(255, 71, 71, 180));
	focuses.resize(2);
	lastMousePos = Vector2f(Mouse::getPosition());
}

PedestalController::~PedestalController()
{
}

void PedestalController::start(TerrainObject* object)
{
	readyToStart = false;
	boundObject = object;
	focuses[0] = boundObject->getFocus1();
	focuses[1] = boundObject->getFocus2();
	running = true;
}

void PedestalController::stop()
{
	writeToFile();
	readyToStart = false;
	boundObject = nullptr;
	running = false;
}

void PedestalController::writeToFile()
{
	if (!boundObject)
		return;

	Vector2f center = Vector2f((focuses[0].x + focuses[1].x) / 2, (focuses[0].y + focuses[1].y) / 2);
	Vector2f offset = { (boundObject->getTextureOffset().x + center.x - boundObject->getPosition().x) / boundObject->getTextureSize().x,
		(boundObject->getTextureOffset().y + center.y - boundObject->getPosition().y) / boundObject->getTextureSize().y };
	Vector2f focus1 = { (focuses[0].x - center.x) / boundObject->getTextureSize().x, (focuses[0].y - center.y) / boundObject->getTextureSize().y };
	Vector2f focus2 = { (focuses[1].x - center.x) / boundObject->getTextureSize().x , (focuses[1].y - center.y) / boundObject->getTextureSize().y };
	std::ofstream fout("Game/pedestalsInfo.txt");
	fout.clear();
	fout << std::setprecision(3) << "Tag: " << ObjectInitializer::mappedTags.at(boundObject->tag) + " " << '\n';
	fout << std::setprecision(3) << "Type: " << boundObject->getType() << '\n';
	fout << std::setprecision(3) << "OffsetX: " << offset.x << " " << "OffsetY: " << offset.y << '\n';
	fout << std::setprecision(3) << "Focus1 X: " << focus1.x << " " << "Focus1 Y: " << focus1.y << '\n';
	fout << std::setprecision(3) << "Focus2 X: " << focus2.x << " " << "Focus2 Y: " << focus2.y << '\n';
	fout << std::setprecision(3) << "Ellipse size: " << boundObject->ellipseSizeMultipliers[0] << '\n';
	fout.close();
}

void PedestalController::interact(float elapsedTime)
{
	if (!boundObject || !running)
		return;

	// moving focuses
	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	const Vector2f worldMousePos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x * scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);
	for (int i = 0; i < focuses.size(); i++)
	{
		if (Helper::getDist(worldMousePos, focuses[i]) <= focusFigures[i].getRadius())
		{
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				focuses[i].x = worldMousePos.x;
				focusFigures[i].setPosition(mousePos.x, focusFigures[i].getPosition().y);
			}
			if (Mouse::isButtonPressed(Mouse::Right))
			{
				focuses[i].y = worldMousePos.y;
				focusFigures[i].setPosition(focusFigures[i].getPosition().x, mousePos.y);
			}
		}
		// alignment
		if (focuses.size() == 2)
		{
			if (abs(focuses[0].x - focuses[1].x) < size.x / 2.0f)
				focuses[i].x = focuses[(i + 1) % 2].x;
			if (abs(focuses[0].y - focuses[1].y) < size.y / 2.0f)
				focuses[i].y = focuses[(i + 1) % 2].y;
		}
		//----------
	}
	//---------------

	// resize ellipse
	if (Mouse::isButtonPressed(Mouse::Middle))
	{
		Vector2f center = Vector2f((focuses[0].x + focuses[1].x) / 2, (focuses[0].y + focuses[1].y) / 2);
		boundObject->ellipseSizeMultipliers[0] += (Helper::getDist(mousePos, center) - Helper::getDist(lastMousePos, center)) / 30;		
	}
	lastMousePos = mousePos;
	//---------------

	boundObject->setFocuses(focuses[0], focuses[1], Helper::getDist(focuses[0], focuses[1]) * boundObject->ellipseSizeMultipliers[0]);
}

void PedestalController::draw(RenderWindow* window, Vector2f cameraPosition, float scaleFactor)
{
	this->cameraPosition = cameraPosition;
	this->scaleFactor = scaleFactor;
	if (!boundObject || !running)
		return;

	const Vector2i upperLeft = Vector2i(std::min(focuses[0].x, focuses[1].x) - boundObject->getMicroBlockCheckAreaBounds().x, std::min(focuses[0].y, focuses[1].y) - boundObject->getMicroBlockCheckAreaBounds().y);
	const Vector2i bottomRight = Vector2i(std::max(focuses[0].x, focuses[1].x) + boundObject->getMicroBlockCheckAreaBounds().x, std::max(focuses[0].y, focuses[1].y) + boundObject->getMicroBlockCheckAreaBounds().y);
	for (int x = upperLeft.x / size.x; x <= bottomRight.x / size.x; x++)
		for (int y = upperLeft.y / size.y; y <= bottomRight.y / size.y; y++)
			{
				if (sqrt(pow(x * size.x - focuses[0].x, 2) + pow(y * size.x - focuses[0].y, 2)) + sqrt(pow(x * size.x - focuses[1].x, 2) + pow(y * size.x - focuses[1].y, 2)) <= Helper::getDist(focuses[0], focuses[1]) * boundObject->ellipseSizeMultipliers[0])
				{
					filedFigure.setPosition((x * size.x - cameraPosition.x - size.x / 2.0f) * scaleFactor + Helper::GetScreenSize().x / 2, (y * size.y - cameraPosition.y - size.y / 2.0f) * scaleFactor + Helper::GetScreenSize().y / 2);
					window->draw(filedFigure);
				}
			}
	for (int i = 0; i < focuses.size(); i++)
	{
		focusFigures[i].setPosition((focuses[i].x - cameraPosition.x - size.x) * scaleFactor + Helper::GetScreenSize().x / 2, (focuses[i].y - cameraPosition.y - size.y) * scaleFactor + Helper::GetScreenSize().y / 2);
		window->draw(focusFigures[i]);
	}
}

