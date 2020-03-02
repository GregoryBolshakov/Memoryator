#include "log.h"

#include "helper.h"

namespace mr
{
	log::log(std::string objectName, const Vector2f centerPosition, const int typeOfObject) : terrain_object(std::move(objectName), centerPosition)
	{
		varietyOfTypes = 3;
		this->typeOfObject = typeOfObject;
		strength = 0;
		radius = 50;
		animationSpeed = 10;
		toSaveName = "log";
		log::setType(typeOfObject);
		tag = entity_tag::log;
	}

	void log::setType(const int typeOfObject)
	{
		this->typeOfObject = typeOfObject;
		if (typeOfObject == 1)
			conditionalSizeUnits = { 687, 179 };
		if (typeOfObject == 2)
			conditionalSizeUnits = { 507, 231 };
		if (typeOfObject == 3)
			conditionalSizeUnits = { 509, 246 };
	}

	Vector2f log::calculateTextureOffset()
	{
		textureBox.width = textureBox.width * getScaleRatio().x;
		textureBox.height = textureBox.height * getScaleRatio().y;

		switch (typeOfObject)
		{
		case 1:
			return { textureBox.width * 0.492f, textureBox.height * 0.721f };
		case 2:
			return { textureBox.width * 0.479f, textureBox.height * 0.735f };
		case 3:
			return { textureBox.width * 0.502f, textureBox.height * 0.584f };
		default:
			return { textureBox.width * 0.485f, textureBox.height * 0.591f };
		}
	}

	void log::initPedestal()
	{
		std::pair<Vector2f, Vector2f> microEllipse;
		switch (typeOfObject)
		{
		case 1:
			this->isMultiEllipse = true;
			focus1 = Vector2f(position.x, position.y);
			focus2 = Vector2f(position.x, position.y);

			microEllipse.first = Vector2f(position.x - textureBox.width * 0.42f, position.y + textureBox.height * 0.15f);
			microEllipse.second = Vector2f(position.x + textureBox.width * 0.073f, position.y - textureBox.height * 0.26f);
			internalEllipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position.x - textureBox.width * 0.107f, position.y - textureBox.height * 0.197f);
			microEllipse.second = Vector2f(position.x + textureBox.width * 0.169f, position.y - textureBox.height * 0.197f);
			internalEllipses.push_back(microEllipse);

			microEllipse.first = Vector2f(position.x + textureBox.width * 0.033f, position.y - textureBox.height * 0.2f);
			microEllipse.second = Vector2f(position.x + textureBox.width * 0.407f, position.y + textureBox.height * 0.103f);
			internalEllipses.push_back(microEllipse);
			ellipseSizeMultipliers = { 1.03f, 1.11f, 1.05f };
			break;
		case 2:
			focus1 = Vector2f(position.x - textureBox.width * 0.399f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.399f, position.y);
			ellipseSizeMultipliers[0] = { 1.07f };
			break;
		case 3:
			focus1 = Vector2f(position.x - textureBox.width * 0.396f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.396f, position.y);
			ellipseSizeMultipliers[0] = { 1.12f };
			break;
		default:
			focus1 = Vector2f(position.x - textureBox.width * 0.311f, position.y);
			focus2 = Vector2f(position.x + textureBox.width * 0.311f, position.y);
			ellipseSizeMultipliers[0] = { 1.307f };
			break;
		}
		initMicroBlocks();
	}

	Vector2f log::getBuildPosition(std::vector<world_object*>, float, Vector2f)
	{
		return { -1, -1 };
	}

	int log::getBuildType(Vector2f, Vector2f)
	{
		return 1;
	}

	std::vector<sprite_chain_element*> log::prepareSprites(long long)
	{
		const auto body = new sprite_chain_element(pack_tag::birchGrove, pack_part::log, direction::DOWN, typeOfObject, position, conditionalSizeUnits, Vector2f(textureBoxOffset), color, mirrored);

		return { body };
	}
}