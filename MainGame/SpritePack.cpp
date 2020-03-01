#include "SpritePack.h"
#include <fstream>
#include "Helper.h"
#include "Button.h"

const Vector2f SpritePack::iconSize = { Helper::GetScreenSize().y / 13.8f, Helper::GetScreenSize().y / 13.8f };
Vector2f SpritePack::iconWithoutSpaceSize = { 0, 0 };


std::map<std::string, PackTag> SpritePack::mappedPackTag = {
    {"empty", PackTag::empty},
    {"heroAbsorb", PackTag::heroAbsorb},
    {"heroHit", PackTag::heroHit},
    {"heroMove", PackTag::heroMove},
    {"heroPick", PackTag::heroPick},
    {"heroRoll", PackTag::heroRoll},
    {"heroStand", PackTag::heroStand},
    {"heroThrow", PackTag::heroThrow},
    {"interfaceElements", PackTag::interfaceElements},
    {"locations", PackTag::locations},
    {"darkWoods", PackTag::darkWoods},
    {"birchGrove", PackTag::birchGrove},
    {"swampyTrees", PackTag::swampyTrees},
	{"craftObjects", PackTag::craftObjects},
	{"inventory", PackTag::inventory},
	{"icons", PackTag::icons},
	{"book", PackTag::book},
	{"hare", PackTag::hare},
	{"nightmare1", PackTag::nightmare1},
	{"nightmare2Stand", PackTag::nightmare2Stand},
	{"nightmare2Move", PackTag::nightmare2Move}, 
	{"nightmare2Hit", PackTag::nightmare2Hit},
	{"deer", PackTag::deer}
};

std::map<std::string, PackPart> SpritePack::mappedPackPart = {
    {"full", PackPart::full},
    {"body", PackPart::body},
    {"legs", PackPart::legs},
    {"lines", PackPart::lines}, // creature's parts
	{"stand", PackPart::stand},
	{"move", PackPart::move}, 
	{"trap", PackPart::trap}, 
	{"hit", PackPart::hit}, 
	{"hunt", PackPart::hunt}, // action's parts
    {"ground", PackPart::ground},
    {"tree", PackPart::tree},
    {"bush", PackPart::bush},
    {"log", PackPart::log},
    {"mushroom", PackPart::mushroom},
    {"path", PackPart::path},
    {"plant", PackPart::plant},
    {"rock", PackPart::rock},
    {"stump", PackPart::stump},
    {"decor", PackPart::decor},
    {"roof", PackPart::roof},
    {"lake", PackPart::lake},
    {"root", PackPart::root},
    {"flower", PackPart::flower}, // biome's parts
    {"bookLittle", PackPart::bookLittle},
    {"crossButton", PackPart::crossButton},
    {"menu", PackPart::menu}, // interface's parts
    {"brazier", PackPart::brazier},
    {"wreathTable", PackPart::wreathTable}, // location's parts
	{"cageBear", PackPart::cageBear},
	{"cageOwl", PackPart::cageOwl},
	{"fence", PackPart::fence}, 
	{"hareTrap", PackPart::hareTrap},
	{"noose", PackPart::noose}, 
	{"totem", PackPart::totem}, // craftObject's parts
	{"areas", PackPart::areas},
	{"bag1", PackPart::bag1}, // inventory's parts
	{"craftObjects", PackPart::craftObjects},
	{"flowers", PackPart::flowers}, 
	{"mobs", PackPart::mobs},
	{"notCraftObjects", PackPart::notCraftObjects}, // icon's parts
	{"arrowBig", PackPart::arrowBig},
	{"arrowSmall", PackPart::arrowSmall},
	{"bookmarks", PackPart::bookmarks},
	{"cover", PackPart::cover}, 
	{"frame1", PackPart::frame1},
	{"frame2", PackPart::frame2}, 
	{"pages", PackPart::pages},
	{"plus", PackPart::plus}, 
	{"cell", PackPart::cell} // book's parts
};

std::map<std::string, Direction> SpritePack::mappedDirection = { {"up", Direction::UP}, {"up-right", Direction::UPRIGHT}, {"right", Direction::RIGHT}, {"down-right", Direction::DOWNRIGHT},
{"down", Direction::DOWN}, {"down-left", Direction::DOWNLEFT}, {"left", Direction::LEFT}, {"up-left", Direction::UPLEFT} };

SpritePack::SpritePack()
= default;

SpritePack::~SpritePack()
= default;

void SpritePack::init(const std::string path, const std::string& jsonPath, PackTag tag)
{
    this->tag = tag;
    this->texture.loadFromFile(path);

    std::ifstream file(jsonPath);
    json jsonFile;
    file >> jsonFile;
    auto sp = jsonFile.get<sprite_pack::pack>();

    for (auto& frame : sp.frames)
    {
        const int pngExtensionWithDotLength = 4;
        frame.frame_name.erase(frame.frame_name.size() - pngExtensionWithDotLength, pngExtensionWithDotLength);
        auto keyWords = Helper::split(frame.frame_name, '/');

        PackPart part = PackPart::full; Direction direction = Direction::DOWN; int number = 1;
        if (!keyWords.empty() && mappedPackPart.count(keyWords[0]) > 0)
        {
            part = mappedPackPart.at(keyWords[0]);
            keyWords.erase(keyWords.begin() + 0);
        }
        if (!keyWords.empty() && mappedDirection.count(keyWords[0]) > 0)
        {
            direction = mappedDirection.at(keyWords[0]);
            keyWords.erase(keyWords.begin() + 0);
        }
        if (!keyWords.empty() && std::stoi(keyWords[0]) >= 1)
            number = std::stoi(keyWords[0]);
        else
            continue;
        pack[part][direction][number] = frame;
    }
}

sprite_pack::sprite SpritePack::getOriginalInfo(PackPart part, Direction direction, int number)
{
	if (pack.count(part) <= 0 || pack.at(part).count(direction) <= 0 || pack.at(part).at(direction).count(number) <= 0)
		return sprite_pack::sprite();

	return pack.at(part).at(direction).at(number);
}

SpriteChainElement* SpritePack::tagToIcon(Tag object, bool selected, int typeOfObject)
{	
	SpriteChainElement* result = new SpriteChainElement(PackTag::empty, PackPart::full, Direction::DOWN, 1, { 0, 0 }, iconSize, { iconSize.x / 2, iconSize.y / 2 });

	int spriteNumber = int(selected);

	switch (object)
	{
	// craftObjects
	case Tag::cageBear:
		spriteNumber += 1;
		result->setDrawInfo(PackTag::icons, PackPart::craftObjects, Direction::DOWN, spriteNumber);
		return result;
	case Tag::noose:
		spriteNumber += 3;
		result->setDrawInfo(PackTag::icons, PackPart::craftObjects, Direction::DOWN, spriteNumber);
		return result;
	case Tag::totem:
		if (typeOfObject == 1)
			spriteNumber += 7;
		if (typeOfObject == 2)
			spriteNumber += 9;
		if (typeOfObject == 3)
			spriteNumber += 11;
		result->setDrawInfo(PackTag::icons, PackPart::craftObjects, Direction::DOWN, spriteNumber);
		return result;
	//-------------
	// flowers
	case Tag::chamomile:
		spriteNumber += 1;
		result->setDrawInfo(PackTag::icons, PackPart::flowers, Direction::DOWN, spriteNumber);
		return result;
	case Tag::fern:
		spriteNumber += 3;
		result->setDrawInfo(PackTag::icons, PackPart::flowers, Direction::DOWN, spriteNumber);
		return result;
	case Tag::mugwort:
		spriteNumber += 5;
		result->setDrawInfo(PackTag::icons, PackPart::flowers, Direction::DOWN, spriteNumber);
		return result;
	case Tag::poppy:
		spriteNumber += 7;
		result->setDrawInfo(PackTag::icons, PackPart::flowers, Direction::DOWN, spriteNumber);
		return result;
	case Tag::yarrow:
		spriteNumber += 9;
		result->setDrawInfo(PackTag::icons, PackPart::flowers, Direction::DOWN, spriteNumber);
		return result;
	//-------
	//mobs
	case Tag::bear:
		spriteNumber = 1;
		result->setDrawInfo(PackTag::icons, PackPart::mobs, Direction::DOWN, spriteNumber);
		return result;
	case Tag::deer:
		spriteNumber = 2;
		result->setDrawInfo(PackTag::icons, PackPart::mobs, Direction::DOWN, spriteNumber);
		return result;
	case Tag::hare:
		spriteNumber = 3;
		result->setDrawInfo(PackTag::icons, PackPart::mobs, Direction::DOWN, spriteNumber);
		return result;
	case Tag::nightmare1:
	case Tag::nightmare2:
	case Tag::nightmare3:
		spriteNumber = 4;
		result->setDrawInfo(PackTag::icons, PackPart::mobs, Direction::DOWN, spriteNumber);
		return result;
	case Tag::owlBoss:
		spriteNumber = 5;
		result->setDrawInfo(PackTag::icons, PackPart::mobs, Direction::DOWN, spriteNumber);
		return result;
	case Tag::owl:
		spriteNumber = 6;
		result->setDrawInfo(PackTag::icons, PackPart::mobs, Direction::DOWN, spriteNumber);
		return result;
	case Tag::wolf:
		spriteNumber = 7;
		result->setDrawInfo(PackTag::icons, PackPart::mobs, Direction::DOWN, spriteNumber);
		return result;
	//----
	// notCraftObjects
	case Tag::inkyBlackPen:
		spriteNumber = 1;
		result->setDrawInfo(PackTag::icons, PackPart::notCraftObjects, Direction::DOWN, spriteNumber);
		return result;
	//----------------
	case Tag::emptyObject:
		result->setDrawInfo(PackTag::inventory, PackPart::areas, Direction::DOWN, 1);
		return result;
	}

	return result;
}

Sprite SpritePack::getSprite(PackPart part, Direction direction, int number, bool mirrored)
{
    Sprite result;
    if (pack.count(part) <= 0 || pack.at(part).count(direction) <= 0 || pack.at(part).at(direction).count(number) <= 0)
        return result;

    result.setTexture(texture);

    const auto spriteInfo = pack.at(part).at(direction).at(number);
    const Vector2f offset(spriteInfo.sprite_source_size.x, spriteInfo.sprite_source_size.y), 
	invertedOffset(spriteInfo.source_size.w - spriteInfo.sprite_source_size.w - spriteInfo.sprite_source_size.x, 
		offset.y);

    if (!spriteInfo.rotated && !mirrored)
    {
        result.setTextureRect(IntRect(spriteInfo.frame.x, spriteInfo.frame.y, spriteInfo.frame.w, spriteInfo.frame.h));
        result.setOrigin(-offset.x, -offset.y);
    }
    if (!spriteInfo.rotated && mirrored)
    {
        result.setTextureRect(IntRect(spriteInfo.frame.x + spriteInfo.frame.w, spriteInfo.frame.y, -spriteInfo.frame.w, spriteInfo.frame.h));
        result.setOrigin(-invertedOffset.x, -invertedOffset.y);
    }
    if (spriteInfo.rotated && !mirrored)
    {
        result.setTextureRect(IntRect(spriteInfo.frame.x, spriteInfo.frame.y, spriteInfo.frame.h, spriteInfo.frame.w));
        result.rotate(-90);
        result.setOrigin(offset.y + spriteInfo.frame.h, -offset.x);
    }
    if (spriteInfo.rotated && mirrored)
    {
        result.setTextureRect(IntRect(spriteInfo.frame.x, spriteInfo.frame.y + spriteInfo.frame.w, spriteInfo.frame.h, -spriteInfo.frame.w));
        result.rotate(-90);
        result.setOrigin(invertedOffset.y + spriteInfo.frame.h, -invertedOffset.x);
    }
    result.~Sprite();

    return result;
}
