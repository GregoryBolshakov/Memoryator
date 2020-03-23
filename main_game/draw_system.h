#pragma once

#include <SFML/Graphics.hpp>
#include <windows.h>


#include "shader_system.h"
#include "shape_chain_element.h"
#include "sprite_pack.h"
#include "text_chain_element.h"

using namespace sf;
using search_func = void (*)(LPCTSTR, std::map<pack_tag, sprite_pack>&);
using func = bool (*)(Vector2f&);

class draw_system
{
public:
	static void draw_text_chain_element(RenderTarget& target, text_chain_element* text_chain_item);
	static void draw_shape_chain_element(RenderTarget& target, shape_chain_element* shape_chain_element);
	static std::vector<drawable_chain_element*> upcast_chain(const std::vector<sprite_chain_element*>& chain);
	static std::vector<sprite_chain_element*> downcast_to_sprite_chain(const std::vector<drawable_chain_element*>& chain);

	std::map<pack_tag, sprite_pack> packs_map{};
	
	explicit draw_system(shader_system& shader_system, Vector2f screen_size);

	void draw_sprite_chain_element(RenderTarget& target, sprite_chain_element* sprite_chain_item, Vector2f camera_position, Vector2f screen_center, float scale);
	void draw(RenderTarget& target, const std::vector<drawable_chain_element*>& drawable_items, float scale = 1, Vector2f camera_position = {0, 0});

	void draw(RenderWindow& target, shader_kind kind);
private:
	static void advanced_scale(sprite_chain_element& item, Sprite& sprite, const sprite_pack_structure::sprite& original_info, float scale = 1);

	void init_packs_map();
	bool search_files(LPCTSTR lpsz_file_name, search_func lp_search_func, bool b_inner_folders = true);

	shader_system& shader_system_;
	RenderTexture render_texture_;
	Texture shader_texture_;
	Sprite shader_sprite_;
};
