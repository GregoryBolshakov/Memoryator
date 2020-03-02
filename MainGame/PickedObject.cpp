#include "PickedObject.h"


PickedObject::PickedObject(std::string objectName, const Vector2f centerPosition) : TerrainObject(std::move(objectName), centerPosition), id(), count(0)
{
	radius = 0;
}

bool PickedObject::pickUp(std::vector<hero_bag> *bags)
{
	if (this->tag == Tag::emptyCell)
		return true;
	while (true)
	{
		cell *maximumFilledCell = nullptr;

		for (auto& bag : *bags)
		{
			for (auto& cell : bag.cells)
			{
				if (maximumFilledCell == nullptr)
				{
					if (cell.content.first == Tag::emptyCell || Tag(cell.content.first) == id && cell.content.second < hero_bag::itemsMaxCount.at(cell.content.first))
						maximumFilledCell = &cell;
				}
				else
					if (Tag(cell.content.first) == id && cell.content.second > maximumFilledCell->content.second && cell.content.second < hero_bag::itemsMaxCount.at(cell.content.first))
						maximumFilledCell = &cell;					
			}
		}
		if (maximumFilledCell != nullptr)
		{
			maximumFilledCell->content.first = id;
			maximumFilledCell->content.second += count;
			if (maximumFilledCell->content.second > hero_bag::itemsMaxCount.at(Tag(id)))
			{
				count = maximumFilledCell->content.second % hero_bag::itemsMaxCount.at(Tag(id));
				maximumFilledCell->content.second = hero_bag::itemsMaxCount.at(Tag(id));
			}
			else
			{
				count = 0;
				id = Tag::emptyCell;
				deletePromiseOn();
				return true;
			}
		}
		else
			return false;
	}
}

PickedObject::~PickedObject()
= default;
