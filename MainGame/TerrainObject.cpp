#include "TerrainObject.h"
#include "Helper.h"

TerrainObject::TerrainObject(std::string objectName, Vector2f centerPosition) : StaticObject(objectName, centerPosition)
{	
	isTerrain = true;
	mirrored = bool(rand() % 2);
}

TerrainObject::~TerrainObject()
{

}

void TerrainObject::initMicroBlocks()
{
	const Vector2i currentMicroBlock = Vector2i(position.x / microBlockSize.x, position.y / microBlockSize.y);
	if (isMultiellipse)
	{
		for (auto& ellipse : internalEllipses)
		{
			microBlockCheckAreaBounds.x += ellipse.first.first;
			microBlockCheckAreaBounds.y += ellipse.first.first;
		}
		for (auto& ellipse : internalEllipses)
		{
			for (int i = -microBlockCheckAreaBounds.x / microBlockSize.x; i <= int(microBlockCheckAreaBounds.x / microBlockSize.x); i++)
				for (int j = -microBlockCheckAreaBounds.y / microBlockSize.y; j <= int(microBlockCheckAreaBounds.y / microBlockSize.y); j++)
				{
					Vector2f pos = Vector2f(position.x + i * microBlockSize.x, position.y + j * microBlockSize.y);
					auto const f1 = ellipse.second.first;
					auto const f2 = ellipse.second.second;
					if (Helper::getDist(pos, f1) + Helper::getDist(pos, f2) < ellipse.first.first - sqrt(2 * microBlockSize.x) * 1.2)
						lockedMicroBlocks.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
				}
		}	
	}
	else
	{		
		microBlockCheckAreaBounds = Vector2i(ellipseSize, ellipseSize);
		for (int i = -microBlockCheckAreaBounds.x / microBlockSize.x; i <= int(microBlockCheckAreaBounds.x / microBlockSize.x); i++)
			for (int j = -microBlockCheckAreaBounds.y / microBlockSize.y; j <= int(microBlockCheckAreaBounds.y / microBlockSize.y); j++)
			{
				Vector2f pos = Vector2f(position.x + i * microBlockSize.x, position.y + j * microBlockSize.y);
				auto const f1 = this->getFocus1();
				auto const f2 = this->getFocus2();
				if (Helper::getDist(pos, f1) + Helper::getDist(pos, f2) < this->getEllipseSize() - sqrt(2 * microBlockSize.x) * 1.2)
					lockedMicroBlocks.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
			}		
	}
}

bool TerrainObject::isIntersected(Vector2f curPosition, Vector2f newPosition) //const
{
	if (this->isMultiellipse)
		return false;

	if (this->isDotsAdjusted)
	{
		auto d1 = this->getDot1();
		auto d2 = this->getDot2();

		auto const triangle1 = Helper::triangleArea(d1.x, d1.y, curPosition.x, curPosition.y, d2.x, d2.y);
		auto const triangle2 = Helper::triangleArea(d1.x, d1.y, newPosition.x, newPosition.y, d2.x, d2.y);

		return (!Helper::checkSigns(triangle1, triangle2)) &&
			((newPosition.x >= std::min(d1.x, d2.x) - 5 && newPosition.x <= std::max(d1.x, d2.x) + 5)
				&& (newPosition.y >= std::min(d1.y, d2.y) - 5 && newPosition.y <= std::max(d1.y, d2.y) + 5));
	}

	auto const f1 = this->getFocus1();
	auto const f2 = this->getFocus2();

	Vector2f const position = newPosition;

	return sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)) <= this->getEllipseSize();
}

std::vector<int> TerrainObject::getMultiellipseIntersect(Vector2f position) const
{
	std::vector<int> ans;
	if (this->internalEllipses.empty())	
		return ans;

	for (int i = 0; i < this->internalEllipses.size(); i++)
	{
		auto const f1 = this->internalEllipses[i].second.first;
		auto const f2 = this->internalEllipses[i].second.second;

		if (sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)/* - dynamic.radius*/) <= this->internalEllipses[i].first.first)
			ans.push_back(i);
	}

	return ans;
}

Vector2f TerrainObject::newSlippingPositionForDotsAdjusted(Vector2f position, Vector2f motionVector, float speed, float elapsedTime)
{
	Vector2f const dynamicPos = position;
	Vector2f dot1 = Vector2f(this->getDot1()), dot2 = Vector2f(this->getDot2());

	Vector2f directiveVector = Vector2f((dot2.x - dot1.x), (dot2.y - dot1.y));

	float motionVectorLength = sqrt(pow(motionVector.x, 2) + pow(motionVector.y, 2)),
	directiveVectorLength = sqrt(pow(directiveVector.x, 2) + pow(directiveVector.y, 2));

	float cosAlpha = (motionVector.x * directiveVector.x + motionVector.y * directiveVector.y) / (motionVectorLength * directiveVectorLength);

	float k = (speed * elapsedTime) / sqrt(pow(directiveVector.x, 2) + pow(directiveVector.y, 2));

	return Vector2f(directiveVector.x * cosAlpha * k, directiveVector.y * cosAlpha * k);
}