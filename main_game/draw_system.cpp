#include "draw_system.h"
#include "sprite_chain_element.h"
#include "text_chain_element.h"
#include "shape_chain_element.h"
#include "shader_system.h"
#include "direction_system.h"
#include "text_system.h"
#include "sprite_pack.h"
#include "world_metrics.h"

draw_system::draw_system(shared_ptr<shader_system> shader_system, const sf::Vector2f screen_size) : shader_system_{
	std::move(shader_system)
}
{
	init_packs_map();
	render_texture_.create(unsigned(screen_size.x), unsigned(screen_size.y));
	shader_texture_ = render_texture_.getTexture();
	shader_sprite_ = sf::Sprite(shader_texture_);
}

bool draw_system::search_files(const LPCTSTR lpsz_file_name, const search_func lp_search_func, const bool b_inner_folders)
{
	LPTSTR part;
	char tmp[MAX_PATH];
	char name[MAX_PATH];

	HANDLE h_search;
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	if (b_inner_folders)
	{
		if (GetFullPathName(lpsz_file_name, MAX_PATH, tmp, &part) == 0)
		{
			return FALSE;
		}
		strcpy(name, part);
		strcpy(part, "*.*");

		wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		if (!((h_search = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
		{
			do
			{
				if ((strncmp(wfd.cFileName, ".", 1) == 0) || (strncmp(wfd.cFileName, "..", 2) == 0))
				{
					continue;
				}

				if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0u)
				{
					char next[MAX_PATH];
					if (GetFullPathName(lpsz_file_name, MAX_PATH, next, &part) == 0)
					{
						return FALSE;
					}
					strcpy(part, wfd.cFileName);
					strcat(next, "\\");
					strcat(next, name);

					search_files(next, lp_search_func, true);
				}
			} while (FindNextFile(h_search, &wfd));
		}

		FindClose(h_search);
	}

	if ((h_search = FindFirstFile(lpsz_file_name, &wfd)) == INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}
	do
	{
		if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0u)
		{
			char file[MAX_PATH];
			if (GetFullPathName(lpsz_file_name, MAX_PATH, file, &part) == 0)
			{
				return FALSE;
			}
			strcpy(part, wfd.cFileName);

			lp_search_func(file, *packs_map_);
		}
	} while (FindNextFile(h_search, &wfd));
	FindClose(h_search);

	return TRUE;
}

void init_sprite_pack(const LPCTSTR lpsz_file_name, std::map<pack_tag, sprite_pack>& packs_map)
{
	std::string pack_path = lpsz_file_name;
	pack_path.erase(0, pack_path.find("\\Game\\spritePacks") + 1);
	std::replace(pack_path.begin(), pack_path.end(), '\\', '/');
	auto json_path = pack_path;
	const auto png_extension_length = 3;
	json_path.erase(json_path.size() - png_extension_length, png_extension_length);
	json_path.insert(json_path.size(), "json");

	auto pack_name = pack_path;
	while (true)
	{
		const auto delimiter = pack_name.find('/');
		if (delimiter <= 0 || delimiter >= pack_name.size())
		{
			break;
		}
		pack_name.erase(0, delimiter + 1);
	}
	const auto png_extension_with_dot_length = 4;
	pack_name.erase(pack_name.size() - png_extension_with_dot_length, png_extension_with_dot_length);
	if (sprite_pack::mapped_pack_tag.count(pack_name) == 0)
	{
		return;
	}
	const auto tag = sprite_pack::mapped_pack_tag.at(pack_name);
	packs_map[tag].init(pack_path, json_path, tag);
}

void draw_system::init_packs_map()
{
	packs_map_ = std::make_unique<packs_map_t>();
	search_files("Game/spritePacks/*.png", init_sprite_pack, true);
	sprite_pack::icon_without_space_size = sf::Vector2f(
		float(packs_map_->at(pack_tag::inventory).get_original_info(pack_part::areas, direction::DOWN, 1).frame.w),
		float(packs_map_->at(pack_tag::inventory).get_original_info(pack_part::areas, direction::DOWN, 1).frame.h));
}

void draw_system::advanced_scale(sprite_chain_element& item, sf::Sprite& sprite, const sprite_pack_structure::sprite& original_info, const float scale)
{
	const auto size_w = float(original_info.source_size.w);
	const auto size_h = float(original_info.source_size.h);

	if (!original_info.rotated)
	{
		sprite.setScale(
			scale * item.size.x / size_w,
			scale * item.size.y / size_h);
	}
	else
	{
		sprite.setScale(
			scale * item.size.y / size_h,
			scale * item.size.x / size_w);
	}

	if (!item.is_background && !item.isometric)
	{
		if (!original_info.rotated)
		{
			sprite.scale(1, pow(scale, 1.0F / 6));
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (pow(scale, 1.0f / 6) - 1) * size_h);
			//sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + pow(scale, 1.0f / 6) * (size_h - item.offset.y));
		}
		else
		{
			sprite.scale(pow(scale, 1.0F / 6), 1);
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (pow(scale, 1.0f / 6) - 1) * size_w);
			//sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + pow(scale, 1.0f / 6) * (size_w - item.offset.y));
		}
	}
}

void draw_system::draw_sprite_chain_element(const shared_ptr<sf::RenderWindow>& target, sprite_chain_element& sprite_chain_item, const float scale) const
{
	if (sprite_chain_item.pack_tag == pack_tag::empty)
	{
		return;
	}

	auto sprite = packs_map_->at(sprite_chain_item.pack_tag).get_sprite(sprite_chain_item.pack_part, sprite_chain_item.direction, sprite_chain_item.number, sprite_chain_item.mirrored);
	if (sprite.getTextureRect() == sf::IntRect())
	{
		return;
	}

	const auto original_info = packs_map_->at(sprite_chain_item.pack_tag).get_original_info(sprite_chain_item.pack_part, sprite_chain_item.direction, sprite_chain_item.number);
	
	if (sprite_chain_item.anti_transparent)
	{
		sprite_chain_item.color.a = 255;
	}

	sprite.setColor(sprite_chain_item.color);
	sprite.rotate(sprite_chain_item.rotation);
	if (scale == 1)
		sprite.setPosition(sprite_chain_item.position - sprite_chain_item.offset);
	else
		sprite.setPosition(world_metrics::world_to_screen_draw_position(sprite_chain_item.position, sprite_chain_item.offset));

	advanced_scale(sprite_chain_item, sprite, original_info, scale);

	/*if (sprite_chain_item.pack_tag == pack_tag::swampyTrees && sprite_chain_item.pack_part == pack_part::tree && original_info.frame_name == "tree/5")
	{
		const auto& shader = shader_system_->get_shader(target, sprite_chain_item, sprite);
		target.draw(sprite, &shader);
		//target.draw(sprite);
	}
	//else*/

	target->draw(sprite);
}

void draw_system::draw_shape_chain_element(const shared_ptr<sf::RenderWindow>& target, shape_chain_element& shape_chain_item, const float scale)
{
	if (shape_chain_item.type == shape_type::circle)
	{
		sf::CircleShape circle(shape_chain_item.radius * scale);
		if (scale == 1)
			circle.setPosition(shape_chain_item.position - shape_chain_item.offset);
		else
			circle.setPosition(world_metrics::world_to_screen_draw_position(shape_chain_item.position, shape_chain_item.offset));
		if (shape_chain_item.hollow)
		{
			circle.setOutlineColor(shape_chain_item.color);
			circle.setOutlineThickness(10);
			circle.setFillColor(sf::Color(255, 255, 255, 0));
		}
		else
			circle.setFillColor(shape_chain_item.color);
		target->draw(circle);
		return;
	}

	if (shape_chain_item.type == shape_type::rectangle)
	{
		sf::RectangleShape rect(shape_chain_item.size * scale);
		if (scale == 1)
			rect.setPosition(shape_chain_item.position - shape_chain_item.offset);
		else
			rect.setPosition(world_metrics::world_to_screen_draw_position(shape_chain_item.position, shape_chain_item.offset));
		if (shape_chain_item.hollow)
		{
			rect.setOutlineColor(shape_chain_item.color);
			rect.setOutlineThickness(10);
			rect.setFillColor(sf::Color(255, 255, 255, 0));
		}
		else
			rect.setFillColor(shape_chain_item.color);
		target->draw(rect);
		return;
	}
}

void draw_system::draw_text_chain_element(const shared_ptr<sf::RenderWindow>& target, text_chain_element& text_chain_item)
{
	text_system::draw_string(
		text_chain_item.string,
		text_chain_item.font,
		text_chain_item.character_size,
		text_chain_item.position.x - text_chain_item.offset.x,
		text_chain_item.position.y - text_chain_item.offset.y,
		target,
		text_chain_item.color);
}

void draw_system::draw(const shared_ptr<sf::RenderWindow>& target, const std::vector<std::unique_ptr<drawable_chain_element>>& drawable_items, const float scale) const
{
	if (drawable_items.empty())
	{
		return;
	}

	for (const auto& item : drawable_items)
	{
		if (!item->initialized)
		{
			continue;
		}

		auto* sprite_chain_item = dynamic_cast<sprite_chain_element*>(item.get());
		if (sprite_chain_item)
		{
			draw_sprite_chain_element(target, *sprite_chain_item, scale);
			continue;
		}

		auto* text_chain_item = dynamic_cast<::text_chain_element*>(item.get());
		if (text_chain_item != nullptr)
		{
			draw_text_chain_element(target, *text_chain_item);
			continue;
		}

		auto* shape_chain_element = dynamic_cast<::shape_chain_element*>(item.get());
		if (shape_chain_element != nullptr)
			draw_shape_chain_element(target, *shape_chain_element, scale);
	}
}

void draw_system::draw(const shared_ptr<sf::RenderWindow>& target, const shader_kind kind)
{
	shader_texture_.update(*target);
	const auto& shader = shader_system_->get_shader(kind);
	target->draw(shader_sprite_, &shader);
}
