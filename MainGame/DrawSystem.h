#pragma once
#include <SFML/Graphics.hpp>
#include "SpritePack.h"
#include <windows.h>

using namespace  sf;
typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName, std::map<PackTag, SpritePack> &packs_map);
typedef bool(*func)(Vector2f &pos);

class DrawSystem
{
public:
    DrawSystem();
    ~DrawSystem();
    void draw(RenderTarget& target, std::vector<SpriteChainElement> sprites);
    void drawToWorld(RenderTarget& target, std::vector<SpriteChainElement> sprites, float scale = 1, Vector2f cameraPosition = {0, 0});

    std::map<PackTag, SpritePack> packsMap;
    static std::map<Tag, bool> unscaledObjects;
private:
    void initPacksMap();
	bool searchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, bool bInnerFolders = true);
	void advancedScale(SpriteChainElement& item, Sprite& sprite, sprite_pack::sprite originalInfo, float scale = 1) const;
};

