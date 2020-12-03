#pragma once

#include "sprite_chain_element.h"

#include <SFML/Graphics.hpp>

enum class button_tag {
	playTag = 0, continueTag = 1, newRunTag = 2, settingsTag = 3, exitTag = 4, buildStartTag = 5, menuTag = 6, openMenu = 7,
	nextPage = 16, previousPage = 17, bookCover = 18, bookmark = 19, pageBackground = 20, pagePattern = 21, bookmarksList = 22, sketching = 46,
	bookmarkMobs = 31, bookmarkItems = 32, bookmarkWorld = 33, bookmarkFlowers = 34, bookmarkWreathes = 35, bookmarkNightmare = 36, bookButtonTag = 40,
	bookStandTag = 41, bookGlowTag = 42, bookLightningTag = 43, hpLineTag = 44, hpFrameTag = 45, cell = 47, cellSelected = 48, iconFrame1 = 49, iconFrame2 = 50, plus = 51, makeWreath = 52,
	chamomile = 401, yarrow = 402, fern = 403, mugwort = 404, poppy = 405,
	hero = 101, hare = 102, owl = 103, deer = 104, fox = 105, bear = 106, wolf = 107, monster = 108, owlBoss = 109,
	heroBag = 201
};

using std::unique_ptr, std::make_unique;

class button
{
public:
	button(pack_tag pack,
		pack_part pack_part,
		int default_sprite_number,
		int selected_sprite_number,
		int pressed_sprite_number,
		sf::Vector2f position,
		sf::Vector2f size,
		bool is_selectable,
		button_tag tag,
		sf::Vector2f offset = sf::Vector2f());

	unique_ptr<sprite_chain_element> prepare_sprite();
	
	void become_gray();
	
	void stop_being_gray();
	
	bool is_selected(sf::Vector2f mouse_pos);

	[[nodiscard]] sf::Vector2f get_position()
	{
		return button_enabled_.position;
	}

	[[nodiscard]] sf::Vector2f get_size()
	{
		return button_enabled_.size;
	}

	[[nodiscard]] sf::Vector2f get_offset()
	{
		return button_enabled_.offset;
	}

	[[nodiscard]] sf::FloatRect get_global_bounds()
	{
		return {get_position(), get_size()};
	}

	void set_position(sf::Vector2f position);

	void set_size(sf::Vector2f size);

	[[nodiscard]] button_tag get_tag() const
	{
		return tag_;
	}

	[[nodiscard]] bool get_gray_state() const
	{
		return is_gray_;
	}	
	
	bool is_selectable{};
	bool is_active = true;

private:
	sprite_chain_element button_enabled_;
	sprite_chain_element button_selected_;
	sprite_chain_element button_pressed_;
	button_tag tag_{};
	bool selected_ = false;
	bool is_gray_ = false;
};
