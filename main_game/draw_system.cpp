#include "draw_system.h"

#include "text_system.h"

draw_system::draw_system()
{
	init_packs_map();
}

draw_system::~draw_system() = default;

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
			}
			while (FindNextFile(h_search, &wfd));
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

			lp_search_func(file, packs_map);
		}
	}
	while (FindNextFile(h_search, &wfd));
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
	search_files("Game/spritePacks/*.png", init_sprite_pack, true);
	sprite_pack::icon_without_space_size = Vector2f(
		float(packs_map.at(pack_tag::inventory).get_original_info(pack_part::areas, direction::DOWN, 1).frame.w),
		float(packs_map.at(pack_tag::inventory).get_original_info(pack_part::areas, direction::DOWN, 1).frame.h));
}

void draw_system::advanced_scale(sprite_chain_element& item, Sprite& sprite, const sprite_pack_structure::sprite& original_info, const float scale)
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

	if (!item.is_background && !item.unscaled)
	{
		if (!original_info.rotated)
		{
			sprite.scale(1, pow(scale, 1.0F / 6));
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (pow(scale, 1.0F / 6) - 1) * size_h);
		}
		else
		{
			sprite.scale(pow(scale, 1.0F / 6), 1);
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - (pow(scale, 1.0F / 6) - 1) * size_w);
		}
	}
}

std::vector<drawable_chain_element*> draw_system::upcast_chain(const std::vector<sprite_chain_element*>& chain)
{
	std::vector<drawable_chain_element*> result = {};
	for (auto sprite_chain_item : chain)
	{
		auto item = static_cast<drawable_chain_element*>(sprite_chain_item);
		result.push_back(item);
	}
	return result;
}

std::vector<sprite_chain_element*> draw_system::downcast_to_sprite_chain(const std::vector<drawable_chain_element*>& chain)
{
	std::vector<sprite_chain_element*> result = {};
	for (auto item : chain)
	{
		auto sprite_chain_item = dynamic_cast<sprite_chain_element*>(item);
		result.push_back(sprite_chain_item);
	}
	return result;
}

void draw_system::draw_sprite_chain_element(RenderTarget& target, sprite_chain_element* sprite_chain_item, const Vector2f camera_position, const Vector2f screen_center, const float scale)
{
	if (sprite_chain_item->pack_tag == pack_tag::empty)
	{
		return;
	}

	auto sprite = packs_map.at(sprite_chain_item->pack_tag).get_sprite(sprite_chain_item->pack_part, sprite_chain_item->direction, sprite_chain_item->number, sprite_chain_item->mirrored);
	if (sprite.getTextureRect() == IntRect())
	{
		return;
	}

	const auto original_info = packs_map.at(sprite_chain_item->pack_tag).get_original_info(sprite_chain_item->pack_part, sprite_chain_item->direction, sprite_chain_item->number);
	const Vector2f sprite_pos = {
		(sprite_chain_item->position.x - camera_position.x - sprite_chain_item->offset.x) * scale + screen_center.x,
		(sprite_chain_item->position.y - camera_position.y - sprite_chain_item->offset.y) * scale + screen_center.y
	};
	if (sprite_chain_item->anti_transparent)
	{
		sprite_chain_item->color.a = 255;
	}

	sprite.setColor(sprite_chain_item->color);
	sprite.rotate(sprite_chain_item->rotation);
	sprite.setPosition(sprite_pos);

	advanced_scale(*sprite_chain_item, sprite, original_info, scale);

	target.draw(sprite);
}

void draw_system::draw_text_chain_element(RenderTarget& target, text_chain_element* text_chain_item)
{
	text_system::draw_string(
		text_chain_item->string,
		text_chain_item->font,
		text_chain_item->character_size,
		text_chain_item->position.x - text_chain_item->offset.x,
		text_chain_item->position.y - text_chain_item->offset.y,
		target,
		text_chain_item->color);
}

void draw_system::draw(RenderTarget& target, const std::vector<drawable_chain_element*>& drawable_items, const float scale, const Vector2f camera_position)
{
	if (drawable_items.empty())
	{
		return;
	}

	std::vector<drawable_chain_element_with_shader*> drawable_items_with_shaders = shader_system::put_on_shaders(drawable_items);
	
	const auto screen_center = camera_position != Vector2f()
		                           ? Vector2f(target.getSize()) / 2.0F
		                           : Vector2f();

	for (auto drawable_chain_item : drawable_items_with_shaders)
	{
		if (!drawable_chain_item->drawable_chain_element->initialized)
		{
			continue;
		}

		const auto sprite_chain_item = dynamic_cast<sprite_chain_element*>(drawable_chain_item->drawable_chain_element);
		if (sprite_chain_item != nullptr)
		{
			draw_sprite_chain_element(target, sprite_chain_item, camera_position, screen_center, scale);
		}

		const auto text_chain_item = dynamic_cast<text_chain_element*>(drawable_chain_item->drawable_chain_element);
		if (text_chain_item != nullptr)
		{
			draw_text_chain_element(target, text_chain_item);
		}
	}
}
