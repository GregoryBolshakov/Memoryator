#pragma once
#ifndef SPRITEPACK_H
#define SPRITEPACK_H

#include <nlohmann/json.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "DynamicObject.h"
using json = nlohmann::json;
using Sprite = sf::Sprite;

enum class PackTag { hero_absorb = 1, hero_hit = 2, hero_move = 3, hero_pick = 4, hero_roll = 5, hero_stand = 6, hero_throw = 7 };
enum class PackPart { full = 1, body = 2, legs = 3, loop = 4, rope = 5 };

namespace sprite_pack
{
	struct size
	{
		int w;
		int h;
	};

	struct rect
	{
		int x;
		int y;
		int w;
		int h;
	};

	struct sprite
	{
		rect frame;
		bool rotated;
		bool trimmed;
		rect sprite_source_size;
		size source_size;
	};

	struct pack
	{
		std::vector<sprite> frames;
	};

	void from_json(const json& j, size& s)
	{
		j.at("w").get_to(s.w);
		j.at("h").get_to(s.h);
	}

	void from_json(const json& j, rect& r)
	{
		j.at("x").get_to(r.x);
		j.at("y").get_to(r.y);
		j.at("w").get_to(r.w);
		j.at("h").get_to(r.h);
	}

	void from_json(const json& j, sprite& s)
	{
		j.at("frame").get_to(s.frame);
		j.at("rotated").get_to(s.rotated);
		j.at("trimmed").get_to(s.trimmed);
		j.at("spriteSourceSize").get_to(s.sprite_source_size);
		j.at("sourceSize").get_to(s.source_size);
	}

	void from_json(const json& j, pack& p)
	{
		j.at("frames").get_to(p.frames);
	}
}

class SpritePack
{
public:
	SpritePack();
	~SpritePack();
	void init(std::string path);
	Sprite getSprite(PackPart part, Direction direction, int number);

	PackTag tag;
	static std::map<std::string, PackTag> mappedPackTag;
	std::map<PackPart, std::map<Direction, std::map<int, Texture>>> pack;
};

#endif

