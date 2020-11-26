#include "picked_object.h"
#include "hero_bag.h"
#include "tags.h"

picked_object::picked_object(std::string objectName, const sf::Vector2f centerPosition) : terrain_object(std::move(objectName), centerPosition), id(), count(0)
{
	radius_ = 0;
}

bool picked_object::pickUp(std::vector<hero_bag>& bags)
{
	if (this->tag == entity_tag::emptyCell)
		return true;
	while (true)
	{
		cell *maximumFilledCell = nullptr;

		for (auto& bag : bags)
		{
			for (auto& cell : bag.cells)
			{
				if (maximumFilledCell == nullptr)
				{
					if (cell.content.first == entity_tag::emptyCell || entity_tag(cell.content.first) == id && cell.content.second < hero_bag::items_max_count.at(cell.content.first))
						maximumFilledCell = &cell;
				}
				else
					if (entity_tag(cell.content.first) == id && cell.content.second > maximumFilledCell->content.second && cell.content.second < hero_bag::items_max_count.at(cell.content.first))
						maximumFilledCell = &cell;
			}
		}
		if (maximumFilledCell != nullptr)
		{
			maximumFilledCell->content.first = id;
			maximumFilledCell->content.second += count;
			if (maximumFilledCell->content.second > hero_bag::items_max_count.at(entity_tag(id)))
			{
				count = maximumFilledCell->content.second % hero_bag::items_max_count.at(entity_tag(id));
				maximumFilledCell->content.second = hero_bag::items_max_count.at(entity_tag(id));
			}
			else
			{
				count = 0;
				id = entity_tag::emptyCell;
				delete_promise_on();
				return true;
			}
		}
		else
			return false;
	}
}

picked_object::~picked_object()
= default;
