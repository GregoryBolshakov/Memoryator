#pragma once

#include <SFML/Graphics.hpp>
#include <windows.h>
#include "SpritePack.h"
#include "TextChainElement.h"

using namespace  sf;
typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName, std::map<PackTag, SpritePack> &packs_map);
typedef bool(*func)(Vector2f &pos);

class DrawSystem
{
public:
    DrawSystem();
    ~DrawSystem();
	void drawSpriteChainElement(RenderTarget& target, SpriteChainElement* spriteChainItem, Vector2f cameraPosition, Vector2f screenCenter, float scale);
	static void drawTextChainElement(RenderTarget& target, TextChainElement* textChainItem);
    void draw(RenderTarget& target, const std::vector<DrawableChainElement*>& drawableItems, float scale = 1, Vector2f cameraPosition = {0, 0});
	static std::vector<DrawableChainElement*> UpcastChain(const std::vector<SpriteChainElement*>& chain);
	static std::vector<SpriteChainElement*> DowncastToSpriteChain(const std::vector<DrawableChainElement*>& chain);

    std::map<PackTag, SpritePack> packsMap;
private:
    void initPacksMap();
	bool searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders = true);
	void advancedScale(SpriteChainElement& item, Sprite& sprite, const sprite_pack::sprite& originalInfo, float scale = 1) const;
};
