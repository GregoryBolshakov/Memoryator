#include "DrawSystem.h"

std::map<Tag, bool> DrawSystem::unscaledObjects = {{Tag::hero1, true}, {Tag::nightmare1, true}, {Tag::nightmare2, true}, {Tag::nightmare3, true}, {Tag::grass, true}, {Tag::lake, true}, {Tag::noose, true} };

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
	int objectsNumber;
	std::string name;

	searchFiles("Game/spritePacks/*.png", initSpritePack, 1);	
	//effectSystem.init(&spriteMap);
}

void DrawSystem::advancedScale(SpriteChainElement& item, Sprite& sprite, sprite_pack::sprite originalInfo, float scale) const
{
	if (!originalInfo.rotated)
		sprite.setScale(scale * item.size.x / originalInfo.source_size.w,
			scale * item.size.y / originalInfo.source_size.h);
	else
		sprite.setScale(scale * item.size.y / originalInfo.source_size.h,
			scale * item.size.x / originalInfo.source_size.w);

	if (!item.isBackground && unscaledObjects.count(item.tag) == 0)
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

void DrawSystem::draw(RenderTarget& target, std::vector<SpriteChainElement> sprites)
{
    if (sprites.empty())
        return;

	for (auto& spriteChainItem : sprites)
	{
		auto sprite = packsMap.at(spriteChainItem.packTag).getSprite(spriteChainItem.packPart, spriteChainItem.direction, spriteChainItem.number);
        const auto originalInfo = packsMap.at(spriteChainItem.packTag).getOriginalInfo(spriteChainItem.packPart, spriteChainItem.direction, spriteChainItem.number);
		if (spriteChainItem.antiTransparent)
			spriteChainItem.color.a = 255;

		sprite.setColor(spriteChainItem.color);
		sprite.rotate(spriteChainItem.rotation);
		sprite.setPosition(spriteChainItem.position);

        if (!originalInfo.rotated)
            sprite.setScale(spriteChainItem.size.x / sprite.getGlobalBounds().width, spriteChainItem.size.y / sprite.getGlobalBounds().height);
        else
            sprite.setScale(spriteChainItem.size.y / sprite.getGlobalBounds().height, spriteChainItem.size.x / sprite.getGlobalBounds().width);

		target.draw(sprite);
	}
}

void DrawSystem::drawToWorld(RenderTarget& target, std::vector<SpriteChainElement> sprites, float scale, Vector2f cameraPosition)
{
    if (sprites.empty())
        return;
    const auto screenSize = target.getSize();
	const auto screenCenter = Vector2f(screenSize.x / 2, screenSize.y / 2);

	for (auto& spriteChainItem : sprites)
	{
		auto sprite = packsMap.at(spriteChainItem.packTag).getSprite(spriteChainItem.packPart, spriteChainItem.direction, spriteChainItem.number, spriteChainItem.mirrored);
		if (sprite.getTextureRect() == IntRect())
			continue;
        const auto originalInfo = packsMap.at(spriteChainItem.packTag).getOriginalInfo(spriteChainItem.packPart, spriteChainItem.direction, spriteChainItem.number);
        Vector2f spritePos = {(spriteChainItem.position.x - cameraPosition.x - spriteChainItem.offset.x) * scale + screenCenter.x,
		(spriteChainItem.position.y - cameraPosition.y - spriteChainItem.offset.y) * scale + screenCenter.y};
        Vector2f localScale(float(spriteChainItem.size.x) / originalInfo.source_size.w, float(spriteChainItem.size.y) / originalInfo.source_size.h);
		if (spriteChainItem.antiTransparent)
			spriteChainItem.color.a = 255;

         if (spriteChainItem.tag == Tag::hero1 && spriteChainItem.packPart == PackPart::legs)
            auto test = 1;

		sprite.setColor(spriteChainItem.color);
        sprite.rotate(spriteChainItem.rotation);
		sprite.setPosition(spritePos);        

		advancedScale(spriteChainItem, sprite, originalInfo, scale);

		target.draw(sprite);
	}
}

