#pragma once

#include <SFML/Graphics.hpp>
#include <windows.h>

#include "sprite_pack.h"
#include "shader_system.h"
#include "text_chain_element.h"

using namespace sf;
using search_func = void (*)(LPCTSTR, std::map<pack_tag, sprite_pack>&);
using func = bool (*)(Vector2f&);

class draw_system
{
public:
	draw_system();
	~draw_system();

	void draw_sprite_chain_element(RenderTarget& target, sprite_chain_element* sprite_chain_item, Vector2f camera_position, Vector2f screen_center, float scale);
	static void draw_text_chain_element(RenderTarget& target, text_chain_element* text_chain_item);
	void draw(RenderTarget& target, const std::vector<drawable_chain_element*>& drawable_items, float scale = 1, Vector2f camera_position = {0, 0});
	static std::vector<drawable_chain_element*> upcast_chain(const std::vector<sprite_chain_element*>& chain);
	static std::vector<sprite_chain_element*> downcast_to_sprite_chain(const std::vector<drawable_chain_element*>& chain);
	
	std::map<pack_tag, sprite_pack> packs_map;
private:
	void init_packs_map();
	bool search_files(LPCTSTR lpsz_file_name, search_func lp_search_func, bool b_inner_folders = true);
	static void advanced_scale(sprite_chain_element& item, Sprite& sprite, const sprite_pack_structure::sprite& original_info, float scale = 1);
};
