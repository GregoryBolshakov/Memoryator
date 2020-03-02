#pragma once

#include <SFML/Graphics.hpp>
#include <windows.h>

#include "sprite_pack.h"
#include "text_chain_element.h"

using namespace  sf;
typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName, std::map<PackTag, sprite_pack> &packs_map);
typedef bool(*func)(Vector2f &pos);

class draw_system
{
public:
    draw_system();
    ~draw_system();
	void drawSpriteChainElement(RenderTarget& target, sprite_chain_element* spriteChainItem, Vector2f cameraPosition, Vector2f screenCenter, float scale);
	static void drawTextChainElement(RenderTarget& target, text_chain_element* textChainItem);
    void draw(RenderTarget& target, const std::vector<drawable_chain_element*>& drawableItems, float scale = 1, Vector2f cameraPosition = {0, 0});
	static std::vector<drawable_chain_element*> UpcastChain(const std::vector<sprite_chain_element*>& chain);
	static std::vector<sprite_chain_element*> DowncastToSpriteChain(const std::vector<drawable_chain_element*>& chain);

    std::map<PackTag, sprite_pack> packsMap;
private:
    void initPacksMap();
	bool searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders = true);
	void advancedScale(sprite_chain_element& item, Sprite& sprite, const sprite_pack_structure::sprite& originalInfo, float scale = 1) const;
};
