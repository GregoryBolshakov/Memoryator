#include "draw_system.h"

#include "text_system.h"

draw_system::draw_system()
{
	initPacksMap();
}

draw_system::~draw_system() = default;

bool draw_system::searchFiles(const LPCTSTR lpszFileName, const LPSEARCHFUNC lpSearchFunc, const bool bInnerFolders)
{
	LPTSTR part;
	char tmp[MAX_PATH];
	char name[MAX_PATH];

	HANDLE hSearch;
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	if (bInnerFolders)
	{
		if (GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
		strcpy(name, part);
		strcpy(part, "*.*");

		wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
			do
			{
				if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
					continue;

				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					char next[MAX_PATH];
					if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
					strcpy(part, wfd.cFileName);
					strcat(next, "\\");
					strcat(next, name);

					searchFiles(next, lpSearchFunc, true);
				}
			} while (FindNextFile(hSearch, &wfd));

		FindClose(hSearch);
	}

	if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE)
		return TRUE;
	do
		if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char file[MAX_PATH];
			if (GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
			strcpy(part, wfd.cFileName);

			lpSearchFunc(file, packsMap);
		}
	while (FindNextFile(hSearch, &wfd));
	FindClose(hSearch);

	return TRUE;
}

void initSpritePack(const LPCTSTR lpszFileName, std::map<pack_tag, sprite_pack>& packs_map)
{
	std::string packPath = lpszFileName;
	packPath.erase(0, packPath.find("\\Game\\spritePacks") + 1);
	std::replace(packPath.begin(), packPath.end(), '\\', '/');
	std::string jsonPath = packPath;
	const int pngExtensionLength = 3;
	jsonPath.erase(jsonPath.size() - pngExtensionLength, pngExtensionLength);
	jsonPath.insert(jsonPath.size(), "json");

	auto packName = packPath;
	while (true)
	{
		const auto delimiter = packName.find('/');
		if (delimiter <= 0 || delimiter >= packName.size())
			break;
		packName.erase(0, delimiter + 1);
	}
	const int pngExtensionWithDotLength = 4;
	packName.erase(packName.size() - pngExtensionWithDotLength, pngExtensionWithDotLength);
	if (sprite_pack::mappedPackTag.count(packName) == 0)
		return;
	const auto tag = sprite_pack::mappedPackTag.at(packName);
	packs_map[tag].init(packPath, jsonPath, tag);
}

void draw_system::initPacksMap()
{
	searchFiles("Game/spritePacks/*.png", initSpritePack, true);
	sprite_pack::iconWithoutSpaceSize = Vector2f(
		float(packsMap.at(pack_tag::inventory).getOriginalInfo(pack_part::areas, Direction::DOWN, 1).frame.w),
		float(packsMap.at(pack_tag::inventory).getOriginalInfo(pack_part::areas, Direction::DOWN, 1).frame.h));
}

void draw_system::advancedScale(sprite_chain_element& item, Sprite& sprite, const sprite_pack_structure::sprite& originalInfo, const float scale) const
{
	const auto size_w = float(originalInfo.source_size.w);
	const auto size_h = float(originalInfo.source_size.h);
	
	if (!originalInfo.rotated)
		sprite.setScale(
			scale * item.size.x / size_w,
			scale * item.size.y / size_h);
	else
		sprite.setScale(
			scale * item.size.y / size_h,
			scale * item.size.x / size_w);

	if (!item.isBackground && !item.unscaled)
	{
		if (!originalInfo.rotated)
		{
			sprite.scale(1, pow(scale, 1.0f / 6));
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (pow(scale, 1.0f / 6) - 1) * size_h);
		}
		else
		{
			sprite.scale(pow(scale, 1.0f / 6), 1);
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (pow(scale, 1.0f / 6) - 1) * size_w);
		}
	}
}

std::vector<drawable_chain_element*> draw_system::UpcastChain(const std::vector<sprite_chain_element*>& chain)
{
	std::vector<drawable_chain_element*> result = {};
	for (auto spriteChainItem : chain)
	{
		auto item = static_cast<drawable_chain_element*>(spriteChainItem);
		result.push_back(item);
	}
	return result;
}

std::vector<sprite_chain_element*> draw_system::DowncastToSpriteChain(const std::vector<drawable_chain_element*>& chain)
{
	std::vector<sprite_chain_element*> result = {};
	for (auto item : chain)
	{
		auto spriteChainItem = dynamic_cast<sprite_chain_element*>(item);
		result.push_back(spriteChainItem);
	}
	return result;
}

void draw_system::drawSpriteChainElement(RenderTarget& target, sprite_chain_element* spriteChainItem, const Vector2f cameraPosition, const Vector2f screenCenter, const float scale)
{
	if (spriteChainItem->packTag == pack_tag::empty)
		return;

	auto sprite = packsMap.at(spriteChainItem->packTag).getSprite(spriteChainItem->packPart, spriteChainItem->direction, spriteChainItem->number, spriteChainItem->mirrored);
	if (sprite.getTextureRect() == IntRect())
		return;

	const auto originalInfo = packsMap.at(spriteChainItem->packTag).getOriginalInfo(spriteChainItem->packPart, spriteChainItem->direction, spriteChainItem->number);
	const Vector2f spritePos = { (spriteChainItem->position.x - cameraPosition.x - spriteChainItem->offset.x) * scale + screenCenter.x,
	(spriteChainItem->position.y - cameraPosition.y - spriteChainItem->offset.y) * scale + screenCenter.y };
	if (spriteChainItem->antiTransparent)
		spriteChainItem->color.a = 255;

	sprite.setColor(spriteChainItem->color);
	sprite.rotate(spriteChainItem->rotation);
	sprite.setPosition(spritePos);

	advancedScale(*spriteChainItem, sprite, originalInfo, scale);

	target.draw(sprite);
}

void draw_system::drawTextChainElement(RenderTarget& target, text_chain_element* textChainItem)
{
	text_system::drawString(
		textChainItem->string,
		textChainItem->font,
		textChainItem->characterSize,
		textChainItem->position.x - textChainItem->offset.x,
		textChainItem->position.y - textChainItem->offset.y,
		target,
		textChainItem->color);
}

void draw_system::draw(RenderTarget& target, const std::vector<drawable_chain_element*>& drawableItems, const float scale, const Vector2f cameraPosition)
{
	if (drawableItems.empty())
		return;

	const auto screenCenter = cameraPosition != Vector2f()
		? Vector2f(target.getSize()) / 2.0f
		: Vector2f();

	for (auto drawableChainItem : drawableItems)
	{
		if (!drawableChainItem->initialized)		
			continue;		

		const auto spriteChainItem = dynamic_cast<sprite_chain_element*>(drawableChainItem);
		if (spriteChainItem)
			drawSpriteChainElement(target, spriteChainItem, cameraPosition, screenCenter, scale);

		const auto textChainItem = dynamic_cast<text_chain_element*>(drawableChainItem);
		if (textChainItem)		
			drawTextChainElement(target, textChainItem);	
	}
}
