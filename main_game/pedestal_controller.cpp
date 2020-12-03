#include "pedestal_controller.h"
#include "terrain_object.h"
#include "shape_chain_element.h"

#include <fstream>
#include <iomanip>

#include "helper.h"
#include "object_initializer.h"

pedestal_controller::pedestal_controller() :
	  filed_figure_(shape_chain_element(sf::Vector2f(0, 0), size_, size_ / 2, false, sf::Color(255, 255, 255, 80)))
	, focus_figure_(sf::Vector2f(0, 0), size_.x / 2, sf::Vector2f(size_.x / 2, size_.x / 2), false, sf::Color(255, 71, 71, 180))
	, center_figure_(sf::Vector2f(0, 0), size_.x / 2, sf::Vector2f(size_.x / 2, size_.x / 2), false, sf::Color(60, 80, 176, 180))
	, last_mouse_pos_(sf::Vector2f(sf::Mouse::getPosition()))
{
}

pedestal_controller::~pedestal_controller()
= default;

void pedestal_controller::start(std::weak_ptr<terrain_object> object)
{
	ready_to_start = false;
	focuses_.clear();

	bound_object_ = std::move(object);
	const auto bound_shared_object = bound_object_.lock();
	if (!bound_shared_object)
		return;
	
	if (bound_shared_object->is_multi_ellipse)
		for (auto& internal_ellipse : bound_shared_object->internal_ellipses)
		{
			focuses_.push_back(internal_ellipse.first);
			focuses_.push_back(internal_ellipse.second);
		}
	else
	{
		focuses_.push_back(bound_shared_object->get_focus1());
		focuses_.push_back(bound_shared_object->get_focus2());
	}
	center_position_ = bound_shared_object->get_position();
	running_ = true;
}

void pedestal_controller::stop()
{
	write_to_file();
	ready_to_start = false;
	bound_object_.reset();
	running_ = false;
}

void pedestal_controller::write_to_file()
{
	const auto bound_shared_object = bound_object_.lock();
	if (!bound_shared_object)
		return;

	const sf::Vector2f offset = { (bound_shared_object->get_offset().x + center_position_.x - bound_shared_object->get_position().x) / bound_shared_object->get_size().x,
		(bound_shared_object->get_offset().y + center_position_.y - bound_shared_object->get_position().y) / bound_shared_object->get_size().y };

	std::ofstream stream_out("Game/pedestalsInfo.txt");

	stream_out.clear();
	stream_out << std::setprecision(3) << "Tag: " << object_initializer::mapped_tags.at(bound_shared_object->tag) + " " << '\n';
	stream_out << std::setprecision(3) << "Type: " << bound_shared_object->get_kind() << '\n';
	stream_out << std::setprecision(3) << "Mirrored: " << bound_shared_object->get_mirrored_state() << '\n';

	if (bound_shared_object->get_mirrored_state())
		stream_out << std::setprecision(3) << "OffsetX: " << 1 - offset.x << " " << "OffsetY: " << offset.y << '\n' << '\n';
	else
		stream_out << std::setprecision(3) << "OffsetX: " << offset.x << " " << "OffsetY: " << offset.y << '\n' << '\n';

	for (auto i = 0u; i < focuses_.size() / 2; i++)
	{
		stream_out << std::setprecision(3) << "Focus" << i * 2 << " X: " << (focuses_[i * 2].x - center_position_.x) / bound_shared_object->get_size().x << " " << "Focus" << i * 2 << " Y: " << (focuses_[i * 2].y - center_position_.y) / bound_shared_object->get_size().y << '\n';
		stream_out << std::setprecision(3) << "Focus" << i * 2 + 1 << " X: " << (focuses_[i * 2 + 1].x - center_position_.x) / bound_shared_object->get_size().x << " " << "Focus" << i * 2 + 1 << " Y: " << (focuses_[i * 2 + 1].y - center_position_.y) / bound_shared_object->get_size().y << '\n';
		stream_out << std::setprecision(3) << "Ellipse size: " << bound_shared_object->ellipse_size_multipliers[i] << '\n' << '\n';
	}
	stream_out.close();
}

void pedestal_controller::handle_events(sf::Event & event)
{
	// double click to reset center
	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (double_click_timer_ <= 200000)
		{
			const auto bound_shared_object = bound_object_.lock();
			if (bound_shared_object)
			{
				if (bound_shared_object->is_multi_ellipse)
					center_position_ = bound_shared_object->get_position();
				else
					center_position_ = { (focuses_[0].x + focuses_[1].x) / 2, (focuses_[0].y + focuses_[1].y) / 2 };
			}
		}
		double_click_timer_ = 0;

		selected_ellipse_ = -1;
	}
	//------------------------------
}

void pedestal_controller::interact(const long long elapsed_time)
{
	const auto bound_shared_object = bound_object_.lock();
	if (!bound_shared_object)
		return;

	this->elapsed_time_ = elapsed_time;
	if (!bound_shared_object || !running_)
		return;

	// moving focuses
	const auto mouse_pos = sf::Vector2f(sf::Mouse::getPosition());
	const auto mouse_world_pos = world_metrics::screen_to_world_position(mouse_pos);

	// selecting focus to move
	if (!selected_center_ && selected_focus_ == -1 && (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)))
	{
		for (auto i = 0u; i < focuses_.size(); i++)
		{
			if (world_metrics::get_dist(mouse_world_pos, focuses_[i]) <= focus_figure_.radius)
			{
				selected_focus_ = i;
				break;
			}
		}
	}
	else
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)))
			selected_focus_ = -1;
	//------------------------

	// moving selected focus
	if (selected_focus_ != -1)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			focuses_[selected_focus_].x = mouse_world_pos.x;
			focus_figure_.position = sf::Vector2f(mouse_pos.x, focus_figure_.position.y);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			focuses_[selected_focus_].y = mouse_world_pos.y;
			focus_figure_.position = sf::Vector2f(focus_figure_.position.x, mouse_pos.y);
		}
		// alignment
		if (selected_focus_ % 2 == 0 && abs(focuses_[selected_focus_].x - focuses_[selected_focus_ + 1].x) < size_.x / 2.0f)
			focuses_[selected_focus_].x = focuses_[selected_focus_ + 1].x;
		if (selected_focus_ % 2 == 0 && abs(focuses_[selected_focus_].y - focuses_[selected_focus_ + 1].y) < size_.y / 2.0f)
			focuses_[selected_focus_].y = focuses_[selected_focus_ + 1].y;
		if (selected_focus_ % 2 == 1 && abs(focuses_[selected_focus_].x - focuses_[selected_focus_ - 1].x) < size_.x / 2.0f)
			focuses_[selected_focus_].x = focuses_[selected_focus_ - 1].x;
		if (selected_focus_ % 2 == 1 && abs(focuses_[selected_focus_].y - focuses_[selected_focus_ - 1].y) < size_.y / 2.0f)
			focuses_[selected_focus_].y = focuses_[selected_focus_ - 1].y;
		//----------
	}
	//----------------------

	// selecting center
	if (selected_focus_ == -1 && !selected_center_ && (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)))
	{
		if (world_metrics::get_dist(mouse_world_pos, center_position_) <= focus_figure_.radius)
			selected_center_ = true;
	}
	else
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)))
			selected_center_ = false;
	//-----------------

	// moving center
	if (selected_center_)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			center_position_.x = mouse_world_pos.x;
			center_figure_.position = sf::Vector2f(mouse_pos.x, center_figure_.position.y);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			center_position_.y = mouse_world_pos.y;
			center_figure_.position = sf::Vector2f(center_figure_.position.x, mouse_pos.y);
		}
	}
	//----------------------

	// resize ellipse
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
	{
		if (selected_ellipse_ == -1)
		{
			float min_dist = 1e5;
			for (auto i = 0u; i < focuses_.size() / 2; i++)
			{
				if (world_metrics::get_dist(mouse_world_pos, focuses_[i * 2]) + world_metrics::get_dist(mouse_world_pos, focuses_[i * 2 + 1]) <= min_dist)
				{
					min_dist = world_metrics::get_dist(mouse_world_pos, focuses_[i * 2]) + world_metrics::get_dist(mouse_world_pos, focuses_[i * 2 + 1]);
					selected_ellipse_ = i;
				}
			}
		}
		const auto center = sf::Vector2f((focuses_[selected_ellipse_ * 2].x + focuses_[selected_ellipse_ * 2 + 1].x) / 2,
			(focuses_[selected_ellipse_ * 2].y + focuses_[selected_ellipse_ * 2 + 1].y) / 2);
		bound_shared_object->ellipse_size_multipliers[selected_ellipse_] += bound_shared_object->ellipse_size_multipliers[selected_ellipse_] * (world_metrics::get_dist(mouse_pos, center) - world_metrics::get_dist(last_mouse_pos_, center)) / 100;
	}
	last_mouse_pos_ = mouse_pos;
	//---------------

	double_click_timer_ += elapsed_time;
	bound_shared_object->set_focuses(focuses_);
}

std::vector<std::unique_ptr<drawable_chain_element>> pedestal_controller::prepare_sprites()
{
	std::vector<std::unique_ptr<drawable_chain_element>> result;
	
	if (!running_)
		return result;

	const auto bound_shared_object = bound_object_.lock(); // no selected object
	if (!bound_shared_object)
		return result;

	sf::Vector2f area_bounds = { 0, 0 };
	for (auto i = 0u; i < focuses_.size() / 2; i++)
	{
		area_bounds.x += world_metrics::get_dist(focuses_[i * 2], focuses_[i * 2 + 1]) * bound_shared_object->ellipse_size_multipliers[i] / 2;
		area_bounds.y += world_metrics::get_dist(focuses_[i * 2], focuses_[i * 2 + 1]) * bound_shared_object->ellipse_size_multipliers[i] / 2;
	}
	const auto upper_left = sf::Vector2f(std::min(focuses_[0].x, focuses_[1].x) - area_bounds.x, std::min(focuses_[0].y, focuses_[1].y) - area_bounds.y);
	const auto bottom_right = sf::Vector2f(std::max(focuses_[0].x, focuses_[1].x) + area_bounds.x, std::max(focuses_[0].y, focuses_[1].y) + area_bounds.y);

	const auto x_end = bottom_right.x / size_.x;
	const auto y_end = bottom_right.y / size_.y;

	auto x = upper_left.x / size_.x;
	while (x <= x_end)
	{
		auto y = upper_left.y / size_.y;
		while (y <= y_end)
		{
			for (auto i = 0u; i < focuses_.size() / 2; i++)
			{
				if (sqrt(pow(x * size_.x - focuses_[i * 2].x, 2) + pow(y * size_.x - focuses_[i * 2].y, 2)) +
					sqrt(pow(x * size_.x - focuses_[i * 2 + 1].x, 2) + pow(y * size_.x - focuses_[i * 2 + 1].y, 2)) <= world_metrics::get_dist(focuses_[i * 2], focuses_[i * 2 + 1]) * bound_shared_object->ellipse_size_multipliers[i])
				{
					filed_figure_.position = sf::Vector2f(x * size_.x, y * size_.y);
					result.push_back(std::make_unique<shape_chain_element>(filed_figure_));
				}
			}
			y++;
		}
		x++;
	}
			
	for (auto& focus : focuses_)
	{
		focus_figure_.position = focus;
		result.push_back(std::make_unique<shape_chain_element>(focus_figure_));
	}
	center_figure_.position = center_position_;
	result.push_back(std::make_unique<shape_chain_element>(center_figure_));

	return result;
}

