#include "sprite_pack.h"

#include <fstream>

#include "button.h"
#include "helper.h"

const Vector2f sprite_pack::iconSize = { helper::GetScreenSize().y / 13.8f, helper::GetScreenSize().y / 13.8f };
Vector2f sprite_pack::iconWithoutSpaceSize = { 0, 0 };


std::map<std::string, pack_tag> sprite_pack::mappedPackTag = {
    {"empty", pack_tag::empty},
    {"heroAbsorb", pack_tag::heroAbsorb},
    {"heroHit", pack_tag::heroHit},
    {"heroMove", pack_tag::heroMove},
    {"heroPick", pack_tag::heroPick},
    {"heroRoll", pack_tag::heroRoll},
    {"heroStand", pack_tag::heroStand},
    {"heroThrow", pack_tag::heroThrow},
    {"interfaceElements", pack_tag::interfaceElements},
    {"locations", pack_tag::locations},
    {"darkWoods", pack_tag::darkWoods},
    {"birchGrove", pack_tag::birchGrove},
    {"swampyTrees", pack_tag::swampyTrees},
	{"craftObjects", pack_tag::craftObjects},
	{"inventory", pack_tag::inventory},
	{"icons", pack_tag::icons},
	{"book", pack_tag::book},
	{"hare", pack_tag::hare},
	{"nightmare1", pack_tag::nightmare1},
	{"nightmare2Stand", pack_tag::nightmare2Stand},
	{"nightmare2Move", pack_tag::nightmare2Move}, 
	{"nightmare2Hit", pack_tag::nightmare2Hit},
	{"deer", pack_tag::deer}
};

std::map<std::string, pack_part> sprite_pack::mappedPackPart = {
    {"full", pack_part::full},
    {"body", pack_part::body},
    {"legs", pack_part::legs},
    {"lines", pack_part::lines}, // creature's parts
	{"stand", pack_part::stand},
	{"move", pack_part::move}, 
	{"trap", pack_part::trap}, 
	{"hit", pack_part::hit}, 
	{"hunt", pack_part::hunt}, // action's parts
    {"ground", pack_part::ground},
    {"tree", pack_part::tree},
    {"bush", pack_part::bush},
    {"log", pack_part::log},
    {"mushroom", pack_part::mushroom},
    {"path", pack_part::path},
    {"plant", pack_part::plant},
    {"rock", pack_part::rock},
    {"stump", pack_part::stump},
    {"decor", pack_part::decor},
    {"roof", pack_part::roof},
    {"lake", pack_part::lake},
    {"root", pack_part::root},
    {"flower", pack_part::flower}, // biome's parts
    {"bookLittle", pack_part::bookLittle},
    {"crossButton", pack_part::crossButton},
    {"menu", pack_part::menu}, // interface's parts
    {"brazier", pack_part::brazier},
    {"wreathTable", pack_part::wreathTable}, // location's parts
	{"cageBear", pack_part::cageBear},
	{"cageOwl", pack_part::cageOwl},
	{"fence", pack_part::fence}, 
	{"hareTrap", pack_part::hareTrap},
	{"noose", pack_part::noose}, 
	{"totem", pack_part::totem}, // craftObject's parts
	{"areas", pack_part::areas},
	{"bag1", pack_part::bag1}, // inventory's parts
	{"craftObjects", pack_part::craftObjects},
	{"flowers", pack_part::flowers}, 
	{"mobs", pack_part::mobs},
	{"notCraftObjects", pack_part::notCraftObjects}, // icon's parts
	{"arrowBig", pack_part::arrowBig},
	{"arrowSmall", pack_part::arrowSmall},
	{"bookmarks", pack_part::bookmarks},
	{"cover", pack_part::cover}, 
	{"frame1", pack_part::frame1},
	{"frame2", pack_part::frame2}, 
	{"pages", pack_part::pages},
	{"plus", pack_part::plus}, 
	{"cell", pack_part::cell} // book's parts
};

std::map<std::string, Direction> sprite_pack::mappedDirection = { {"up", Direction::UP}, {"up-right", Direction::UPRIGHT}, {"right", Direction::RIGHT}, {"down-right", Direction::DOWNRIGHT},
{"down", Direction::DOWN}, {"down-left", Direction::DOWNLEFT}, {"left", Direction::LEFT}, {"up-left", Direction::UPLEFT} };

void sprite_pack::init(const std::string& path, const std::string& jsonPath, const pack_tag tag)
{
    this->tag = tag;
    this->texture.loadFromFile(path);

    std::ifstream file(jsonPath);
    json jsonFile;
    file >> jsonFile;
    auto sp = jsonFile.get<sprite_pack_structure::pack>();

    for (auto& frame : sp.frames)
    {
        const auto pngExtensionWithDotLength = 4;
        frame.frame_name.erase(frame.frame_name.size() - pngExtensionWithDotLength, pngExtensionWithDotLength);
        auto keyWords = helper::split(frame.frame_name, '/');

        auto part = pack_part::full;
        auto direction = Direction::DOWN;
        auto number = 1;
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

sprite_pack_structure::sprite sprite_pack::getOriginalInfo(const pack_part part, const Direction direction, const int number)
{
	if (pack.count(part) <= 0 || pack.at(part).count(direction) <= 0 || pack.at(part).at(direction).count(number) <= 0)
		return sprite_pack_structure::sprite();

	return pack.at(part).at(direction).at(number);
}

sprite_chain_element* sprite_pack::tagToIcon(const entity_tag object, const bool selected, const int typeOfObject)
{
	auto result = new sprite_chain_element(pack_tag::empty, pack_part::full, Direction::DOWN, 1, { 0, 0 }, iconSize, { iconSize.x / 2, iconSize.y / 2 });

	auto spriteNumber = int(selected);

	switch (object)
	{
	// craftObjects
	case entity_tag::cageBear:
		spriteNumber += 1;
		result->setDrawInfo(pack_tag::icons, pack_part::craftObjects, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::noose:
		spriteNumber += 3;
		result->setDrawInfo(pack_tag::icons, pack_part::craftObjects, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::totem:
		if (typeOfObject == 1)
			spriteNumber += 7;
		if (typeOfObject == 2)
			spriteNumber += 9;
		if (typeOfObject == 3)
			spriteNumber += 11;
		result->setDrawInfo(pack_tag::icons, pack_part::craftObjects, Direction::DOWN, spriteNumber);
		return result;
	//-------------
	// flowers
	case entity_tag::chamomile:
		spriteNumber += 1;
		result->setDrawInfo(pack_tag::icons, pack_part::flowers, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::fern:
		spriteNumber += 3;
		result->setDrawInfo(pack_tag::icons, pack_part::flowers, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::mugwort:
		spriteNumber += 5;
		result->setDrawInfo(pack_tag::icons, pack_part::flowers, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::poppy:
		spriteNumber += 7;
		result->setDrawInfo(pack_tag::icons, pack_part::flowers, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::yarrow:
		spriteNumber += 9;
		result->setDrawInfo(pack_tag::icons, pack_part::flowers, Direction::DOWN, spriteNumber);
		return result;
	//-------
	//mobs
	case entity_tag::bear:
		spriteNumber = 1;
		result->setDrawInfo(pack_tag::icons, pack_part::mobs, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::deer:
		spriteNumber = 2;
		result->setDrawInfo(pack_tag::icons, pack_part::mobs, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::hare:
		spriteNumber = 3;
		result->setDrawInfo(pack_tag::icons, pack_part::mobs, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::nightmare1:
	case entity_tag::nightmare2:
	case entity_tag::nightmare3:
		spriteNumber = 4;
		result->setDrawInfo(pack_tag::icons, pack_part::mobs, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::owlBoss:
		spriteNumber = 5;
		result->setDrawInfo(pack_tag::icons, pack_part::mobs, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::owl:
		spriteNumber = 6;
		result->setDrawInfo(pack_tag::icons, pack_part::mobs, Direction::DOWN, spriteNumber);
		return result;
	case entity_tag::wolf:
		spriteNumber = 7;
		result->setDrawInfo(pack_tag::icons, pack_part::mobs, Direction::DOWN, spriteNumber);
		return result;
	//----
	// notCraftObjects
	case entity_tag::inkyBlackPen:
		spriteNumber = 1;
		result->setDrawInfo(pack_tag::icons, pack_part::notCraftObjects, Direction::DOWN, spriteNumber);
		return result;
	//----------------
	case entity_tag::emptyObject:
		result->setDrawInfo(pack_tag::inventory, pack_part::areas, Direction::DOWN, 1);
		return result;
	}

	return result;
}

Sprite sprite_pack::getSprite(const pack_part part, const Direction direction, const int number, const bool mirrored)
{
    Sprite result;
    if (pack.count(part) <= 0 || pack.at(part).count(direction) <= 0 || pack.at(part).at(direction).count(number) <= 0)
        return result;

    result.setTexture(texture);

    const auto spriteInfo = pack.at(part).at(direction).at(number);
    const Vector2f offset(
		float(spriteInfo.sprite_source_size.x),
		float(spriteInfo.sprite_source_size.y));
    const Vector2f invertedOffset(
		float(spriteInfo.source_size.w - spriteInfo.sprite_source_size.w - spriteInfo.sprite_source_size.x),
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
        result.setOrigin(offset.y + float(spriteInfo.frame.h), -offset.x);
    }
    if (spriteInfo.rotated && mirrored)
    {
        result.setTextureRect(IntRect(spriteInfo.frame.x, spriteInfo.frame.y + spriteInfo.frame.w, spriteInfo.frame.h, -spriteInfo.frame.w));
        result.rotate(-90);
        result.setOrigin(invertedOffset.y + float(spriteInfo.frame.h), -invertedOffset.x);
    }
    result.~Sprite();

    return result;
}
