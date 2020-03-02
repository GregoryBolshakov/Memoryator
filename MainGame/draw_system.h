#pragma once

#include <SFML/Graphics.hpp>
#include <windows.h>

#include "SpritePack.h"
#include "TextChainElement.h"

using namespace  sf;
typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName, std::map<PackTag, SpritePack> &packs_map);
typedef bool(*func)(Vector2f &pos);

class draw_system
{
public:
    draw_system();
    ~draw_system();
	void drawSpriteChainElement(RenderTarget& target, sprite_chain_element* spriteChainItem, Vector2f cameraPosition, Vector2f screenCenter, float scale);
	static void drawTextChainElement(RenderTarget& target, TextChainElement* textChainItem);
    void draw(RenderTarget& target, const std::vector<drawable_chain_element*>& drawableItems, float scale = 1, Vector2f cameraPosition = {0, 0});
	static std::vector<drawable_chain_element*> UpcastChain(const std::vector<sprite_chain_element*>& chain);
	static std::vector<sprite_chain_element*> DowncastToSpriteChain(const std::vector<drawable_chain_element*>& chain);

    std::map<PackTag, SpritePack> packsMap;
private:
    void initPacksMap();
	bool searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders = true);
	void advancedScale(sprite_chain_element& item, Sprite& sprite, const sprite_pack::sprite& originalInfo, float scale = 1) const;
};
