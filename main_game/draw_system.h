#pragma once

#include <memory>
#include <map>
#include <Windows.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sprite_pack_structure {
	struct sprite;
}

namespace sf
{
	class RenderWindow;
}
class shader_system;
class sprite_pack;
class drawable_chain_element;
class shape_chain_element;
class text_chain_element;
class sprite_chain_element;
enum class shader_kind;
enum class pack_tag;

using packs_map_t = std::map<pack_tag, sprite_pack>;
using search_func = void (*)(LPCTSTR, packs_map_t&);
using func = bool (*)(sf::Vector2f&);

class draw_system
{
public:
	draw_system(std::shared_ptr<shader_system> shader_system, sf::Vector2f screen_size);
	static void draw_text_chain_element(const std::shared_ptr<sf::RenderWindow>& target, text_chain_element& text_chain_item);
	static void draw_shape_chain_element(const std::shared_ptr<sf::RenderWindow>& target, shape_chain_element& shape_chain_element);
	void draw_sprite_chain_element(const std::shared_ptr<sf::RenderWindow>& target, sprite_chain_element& sprite_chain_item, sf::Vector2f camera_position, sf::Vector2f screen_center, float scale);
	void draw(const std::shared_ptr<sf::RenderWindow>& target, const std::vector<std::unique_ptr<drawable_chain_element>>& drawable_items, float scale = 1, sf::Vector2f camera_position = {0, 0});
	void draw(const std::shared_ptr<sf::RenderWindow>& target, shader_kind kind);

	const std::shared_ptr<packs_map_t>& get_packs_map() const { return packs_map_; }
private:
	static void advanced_scale(sprite_chain_element& item, sf::Sprite& sprite, const sprite_pack_structure::sprite& original_info, float scale = 1);

	void init_packs_map();
	bool search_files(LPCTSTR lpsz_file_name, search_func lp_search_func, bool b_inner_folders = true);

	std::shared_ptr<packs_map_t> packs_map_;
	std::shared_ptr<shader_system> shader_system_;
	sf::RenderTexture render_texture_;
	sf::Texture shader_texture_;
	sf::Sprite shader_sprite_;
};
