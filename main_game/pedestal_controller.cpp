#include "pedestal_controller.h"

#include <fstream>
#include <iomanip>

#include "helper.h"
#include "object_initializer.h"

pedestal_controller::pedestal_controller()
{
	filed_figure_.setSize(sf::Vector2f(size_));
	filed_figure_.setFillColor(sf::Color(255, 255, 255, 80));
	focus_figure_.setRadius(size_.x);
	focus_figure_.setFillColor(sf::Color(255, 71, 71, 180));
	center_figure_.setRadius(size_.x);
	center_figure_.setFillColor(sf::Color(60, 80, 176, 180));
	last_mouse_pos_ = sf::Vector2f(sf::Mouse::getPosition());
}

pedestal_controller::~pedestal_controller()
= default;

void pedestal_controller::start(terrain_object * object)
{
	ready_to_start = false;
	bound_object_ = object;
	focuses_.clear();
	if (bound_object_->is_multi_ellipse)
		for (auto& internal_ellipse : bound_object_->internal_ellipses)
		{
			focuses_.push_back(internal_ellipse.first);
			focuses_.push_back(internal_ellipse.second);
		}
	else
	{
		focuses_.push_back(bound_object_->get_focus1());
		focuses_.push_back(bound_object_->get_focus2());
	}
	center_position_ = bound_object_->get_position();
	running_ = true;
}

void pedestal_controller::stop()
{
	write_to_file();
	ready_to_start = false;
	bound_object_ = nullptr;
	running_ = false;
}

void pedestal_controller::write_to_file()
{
	if (!bound_object_)
		return;

	const sf::Vector2f offset = { (bound_object_->get_texture_offset().x + center_position_.x - bound_object_->get_position().x) / bound_object_->get_texture_size().x,
		(bound_object_->get_texture_offset().y + center_position_.y - bound_object_->get_position().y) / bound_object_->get_texture_size().y };

	std::ofstream stream_out("Game/pedestalsInfo.txt");

	stream_out.clear();
	stream_out << std::setprecision(3) << "Tag: " << object_initializer::mapped_tags.at(bound_object_->tag) + " " << '\n';
	stream_out << std::setprecision(3) << "Type: " << bound_object_->get_type() << '\n';
	stream_out << std::setprecision(3) << "Mirrored: " << bound_object_->get_mirrored_state() << '\n';

	if (bound_object_->get_mirrored_state())
		stream_out << std::setprecision(3) << "OffsetX: " << 1 - offset.x << " " << "OffsetY: " << offset.y << '\n' << '\n';
	else
		stream_out << std::setprecision(3) << "OffsetX: " << offset.x << " " << "OffsetY: " << offset.y << '\n' << '\n';

	for (auto i = 0u; i < focuses_.size() / 2; i++)
	{
		stream_out << std::setprecision(3) << "Focus" << i * 2 << " X: " << (focuses_[i * 2].x - center_position_.x) / bound_object_->get_texture_size().x << " " << "Focus" << i * 2 << " Y: " << (focuses_[i * 2].y - center_position_.y) / bound_object_->get_texture_size().y << '\n';
		stream_out << std::setprecision(3) << "Focus" << i * 2 + 1 << " X: " << (focuses_[i * 2 + 1].x - center_position_.x) / bound_object_->get_texture_size().x << " " << "Focus" << i * 2 + 1 << " Y: " << (focuses_[i * 2 + 1].y - center_position_.y) / bound_object_->get_texture_size().y << '\n';
		stream_out << std::setprecision(3) << "Ellipse size: " << bound_object_->ellipse_size_multipliers[i] << '\n' << '\n';
	}
	stream_out.close();
}

void pedestal_controller::handle_events(sf::Event & event)
{
	if (!bound_object_)
		return;
	// double click to reset center
	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (double_click_timer_ <= long(2e5))
		{
			if (bound_object_->is_multi_ellipse)
				center_position_ = bound_object_->get_position();
			else
				center_position_ = { (focuses_[0].x + focuses_[1].x) / 2, (focuses_[0].y + focuses_[1].y) / 2 };
		}
		double_click_timer_ = 0;

		selected_ellipse_ = -1;
	}
	//------------------------------
}

void pedestal_controller::interact(long long elapsed_time, sf::Event event)
{
	this->elapsed_time_ = elapsed_time;
	if (!bound_object_ || !running_)
		return;

	// moving focuses
	const auto mouse_pos = sf::Vector2f(sf::Mouse::getPosition());
	const auto mouse_world_pos = sf::Vector2f((mouse_pos.x - helper::GetScreenSize().x / 2 + camera_position_.x * scale_factor_) / scale_factor_,
		(mouse_pos.y - helper::GetScreenSize().y / 2 + camera_position_.y * scale_factor_) / scale_factor_);

	// selecting focus to move
	if (!selected_center_ && selected_focus_ == -1 && (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)))
	{
		for (auto i = 0u; i < focuses_.size(); i++)
		{
			if (helper::getDist(mouse_world_pos, focuses_[i]) * scale_factor_ <= focus_figure_.getRadius())
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
			focus_figure_.setPosition(mouse_pos.x, focus_figure_.getPosition().y);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			focuses_[selected_focus_].y = mouse_world_pos.y;
			focus_figure_.setPosition(focus_figure_.getPosition().x, mouse_pos.y);
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
		if (helper::getDist(mouse_world_pos, center_position_) <= focus_figure_.getRadius())
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
			center_figure_.setPosition(mouse_pos.x, center_figure_.getPosition().y);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			center_position_.y = mouse_world_pos.y;
			center_figure_.setPosition(center_figure_.getPosition().x, mouse_pos.y);
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
				if (helper::getDist(mouse_world_pos, focuses_[i * 2]) + helper::getDist(mouse_world_pos, focuses_[i * 2 + 1]) <= min_dist)
				{
					min_dist = helper::getDist(mouse_world_pos, focuses_[i * 2]) + helper::getDist(mouse_world_pos, focuses_[i * 2 + 1]);
					selected_ellipse_ = i;
				}
			}
		}
		const auto center = sf::Vector2f((focuses_[selected_ellipse_ * 2].x + focuses_[selected_ellipse_ * 2 + 1].x) / 2,
			(focuses_[selected_ellipse_ * 2].y + focuses_[selected_ellipse_ * 2 + 1].y) / 2);
		bound_object_->ellipse_size_multipliers[selected_ellipse_] += bound_object_->ellipse_size_multipliers[selected_ellipse_] * (helper::getDist(mouse_pos, center) - helper::getDist(last_mouse_pos_, center)) / 100;
	}
	last_mouse_pos_ = mouse_pos;
	//---------------

	double_click_timer_ += elapsed_time;
	bound_object_->set_focuses(focuses_);
}

void pedestal_controller::draw(const shared_ptr<sf::RenderWindow>& target, sf::Vector2f camera_position, float scale_factor)
{
	this->camera_position_ = camera_position;
	this->scale_factor_ = scale_factor;
	if (!bound_object_ || !running_)
		return;

	sf::Vector2f area_bounds = { 0, 0 };
	for (auto i = 0u; i < focuses_.size() / 2; i++)
	{
		area_bounds.x += helper::getDist(focuses_[i * 2], focuses_[i * 2 + 1]) * bound_object_->ellipse_size_multipliers[i] / 2;
		area_bounds.y += helper::getDist(focuses_[i * 2], focuses_[i * 2 + 1]) * bound_object_->ellipse_size_multipliers[i] / 2;
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
					sqrt(pow(x * size_.x - focuses_[i * 2 + 1].x, 2) + pow(y * size_.x - focuses_[i * 2 + 1].y, 2)) <= helper::getDist(focuses_[i * 2], focuses_[i * 2 + 1]) * bound_object_->ellipse_size_multipliers[i])
				{
					filed_figure_.setPosition(
						(x * size_.x - camera_position.x - size_.x / 2.0f) * scale_factor + helper::GetScreenSize().x / 2,
						(y * size_.y - camera_position.y - size_.y / 2.0f) * scale_factor + helper::GetScreenSize().y / 2);
					target->draw(filed_figure_);
				}
			}
			y++;
		}
		x++;
	}
			
	for (auto& focus : focuses_)
	{
		focus_figure_.setPosition((focus.x - camera_position.x - size_.x) * scale_factor + helper::GetScreenSize().x / 2,
			(focus.y - camera_position.y - size_.y) * scale_factor + helper::GetScreenSize().y / 2);
		target->draw(focus_figure_);
	}
	center_figure_.setPosition((center_position_.x - camera_position.x - size_.x) * scale_factor + helper::GetScreenSize().x / 2,
		(center_position_.y - camera_position.y - size_.y) * scale_factor + helper::GetScreenSize().y / 2);
	target->draw(center_figure_);
}

