#include "terrain_object.h"
#include "world_metrics.h"

terrain_object::terrain_object(std::string name, const sf::Vector2f position, const int kind) : static_object(std::move(name), position, kind)
{
	is_terrain = true;
	mirrored_ = static_cast<bool>(rand() % 2);
}

terrain_object::~terrain_object()
= default;

void terrain_object::init_route_blocks()
{	
	const auto currentMicroBlock = sf::Vector2i(int(round(position_.x / micro_block_size.x)), int(round(position_.y / micro_block_size.y)));
	if (mirrored_)
		offset_.x = size_.x - offset_.x;
	
	if (is_multi_ellipse)
	{
		sf::Vector2f route_block_area_bounds{};
		for (auto i = 0u; i < internal_ellipses.size(); i++)
		{
			route_block_area_bounds.x += get_ellipse_size(i) / 2;
			route_block_area_bounds.y += get_ellipse_size(i) / 2;
		}
		
		const auto i_end = int(route_block_area_bounds.x / micro_block_size.x);
		const auto j_end = int(route_block_area_bounds.y / micro_block_size.y);
		
		for (auto cnt = 0u; cnt < internal_ellipses.size(); cnt++)
		{
			auto i = int(-route_block_area_bounds.x / micro_block_size.x);
			while (i < i_end)
			{
				auto j = int(-route_block_area_bounds.y / micro_block_size.y);
				while (j <= j_end)
				{
					const auto pos = sf::Vector2f(position_.x + i * micro_block_size.x, position_.y + j * micro_block_size.y);
					auto const f1 = internal_ellipses[cnt].first;
					auto const f2 = internal_ellipses[cnt].second;
					if (world_metrics::get_dist(pos, f1) + world_metrics::get_dist(pos, f2) < get_ellipse_size(cnt) - sqrt(2.0f * micro_block_size.x) * 1.2f)
						locked_route_blocks_.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
					j++;
				}
				i++;
			}
		}
	}
	else
	{
		const sf::Vector2f route_block_area_bounds = { get_ellipse_size() / 2.0f, get_ellipse_size() / 2.0f };

		const auto i_end = route_block_area_bounds.x / micro_block_size.x;
		const auto j_end = route_block_area_bounds.y / micro_block_size.y;

		auto i = int(-route_block_area_bounds.x / micro_block_size.x);
		while (i <= i_end)
		{
			auto j = int(-route_block_area_bounds.y / micro_block_size.y);
			while (j <= j_end)
			{
				const auto pos = sf::Vector2f(position_.x + i * micro_block_size.x, position_.y + j * micro_block_size.y);
				auto const f1 = this->get_focus1();
				auto const f2 = this->get_focus2();
				if (world_metrics::get_dist(pos, f1) + world_metrics::get_dist(pos, f2) < this->get_ellipse_size() - sqrt(2.0f * micro_block_size.x) * 1.2f)
					locked_route_blocks_.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
				j++;
			}
			i++;
		}
	}
}

void terrain_object::set_position(const sf::Vector2f position)
{
	const auto shift_vector = position - position_;
	if (is_multi_ellipse)
	{
		for (auto& ellipse : internal_ellipses)
		{
			ellipse.first += shift_vector;
			ellipse.second += shift_vector;
		}
	}
	else
	{
		focus1_ += shift_vector;
		focus2_ += shift_vector;
	}

	position_ = sf::Vector2f(position);
}

float terrain_object::get_ellipse_size(int i)
{
	if (is_multi_ellipse)
		return world_metrics::get_dist(internal_ellipses[i].first, internal_ellipses[i].second) * ellipse_size_multipliers[i];

	return world_metrics::get_dist(focus1_, focus2_) * ellipse_size_multipliers[0];
}

void terrain_object::set_focuses(std::vector<sf::Vector2f> focuses)
{
	if (is_multi_ellipse)
	{
		for (auto i = 0u; i < focuses.size() / 2; i++)
		{
			internal_ellipses[i].first = focuses[i * 2];
			internal_ellipses[i].second = focuses[i * 2 + 1];
		}
	}
	else
	{
		focus1_ = focuses[0];
		focus2_ = focuses[1];
	}
}

bool terrain_object::is_intersected(const sf::Vector2f position) const
{
	if (this->is_multi_ellipse)
		return false;

	auto const f1 = this->get_focus1();
	auto const f2 = this->get_focus2();

	const auto distance = world_metrics::get_dist(position, f1) + world_metrics::get_dist(position, f2);
	const auto radius = world_metrics::get_dist(f1, f2) * ellipse_size_multipliers[0];
	return  distance <= radius;
}

std::vector<int> terrain_object::get_multi_ellipse_intersect(sf::Vector2f position) const
{
	std::vector<int> ans;
	if (this->internal_ellipses.empty())
		return ans;

	for (auto i = 0u; i < this->internal_ellipses.size(); i++)
	{
		auto const f1 = this->internal_ellipses[i].first;
		auto const f2 = this->internal_ellipses[i].second;

		if (sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)/* - dynamic.radius*/) <= world_metrics::get_dist(f1, f2) * ellipse_size_multipliers[i])
			ans.push_back(i);
	}

	return ans;
}
