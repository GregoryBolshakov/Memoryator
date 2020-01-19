#include "TerrainObject.h"
#include "Helper.h"

TerrainObject::TerrainObject(std::string objectName, Vector2f centerPosition) : StaticObject(objectName, centerPosition)
{	
	isTerrain = true;
	if (!isMultiellipse)
		mirrored = bool(rand() % 2);
}

TerrainObject::~TerrainObject()
{

}

void TerrainObject::initMicroBlocks()
{	
	const Vector2i currentMicroBlock = Vector2i(position.x / microBlockSize.x, position.y / microBlockSize.y);
	if (mirrored)
		textureBoxOffset.x = conditionalSizeUnits.x - textureBoxOffset.x;
	
	if (isMultiellipse)
	{
		for (int i = 0; i < internalEllipses.size(); i++)
		{
			microBlockCheckAreaBounds.x += getEllipseSize(i) / 2;
			microBlockCheckAreaBounds.y += getEllipseSize(i) / 2;
		}
		for (int cnt = 0; cnt < internalEllipses.size(); cnt++)
		{
			for (int i = -microBlockCheckAreaBounds.x / microBlockSize.x; i <= int(microBlockCheckAreaBounds.x / microBlockSize.x); i++)
				for (int j = -microBlockCheckAreaBounds.y / microBlockSize.y; j <= int(microBlockCheckAreaBounds.y / microBlockSize.y); j++)
				{
					Vector2f pos = Vector2f(position.x + i * microBlockSize.x, position.y + j * microBlockSize.y);
					auto const f1 = internalEllipses[cnt].first;
					auto const f2 = internalEllipses[cnt].second;
					if (Helper::getDist(pos, f1) + Helper::getDist(pos, f2) < getEllipseSize(cnt) - sqrt(2 * microBlockSize.x) * 1.2)
						lockedMicroBlocks.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
				}
		}	
	}
	else
	{
		microBlockCheckAreaBounds = Vector2i(getEllipseSize() / 2, getEllipseSize() / 2);
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

int TerrainObject::getEllipseSize(int i)
{
	if (isMultiellipse)
		return Helper::getDist(internalEllipses[i].first, internalEllipses[i].second) * ellipseSizeMultipliers[i];

	return Helper::getDist(focus1, focus2) * ellipseSizeMultipliers[0];
}

void TerrainObject::setFocuses(std::vector<Vector2f> focuses)
{
	if (isMultiellipse)
	{
		for (int i = 0; i < focuses.size() / 2; i++)
		{
			internalEllipses[i].first = focuses[i * 2];
			internalEllipses[i].second = focuses[i * 2 + 1];
		}
	}
	else
	{
		focus1 = focuses[0];
		focus2 = focuses[1];
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

	return sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)) <= Helper::getDist(f1, f2) * ellipseSizeMultipliers[0];
}

std::vector<int> TerrainObject::getMultiellipseIntersect(Vector2f position) const
{
	std::vector<int> ans;
	if (this->internalEllipses.empty())	
		return ans;

	for (int i = 0; i < this->internalEllipses.size(); i++)
	{
		auto const f1 = this->internalEllipses[i].first;
		auto const f2 = this->internalEllipses[i].second;

		if (sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)/* - dynamic.radius*/) <= Helper::getDist(f1, f2) * ellipseSizeMultipliers[i])
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