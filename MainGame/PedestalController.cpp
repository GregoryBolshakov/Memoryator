#include "PedestalController.h"
#include "Helper.h"
#include <fstream>
#include <iomanip>
#include "ObjectInitializer.h"

PedestalController::PedestalController()
{
	filedFigure.setSize(Vector2f(size));
	filedFigure.setFillColor(Color(255, 255, 255, 80));
	focusFigure.setRadius(size.x);
	focusFigure.setFillColor(Color(255, 71, 71, 180));
	centerFigure.setRadius(size.x);
	centerFigure.setFillColor(Color(60, 80, 176, 180));
	lastMousePos = Vector2f(Mouse::getPosition());
}

PedestalController::~PedestalController()
{
}

void PedestalController::start(TerrainObject* object)
{
	readyToStart = false;
	boundObject = object;
	focuses.clear();
	if (boundObject->isMultiellipse)	
		for (auto& internalEllipse : boundObject->internalEllipses)
		{
			focuses.push_back(internalEllipse.first);
			focuses.push_back(internalEllipse.second);
		}	
	else
	{
		focuses.push_back(boundObject->getFocus1());
		focuses.push_back(boundObject->getFocus2());
	}
	centerPosition = boundObject->getPosition();
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
	
	Vector2f offset = { (boundObject->getTextureOffset().x + centerPosition.x - boundObject->getPosition().x) / boundObject->getTextureSize().x,
		(boundObject->getTextureOffset().y + centerPosition.y - boundObject->getPosition().y) / boundObject->getTextureSize().y };
	std::ofstream fout("Game/pedestalsInfo.txt");
	fout.clear();
	fout << std::setprecision(3) << "Tag: " << ObjectInitializer::mappedTags.at(boundObject->tag) + " " << '\n';
	fout << std::setprecision(3) << "Type: " << boundObject->getType() << '\n';
	fout << std::setprecision(3) << "Mirrored: " << boundObject->getMirroredState() << '\n';
	if (boundObject->getMirroredState())
		fout << std::setprecision(3) << "OffsetX: " << 1 - offset.x << " " << "OffsetY: " << offset.y << '\n' << '\n';		
	else
		fout << std::setprecision(3) << "OffsetX: " << offset.x << " " << "OffsetY: " << offset.y << '\n' << '\n';
	for (int i = 0; i < focuses.size() / 2; i++)
	{
		fout << std::setprecision(3) << "Focus" << i * 2 << " X: " << (focuses[i * 2].x - centerPosition.x) / boundObject->getTextureSize().x << " " << "Focus" << i * 2 << " Y: " << (focuses[i * 2].y - centerPosition.y) / boundObject->getTextureSize().y << '\n';
		fout << std::setprecision(3) << "Focus" << i * 2 + 1 << " X: " << (focuses[i * 2 + 1].x - centerPosition.x) / boundObject->getTextureSize().x << " " << "Focus" << i * 2 + 1 << " Y: " << (focuses[i * 2 + 1].y - centerPosition.y) / boundObject->getTextureSize().y << '\n';
		fout << std::setprecision(3) << "Ellipse size: " << boundObject->ellipseSizeMultipliers[i] << '\n' << '\n';
	}
	fout.close();
}

void PedestalController::handleEvents(Event& event)
{
	if (!boundObject)
		return;
	// double click to reset center
	if (event.type == Event::MouseButtonReleased)
	{
		if (doubleClickTimer <= 2e5)
		{
			if (boundObject->isMultiellipse)
				centerPosition = boundObject->getPosition();
			else
				centerPosition = { (focuses[0].x + focuses[1].x) / 2, (focuses[0].y + focuses[1].y) / 2 };
		}
		doubleClickTimer = 0;

		selectedEllipse = -1;
	}	
	//------------------------------
}

void PedestalController::interact(float elapsedTime, Event event)
{
	this->elapsedTime = elapsedTime;
	if (!boundObject || !running)
		return;

	// moving focuses
	const Vector2f mousePos = Vector2f(Mouse::getPosition());
	const Vector2f worldMousePos = Vector2f((mousePos.x - Helper::GetScreenSize().x / 2 + cameraPosition.x * scaleFactor) / scaleFactor,
		(mousePos.y - Helper::GetScreenSize().y / 2 + cameraPosition.y*scaleFactor) / scaleFactor);

	// selecting focus to move
	if (!selectedCenter && selectedFocus == -1 && (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)))
	{
		for (int i = 0; i < focuses.size(); i++)
			if (Helper::getDist(worldMousePos, focuses[i]) <= focusFigure.getRadius())
			{
				selectedFocus = i;
				break;
			}
	}
	else
		if (!(Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)))
			selectedFocus = -1;
	//------------------------

	// moving selected focus
	if (selectedFocus != -1)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			focuses[selectedFocus].x = worldMousePos.x;
			focusFigure.setPosition(mousePos.x, focusFigure.getPosition().y);
		}
		if (Mouse::isButtonPressed(Mouse::Right))
		{
			focuses[selectedFocus].y = worldMousePos.y;
			focusFigure.setPosition(focusFigure.getPosition().x, mousePos.y);
		}
		// alignment
		if (selectedFocus % 2 == 0 && abs(focuses[selectedFocus].x - focuses[selectedFocus + 1].x) < size.x / 2.0f)
			focuses[selectedFocus].x = focuses[selectedFocus + 1].x;
		if (selectedFocus % 2 == 0 && abs(focuses[selectedFocus].y - focuses[selectedFocus + 1].y) < size.y / 2.0f)
			focuses[selectedFocus].y = focuses[selectedFocus + 1].y;
		if (selectedFocus % 2 == 1 && abs(focuses[selectedFocus].x - focuses[selectedFocus - 1].x) < size.x / 2.0f)
			focuses[selectedFocus].x = focuses[selectedFocus - 1].x;
		if (selectedFocus % 2 == 1 && abs(focuses[selectedFocus].y - focuses[selectedFocus - 1].y) < size.y / 2.0f)
			focuses[selectedFocus].y = focuses[selectedFocus - 1].y;
		//----------
	}
	//----------------------

	// selecting center
	if (selectedFocus == -1 && !selectedCenter && (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)))
	{		
		if (Helper::getDist(worldMousePos, centerPosition) <= focusFigure.getRadius())
			selectedCenter = true;
	}
	else
		if (!(Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)))
			selectedCenter = false;
	//-----------------

	// moving center
	if (selectedCenter)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			centerPosition.x = worldMousePos.x;
			centerFigure.setPosition(mousePos.x, centerFigure.getPosition().y);
		}
		if (Mouse::isButtonPressed(Mouse::Right))
		{
			centerPosition.y = worldMousePos.y;
			centerFigure.setPosition(centerFigure.getPosition().x, mousePos.y);
		}
	}
	//----------------------

	// resize ellipse
	if (Mouse::isButtonPressed(Mouse::Middle))
	{
		if (selectedEllipse == -1)
		{
			float minDist = 1e5;
			for (int i = 0; i < focuses.size() / 2; i++)
			{
				if (Helper::getDist(worldMousePos, focuses[i * 2]) + Helper::getDist(worldMousePos, focuses[i * 2 + 1]) <= minDist)
				{
					minDist = Helper::getDist(worldMousePos, focuses[i * 2]) + Helper::getDist(worldMousePos, focuses[i * 2 + 1]);
					selectedEllipse = i;
				}
			}
		}
		const Vector2f center = Vector2f((focuses[selectedEllipse * 2].x + focuses[selectedEllipse * 2 + 1].x) / 2,
		                           (focuses[selectedEllipse * 2].y + focuses[selectedEllipse * 2 + 1].y) / 2);
		boundObject->ellipseSizeMultipliers[selectedEllipse] += boundObject->ellipseSizeMultipliers[selectedEllipse] * (Helper::getDist(mousePos, center) - Helper::getDist(lastMousePos, center)) / 100;
	}
	lastMousePos = mousePos;
	//---------------

	doubleClickTimer += elapsedTime;
	boundObject->setFocuses(focuses);
}

void PedestalController::draw(RenderWindow* window, Vector2f cameraPosition, float scaleFactor)
{
	this->cameraPosition = cameraPosition;
	this->scaleFactor = scaleFactor;
	if (!boundObject || !running)
		return;

	Vector2f areaBounds = { 0, 0 };
	for (int i = 0; i < focuses.size() / 2; i++)
	{
		areaBounds.x += Helper::getDist(focuses[i * 2], focuses[i * 2 + 1]) * boundObject->ellipseSizeMultipliers[i] / 2;
		areaBounds.y += Helper::getDist(focuses[i * 2], focuses[i * 2 + 1]) * boundObject->ellipseSizeMultipliers[i] / 2;
	}
	const Vector2i upperLeft = Vector2i(std::min(focuses[0].x, focuses[1].x) - areaBounds.x, std::min(focuses[0].y, focuses[1].y) - areaBounds.y);
	const Vector2i bottomRight = Vector2i(std::max(focuses[0].x, focuses[1].x) + areaBounds.x, std::max(focuses[0].y, focuses[1].y) + areaBounds.y);
	for (int x = upperLeft.x / size.x; x <= bottomRight.x / size.x; x++)
		for (int y = upperLeft.y / size.y; y <= bottomRight.y / size.y; y++)
			for (auto i = 0; i < focuses.size() / 2; i++)
			{			
				if (sqrt(pow(x * size.x - focuses[i * 2].x, 2) + pow(y * size.x - focuses[i * 2].y, 2)) + sqrt(pow(x * size.x - focuses[i * 2 + 1].x, 2) + pow(y * size.x - focuses[i * 2 + 1].y, 2)) <= Helper::getDist(focuses[i * 2], focuses[i * 2 + 1]) * boundObject->ellipseSizeMultipliers[i])
				{
					filedFigure.setPosition((x * size.x - cameraPosition.x - size.x / 2.0f) * scaleFactor + Helper::GetScreenSize().x / 2, (y * size.y - cameraPosition.y - size.y / 2.0f) * scaleFactor + Helper::GetScreenSize().y / 2);
					window->draw(filedFigure);
				}
			}
	for (auto& focus : focuses)
	{
		focusFigure.setPosition((focus.x - cameraPosition.x - size.x) * scaleFactor + Helper::GetScreenSize().x / 2, (
			                        focus.y - cameraPosition.y - size.y) * scaleFactor + Helper::GetScreenSize().y / 2);
		window->draw(focusFigure);
	}
	centerFigure.setPosition((centerPosition.x - cameraPosition.x - size.x) * scaleFactor + Helper::GetScreenSize().x / 2, (
		centerPosition.y - cameraPosition.y - size.y) * scaleFactor + Helper::GetScreenSize().y / 2);
	window->draw(centerFigure);
}

