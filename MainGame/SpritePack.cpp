#include "SpritePack.h"

std::map<std::string, PackTag> SpritePack::mappedPackTag = { {"hero_absorb", PackTag::hero_absorb}, {"hero_hit", PackTag::hero_hit}, {"hero_move", PackTag::hero_move},
	{"hero_pick", PackTag::hero_pick}, {"hero_roll", PackTag::hero_roll}, {"hero_stand", PackTag::hero_stand}, {"hero_throw", PackTag::hero_throw} };

SpritePack::SpritePack()
{
}

SpritePack::~SpritePack()
{
}

void SpritePack::init(std::string path)
{
	
}

Sprite SpritePack::getSprite(PackPart part, Direction direction, int number)
{
	return Sprite();
}
