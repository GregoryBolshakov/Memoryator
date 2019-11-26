#include "ObjectInitializer.h"
#include "Helper.h"

ObjectInitializer::ObjectInitializer()
{
}


ObjectInitializer::~ObjectInitializer()
{
}

int ObjectInitializer::newNameId = 0;

StaticObject* ObjectInitializer::initializeStaticItem(
	Tag itemClass,
	Vector2f itemPosition,
	int itemType,
	std::string itemName,
	int count,
	std::unordered_map<std::string, BoardSprite>* spriteMap,
	std::vector<std::pair<Tag, int>> inventory)
{
	StaticObject* item = nullptr;

	switch (itemClass)
	{
		case Tag::tree:
		{
			item = new ForestTree("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::grass:
		{
			item = new Grass("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::spawn:
		{
			item = new Spawn("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::bonefireOfInsight:
		{
			item = new BonefireOfInsight("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::homeCosiness:
		{
			item = new HomeCosiness("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::mushroomStone:
		{
			item = new MushroomStone("item", Vector2f(0, 0), -1);
		}
		case Tag::mushroomsOnStone:
		{
			item = new MushroomsOnStone("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::ground:
		{
			item = new Ground("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::groundConnection:
		{
			item = new GroundConnection("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::chamomile:
		{
			item = new Chamomile("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::brazier:
		{
			item = new Brazier("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::wreathTable:
		{
			item = new WreathTable("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::yarrow:
		{
			item = new Yarrow("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fern:
		{
			item = new Fern("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::hareTrap:
		{
			item = new HareTrap("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::rock:
		{
			item = new Rock("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fence:
		{
			item = new Fence("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::stump:
		{
			item = new Stump("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::droppedLoot:
		{
			item = new DroppedLoot("item", Vector2f(0, 0), -1, count);
			break;
		}
		case Tag::mugwort:
		{
			item = new Mugwort("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::totem:
		{
			item = new Totem("item", Vector2f(0, 0), -1);
			break;
		}
		case Tag::fog:
		{
			item = new Fog("item", Vector2f(0, 0), -1);
			break;
		}
		default:
		{
			item = new Spawn("item", Vector2f(0, 0), -1);
			break;
		}
	}

	int currentType = itemType == -1
		? currentType = rand() % item->getVarietyOfTypes() + 1
		: currentType = itemType;

	newNameId++;

	item->setPosition(Vector2f(itemPosition));
	item->setType(currentType);
	item->prepareSpriteNames(0);
	FloatRect textureBounds = FloatRect(0, 0, 0, 0);
	if (spriteMap->count(item->additionalSprites[0].path) > 0)
	textureBounds = spriteMap->at(item->additionalSprites[0].path).sprite.getGlobalBounds();
	const auto textureSize = Vector2f(int(textureBounds.width), int(textureBounds.height));
	item->setTextureSize(textureSize);
	const std::string name = itemName.empty()
		? item->additionalSprites[0].path + "_" + std::to_string(newNameId)
		: itemName;
	item->setName(name);
	if (!inventory.empty())
		item->inventory = inventory;	
	return item;
}

DynamicObject* ObjectInitializer::initializeDynamicItem(
	Tag itemClass,
	Vector2f itemPosition,
	std::string itemName,
	std::unordered_map<std::string, BoardSprite>* spriteMap,
	WorldObject* owner)
{
	DynamicObject* item;
	std::string nameOfImage;

	switch (itemClass)
	{
	case Tag::hero1:
	{
		item = new Deerchant("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/hero/stand/down/1";		
		break;
	}
	case Tag::wolf:
	{
		item = new Wolf("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/wolf/stand/down/1";
		break;
	}
	case Tag::hare:
	{
		item = new Hare("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/hare/stand/down/1";
		break;
	}
	case Tag::deer:
	{
		item = new Deer("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case Tag::bear:
	{
		item = new Bear("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case Tag::owl:
	{
		item = new Owl("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/deer/stand/down/1";
		break;
	}
	case Tag::noose:
	{
		item = new Noose("item", Vector2f(0, 0), owner);
		nameOfImage = "Game/worldSprites/noose/nooseLoop/1";
		break;
	}
	case Tag::clapWhirl:
	{
		item = new ClapWhirl("item", Vector2f(0, 0), owner);
		nameOfImage = "Game/worldSprites/nightmare3/clap/whirl";
		break;
	}
	case Tag::owlBoss:
	{
		item = new OwlBoss("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/owlBoss/stand/down/1";
		break;
	}
	case Tag::nightmare1:
	{
		item = new Nightmare1("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare1/stand/down/1";
		break;
	}
	case Tag::nightmare2:
	{
		item = new Nightmare2("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	case Tag::nightmare3:
	{
		item = new Nightmare3("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	default:
	{
		item = new Nightmare2("item", Vector2f(0, 0));
		nameOfImage = "Game/worldSprites/nightmare2/stand/down/1";
		break;
	}
	}

	newNameId++;
	nameOfImage += ".png";

	const std::string name = itemName.empty()
		? std::string(item->getToSaveName()) + "_" + std::to_string(newNameId)
		: itemName;

	item->setName(name);
	item->setPosition(Vector2f(itemPosition));
	const Vector2f textureSize = Vector2f(spriteMap->at(nameOfImage).texture.getSize());
	item->setTextureSize(textureSize);
	return item;
}

std::vector<StaticObject*> ObjectInitializer::vectorCastToStatic(std::vector<WorldObject*> items)
{
	std::vector<StaticObject*> staticItems = *(new std::vector<StaticObject*>());
	for (auto& item : items)
		staticItems.push_back(dynamic_cast<StaticObject*>(item));
	return staticItems;
}

std::vector<DynamicObject*> ObjectInitializer::vectorCastToDynamic(std::vector<WorldObject*> items)
{
	std::vector<DynamicObject*> dynamicItems = *(new std::vector<DynamicObject*>());
	for (auto& item : items)
		dynamicItems.push_back(dynamic_cast<DynamicObject*>(item));
	return dynamicItems;
}

