#pragma once

#include "drawable_chain_element.h"

enum class direction;

enum class pack_tag
{
	empty = 0, heroAbsorb = 1, heroHit = 2, heroMove = 3, heroPick = 4, heroRoll = 5, heroStand = 6, heroThrow = 7,
	interfaceElements = 8, locations = 9, darkWoods = 10, birchGrove = 11, swampyTrees = 12, craftObjects = 13,
	inventory = 14, icons = 15, book = 16, hare = 17, nightmare1 = 18, nightmare2Stand = 20,
	nightmare2Move = 21, nightmare2Hit = 22, owl = 24, deer = 25, deerStand = 26
};

enum class pack_part
{
	full = 101, body = 102, legs = 103, lines = 104, // creature's parts
	stand = 1, move = 2, walk = 3, hit = 4, hunt = 5, listening = 6, sniff = 7,
	transition = 8, jump = 9, look_around = 10, startle = 11, flap = 12, head = 13,
	landing = 14, soar = 15, takeoff = 16, stop = 17, trap = 18, bend_down = 19, bend_up = 20, // action's parts
	ground = 201, tree = 202, bush = 203, log = 204, mushroom = 205, path = 206, plant = 207, rock = 208, stump = 209,
	decor = 210, roof = 211, lake = 212, root = 213, flower = 213,// biome's parts
	bookLittle = 301, crossButton = 302, menu = 303, // interface's parts
	brazier = 401, wreathTable = 402, // location's parts
	cageBear = 501, cageOwl = 502, fence = 503, hareTrap = 504, noose = 505, totem = 506, // craftObject's parts
	areas = 601, bag1 = 602, // inventory's parts
	craftObjects = 701, flowers = 702, mobs = 703, notCraftObjects = 704, // icon's parts
	arrowBig = 801, arrowSmall = 802, bookmarks = 803, cover = 804, frame1 = 805, frame2 = 806, pages = 807, plus = 808, cell = 809 // book's parts
};

class sprite_chain_element : public drawable_chain_element
{
public:
	sprite_chain_element();
	sprite_chain_element(sprite_chain_element& element);
	sprite_chain_element(
	    pack_tag tag,
	    pack_part part,
	    direction direction,
	    int number = 1,
	    sf::Vector2f position = {0, 0},
	    sf::Vector2f size = {0, 0},
	    sf::Vector2f offset = {0, 0},
		sf::Color color = sf::Color(255, 255, 255, 255),
		bool mirrored = false,
		bool isometric = false,
		float rotation = 0);
	sprite_chain_element(
		sf::Vector2f position,
		sf::Vector2f size,
		sf::Vector2f offset,
		sf::Color color = sf::Color(255, 255, 255, 255),
		bool mirrored = false,
		bool isometric = false,
		float rotation = 0);
	sprite_chain_element(sprite_chain_element&& element) noexcept : drawable_chain_element(std::move(element))
	{
		this->pack_tag = element.pack_tag;
		this->pack_part = element.pack_part;
		this->direction = element.direction;
		this->number = element.number;
		this->animation_length = element.animation_length;
		this->z_coordinate = element.z_coordinate;
		this->finish_sprite = element.finish_sprite;
		this->is_background = element.is_background;
		this->mirrored = element.mirrored;
		this->anti_transparent = element.anti_transparent;
		this->isometric = element.isometric;
	}
	sprite_chain_element operator =(const sprite_chain_element&);
	virtual ~sprite_chain_element() = default;
	void set_draw_info(pack_tag tag, pack_part part, direction direction, int number = 1);
	//drawable_chain_element* up_cast() { drawable_chain_element* result = this; return result; }

	pack_tag pack_tag;
	pack_part pack_part;
	direction direction;

	int number = 1;
	int animation_length = 1;
	int z_coordinate = 1;
	int finish_sprite = 0;
	bool is_background = false;
	bool mirrored = false;
	bool anti_transparent = false;
	bool isometric = false;
};
