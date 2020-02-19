#include "DrawSystem.h"

DrawSystem::DrawSystem()
{
    initPacksMap();
}

DrawSystem::~DrawSystem()
{
}

bool DrawSystem::searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders)
{
	LPTSTR part;
	char tmp[MAX_PATH];
	char name[MAX_PATH];

	HANDLE hSearch = NULL;
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

					searchFiles(next, lpSearchFunc, 1);
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

void initSpritePack(LPCTSTR lpszFileName, std::map<PackTag, SpritePack> &packs_map)
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
        auto delimiter = packName.find('/');
        if (delimiter <= 0 || delimiter >= packName.size())
            break;
        packName.erase(0, delimiter + 1);
    }
    const int pngExtensionWithDotLength = 4;
    packName.erase(packName.size() - pngExtensionWithDotLength, pngExtensionWithDotLength);
    if (SpritePack::mappedPackTag.count(packName) == 0)
        return;
    auto tag = SpritePack::mappedPackTag.at(packName);
    packs_map[tag].init(packPath, jsonPath, tag);
}

void DrawSystem::initPacksMap()
{
	searchFiles("Game/spritePacks/*.png", initSpritePack, 1);
	SpritePack::iconWithoutSpaceSize = Vector2f(packsMap.at(PackTag::inventory).getOriginalInfo(PackPart::areas, Direction::DOWN, 1).frame.w,
		packsMap.at(PackTag::inventory).getOriginalInfo(PackPart::areas, Direction::DOWN, 1).frame.h);
}

void DrawSystem::advancedScale(SpriteChainElement& item, Sprite& sprite, sprite_pack::sprite originalInfo, float scale) const
{
	if (!originalInfo.rotated)
		sprite.setScale(scale * item.size.x / originalInfo.source_size.w,
			scale * item.size.y / originalInfo.source_size.h);
	else
		sprite.setScale(scale * item.size.y / originalInfo.source_size.h,
			scale * item.size.x / originalInfo.source_size.w);

	if (!item.isBackground && !item.unscaled)
	{
		if (!originalInfo.rotated)
		{
			sprite.scale(1, pow(scale, double(1) / 6));
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (pow(scale, double(1) / 6) - 1) * originalInfo.source_size.h);
		}
		else
		{
			sprite.scale(pow(scale, double(1) / 6), 1);
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (pow(scale, double(1) / 6) - 1) * originalInfo.source_size.w);
		}
	}
}

std::vector<DrawableChainElement*> DrawSystem::UpcastChain(std::vector<SpriteChainElement*> chain)
{
	std::vector<DrawableChainElement*> result = {};
	for (auto spriteChainItem : chain)
	{
		auto item = static_cast<DrawableChainElement*>(spriteChainItem);
		result.push_back(item);
	}
	return result;
}

std::vector<SpriteChainElement*> DrawSystem::DowncastToSpriteChain(std::vector<DrawableChainElement*> chain)
{
	std::vector<SpriteChainElement*> result = {};
	for (auto item : chain)
	{
		auto spriteChainItem = static_cast<SpriteChainElement*>(item);
		result.push_back(spriteChainItem);
	}
	return result;
}

void DrawSystem::drawSpriteChainElement(RenderTarget& target, SpriteChainElement* spriteChainItem, Vector2f cameraPosition, Vector2f screenCenter, float scale)
{	
	if (spriteChainItem->packTag == PackTag::empty)
		return;

	auto sprite = packsMap.at(spriteChainItem->packTag).getSprite(spriteChainItem->packPart, spriteChainItem->direction, spriteChainItem->number, spriteChainItem->mirrored);
	if (sprite.getTextureRect() == IntRect())
		return;

	const auto originalInfo = packsMap.at(spriteChainItem->packTag).getOriginalInfo(spriteChainItem->packPart, spriteChainItem->direction, spriteChainItem->number);
	const Vector2f spritePos = { (spriteChainItem->position.x - cameraPosition.x - spriteChainItem->offset.x) * scale + screenCenter.x,
	(spriteChainItem->position.y - cameraPosition.y - spriteChainItem->offset.y) * scale + screenCenter.y };
	Vector2f localScale(float(spriteChainItem->size.x) / originalInfo.source_size.w, float(spriteChainItem->size.y) / originalInfo.source_size.h);
	if (spriteChainItem->antiTransparent)
		spriteChainItem->color.a = 255;

	sprite.setColor(spriteChainItem->color);
	sprite.rotate(spriteChainItem->rotation);
	sprite.setPosition(spritePos);

	advancedScale(*spriteChainItem, sprite, originalInfo, scale);

	target.draw(sprite);
}

void DrawSystem::drawTextChainElement(RenderTarget& target, TextChainElement* textChainItem)
{
	TextSystem::drawString(
		textChainItem->string,
		textChainItem->font,
		textChainItem->characterSize,
		textChainItem->position.x - textChainItem->offset.x, 
		textChainItem->position.y - textChainItem->offset.y,
		target,
		textChainItem->color);
}

void DrawSystem::draw(RenderTarget& target, std::vector<DrawableChainElement*> drawableItems, float scale, Vector2f cameraPosition)
{
    if (drawableItems.empty())
        return;
    const auto screenSize = target.getSize();
	Vector2f screenCenter;
	if (cameraPosition != Vector2f(0, 0))
		screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);
	else
		screenCenter = { 0, 0 };

	for (auto drawableChainItem : drawableItems)
	{
		const auto spriteChainItem = dynamic_cast<SpriteChainElement*>(drawableChainItem);
		if (spriteChainItem)
			drawSpriteChainElement(target, spriteChainItem, cameraPosition, screenCenter, scale);

		const auto textChainItem = dynamic_cast<TextChainElement*>(drawableChainItem);
		if (textChainItem)
			drawTextChainElement(target, textChainItem);
	}
}

