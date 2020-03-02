#include "ground_connection.h"

#include "helper.h"

ground_connection::ground_connection(std::string object_name, const Vector2f center_position, const int type_of_object) : static_object(std::move(object_name), center_position)
{
	varietyOfTypes = 12; // SwampyTrees: 1-4; DarkWoods: 5-8; BirchGrove: 9-12
	this->typeOfObject = type_of_object;
	isBackground = true;
	toSaveName = "groundConnection";
	ground_connection::setType(type_of_object);
	tag = entity_tag::groundConnection;
}

void ground_connection::setType(const int type_of_object)
{
	if (type_of_object == -1)
		return;

	this->typeOfObject = type_of_object;
	if (type_of_object % 4 == 1)
	{
		this->conditionalSizeUnits = { 1000, 250 };
	}
	else
		if (type_of_object % 4 == 2)
		{
			this->conditionalSizeUnits = { 1000, 250 };
		}
		else
			if (type_of_object % 4 == 3)
			{
				this->conditionalSizeUnits = { 250, 1000 };
			}
			else
				if (type_of_object % 4 == 0)
				{
					this->conditionalSizeUnits = { 250, 1000 };
				}

	if (type_of_object >= 1 && type_of_object <= 4)
		this->zCoord = 10;
	else
		if (type_of_object >= 5 && type_of_object <= 8)
			this->zCoord = 20;
		else
			if (type_of_object >= 9 && type_of_object <= 12)
				this->zCoord = 30;
			else
				if (type_of_object >= 13 && type_of_object <= 16)
					this->zCoord = 40;
				else
					if (type_of_object >= 17 && type_of_object <= 20)
						this->zCoord = 50;
}

Vector2f ground_connection::calculateTextureOffset()
{
	if (typeOfObject % 4 == 1)
		return { 0, textureBox.height - 5 };
	if (typeOfObject % 4 == 2)
		return { 0, 5 };
	if (typeOfObject % 4 == 3)
		return { textureBox.width - 5, 0 };
	if (typeOfObject % 4 == 0)
		return { 5, 0 };
	return { 0, 0 };
}

Vector2f ground_connection::getBuildPosition(std::vector<world_object*>, float, Vector2f)
{
	return { -1, -1 };
}

int ground_connection::getBuildType(Vector2f, Vector2f)
{
	return 1;
}

std::vector<sprite_chain_element*> ground_connection::prepareSprites(long long)
{
	if (typeOfObject >= 1 && typeOfObject <= 4)
		return { new sprite_chain_element() };

	int spriteType = typeOfObject % 4 + 1;
	if (spriteType == 1)
		spriteType = 5;

	auto body = new sprite_chain_element(pack_tag::darkWoods, pack_part::ground, direction::DOWN, spriteType, position, conditionalSizeUnits, Vector2f(textureBoxOffset));

	body->z_coordinate = zCoord;

	//if (typeOfObject >= 1 && typeOfObject <= 4)
		//body->packTag = PackTag::darkWoods;
	if (typeOfObject >= 5 && typeOfObject <= 8)
		body->pack_tag = pack_tag::darkWoods;
	if (typeOfObject >= 9 && typeOfObject <= 12)
		body->pack_tag = pack_tag::birchGrove;

	body->is_background = true;
	return { body };
}