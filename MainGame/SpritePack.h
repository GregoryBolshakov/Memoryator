#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Helper.h"
#include "sprite_chain_element.h"

using namespace sf;
using json = nlohmann::json;

namespace sprite_pack
{
    struct size
    {
	    explicit size(const int w = 0, const int h = 0) { this->w = w; this->h = h; }
        int w = 0;
        int h = 0;
    };

	inline bool operator == (const sprite_pack::size a, const sprite_pack::size b)
	{
		return (a.w == b.w && a.h == b.h);
	}

    struct rect
    {
        int x;
        int y;
        int w;
        int h;
    };

    struct sprite
    {
        std::string frame_name;
        rect frame{};
        bool rotated{};
        bool trimmed{};
        rect sprite_source_size{};
        size source_size;
    };

    struct pack
    {
        std::vector<sprite> frames;
    };

    inline void from_json(const json& j, size& s)
    {
        j.at("w").get_to(s.w);
        j.at("h").get_to(s.h);
    }

    inline void from_json(const json& j, rect& r)
    {
        j.at("x").get_to(r.x);
        j.at("y").get_to(r.y);
        j.at("w").get_to(r.w);
        j.at("h").get_to(r.h);
    }

    inline void from_json(const json& j, sprite& s)
    {
        j.at("filename").get_to(s.frame_name);
        j.at("frame").get_to(s.frame);
        j.at("rotated").get_to(s.rotated);
        j.at("trimmed").get_to(s.trimmed);
        j.at("spriteSourceSize").get_to(s.sprite_source_size);
        j.at("sourceSize").get_to(s.source_size);
    }

    inline void from_json(const json& j, pack& p)
    {
        j.at("frames").get_to(p.frames);
    }
}

class SpritePack
{
public:
    void init(const std::string& path, const std::string& jsonPath, PackTag tag);
    Sprite getSprite(PackPart part, Direction direction, int number, bool mirrored = false);
	sprite_pack::sprite getOriginalInfo(PackPart part, Direction direction, int number);
	static sprite_chain_element* tagToIcon(Tag object, bool selected = false, int typeOfObject = 1);

    PackTag tag;
    static std::map<std::string, PackTag> mappedPackTag;
    static std::map<std::string, PackPart> mappedPackPart;
    static std::map<std::string, Direction> mappedDirection;
	static const Vector2f iconSize;
	static Vector2f iconWithoutSpaceSize;
private:
    Texture texture;
    std::map<PackPart, std::map<Direction, std::map<int, sprite_pack::sprite>>> pack;
};
