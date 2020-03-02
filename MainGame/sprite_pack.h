#pragma once

#include <nlohmann/json.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "helper.h"
#include "sprite_chain_element.h"

using namespace sf;
using json = nlohmann::json;

namespace sprite_pack_structure
{
    struct size
    {
	    explicit size(const int w = 0, const int h = 0) { this->w = w; this->h = h; }
        int w = 0;
        int h = 0;
    };

	inline bool operator == (const size a, const size b)
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

class sprite_pack
{
public:
    void init(const std::string& path, const std::string& jsonPath, pack_tag tag);
    Sprite getSprite(pack_part part, direction direction, int number, bool mirrored = false);
	sprite_pack_structure::sprite getOriginalInfo(pack_part part, direction direction, int number);
	static sprite_chain_element* tagToIcon(entity_tag object, bool selected = false, int typeOfObject = 1);

    pack_tag tag;
    static std::map<std::string, pack_tag> mappedPackTag;
    static std::map<std::string, pack_part> mappedPackPart;
    static std::map<std::string, direction> mappedDirection;
	static const Vector2f iconSize;
	static Vector2f iconWithoutSpaceSize;
private:
    Texture texture;
    std::map<pack_part, std::map<direction, std::map<int, sprite_pack_structure::sprite>>> pack;
};
