#include "terrain_object.h"

#include "helper.h"

terrain_object::terrain_object(std::string objectName, Vector2f centerPosition) : static_object(std::move(objectName), centerPosition)
{
	isTerrain = true;
	mirrored = bool(rand() % 2);
}

terrain_object::~terrain_object()
= default;

void terrain_object::initMicroBlocks()
{	
	const auto currentMicroBlock = Vector2i(int(round(position.x / microBlockSize.x)), int(round(position.y / microBlockSize.y)));
	if (mirrored)
		textureBoxOffset.x = conditionalSizeUnits.x - textureBoxOffset.x;
	
	if (isMultiEllipse)
	{
		for (auto i = 0u; i < internalEllipses.size(); i++)
		{
			microBlockCheckAreaBounds.x += getEllipseSize(i) / 2;
			microBlockCheckAreaBounds.y += getEllipseSize(i) / 2;
		}
		
		const auto i_end = int(microBlockCheckAreaBounds.x / microBlockSize.x);
		const auto j_end = int(microBlockCheckAreaBounds.y / microBlockSize.y);
		
		for (auto cnt = 0u; cnt < internalEllipses.size(); cnt++)
		{
			auto i = int(-microBlockCheckAreaBounds.x / microBlockSize.x);
			while (i < i_end)
			{
				auto j = int(-microBlockCheckAreaBounds.y / microBlockSize.y);
				while (j <= j_end)
				{
					const auto pos = Vector2f(position.x + i * microBlockSize.x, position.y + j * microBlockSize.y);
					auto const f1 = internalEllipses[cnt].first;
					auto const f2 = internalEllipses[cnt].second;
					if (helper::getDist(pos, f1) + helper::getDist(pos, f2) < getEllipseSize(cnt) - sqrt(2.0f * microBlockSize.x) * 1.2f)
						lockedMicroBlocks.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
					j++;
				}
				i++;
			}
		}
	}
	else
	{
		microBlockCheckAreaBounds = { getEllipseSize() / 2.0f, getEllipseSize() / 2.0f };

		const auto i_end = microBlockCheckAreaBounds.x / microBlockSize.x;
		const auto j_end = microBlockCheckAreaBounds.y / microBlockSize.y;

		auto i = int(-microBlockCheckAreaBounds.x / microBlockSize.x);
		while (i <= i_end)
		{
			auto j = int(-microBlockCheckAreaBounds.y / microBlockSize.y);
			while (j <= j_end)
			{
				const auto pos = Vector2f(position.x + i * microBlockSize.x, position.y + j * microBlockSize.y);
				auto const f1 = this->getFocus1();
				auto const f2 = this->getFocus2();
				if (helper::getDist(pos, f1) + helper::getDist(pos, f2) < this->getEllipseSize() - sqrt(2.0f * microBlockSize.x) * 1.2f)
					lockedMicroBlocks.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
				j++;
			}
			i++;
		}
	}
}

float terrain_object::getEllipseSize(int i)
{
	if (isMultiEllipse)
		return helper::getDist(internalEllipses[i].first, internalEllipses[i].second) * ellipseSizeMultipliers[i];

	return helper::getDist(focus1, focus2) * ellipseSizeMultipliers[0];
}

void terrain_object::setFocuses(std::vector<Vector2f> focuses)
{
	if (isMultiEllipse)
	{
		for (auto i = 0u; i < focuses.size() / 2; i++)
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

bool terrain_object::isIntersected(Vector2f curPosition, Vector2f newPosition) //const
{
	if (this->isMultiEllipse)
		return false;

	if (this->isDotsAdjusted)
	{
		const auto d1 = this->getDot1();
		const auto d2 = this->getDot2();

		auto const triangle1 = helper::triangleArea(d1.x, d1.y, curPosition.x, curPosition.y, d2.x, d2.y);
		auto const triangle2 = helper::triangleArea(d1.x, d1.y, newPosition.x, newPosition.y, d2.x, d2.y);

		return (!helper::checkSigns(triangle1, triangle2)) &&
			((newPosition.x >= std::min(d1.x, d2.x) - 5 && newPosition.x <= std::max(d1.x, d2.x) + 5)
				&& (newPosition.y >= std::min(d1.y, d2.y) - 5 && newPosition.y <= std::max(d1.y, d2.y) + 5));
	}

	auto const f1 = this->getFocus1();
	auto const f2 = this->getFocus2();

	auto const position = newPosition;

	return sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)) <= helper::getDist(f1, f2) * ellipseSizeMultipliers[0];
}

std::vector<int> terrain_object::getMultiellipseIntersect(Vector2f position) const
{
	std::vector<int> ans;
	if (this->internalEllipses.empty())	
		return ans;

	for (auto i = 0u; i < this->internalEllipses.size(); i++)
	{
		auto const f1 = this->internalEllipses[i].first;
		auto const f2 = this->internalEllipses[i].second;

		if (sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)/* - dynamic.radius*/) <= helper::getDist(f1, f2) * ellipseSizeMultipliers[i])
			ans.push_back(i);
	}

	return ans;
}

Vector2f terrain_object::newSlippingPositionForDotsAdjusted(const Vector2f motionVector, const float speed, const long long elapsedTime) const
{
	const auto dot1 = Vector2f(this->getDot1()), dot2 = Vector2f(this->getDot2());

	const auto directiveVector = Vector2f((dot2.x - dot1.x), (dot2.y - dot1.y));

	const auto motionVectorLength = sqrt(pow(motionVector.x, 2) + pow(motionVector.y, 2)),
	           directiveVectorLength = sqrt(pow(directiveVector.x, 2) + pow(directiveVector.y, 2));

	const auto cosAlpha = (motionVector.x * directiveVector.x + motionVector.y * directiveVector.y) /
		(motionVectorLength * directiveVectorLength);

	const auto k = (speed * float(elapsedTime)) / sqrt(pow(directiveVector.x, 2) + pow(directiveVector.y, 2));

	return { directiveVector.x * cosAlpha * k, directiveVector.y * cosAlpha * k };
}
