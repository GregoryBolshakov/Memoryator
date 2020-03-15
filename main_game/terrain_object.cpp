#include "terrain_object.h"

#include "helper.h"

terrain_object::terrain_object(std::string object_name, Vector2f center_position) : static_object(std::move(object_name), center_position)
{
	is_terrain = true;
	mirrored_ = bool(rand() % 2);
}

terrain_object::~terrain_object()
= default;

void terrain_object::init_micro_blocks()
{	
	const auto currentMicroBlock = Vector2i(int(round(position_.x / micro_block_size.x)), int(round(position_.y / micro_block_size.y)));
	if (mirrored_)
		texture_box_offset_.x = conditional_size_units_.x - texture_box_offset_.x;
	
	if (is_multi_ellipse)
	{
		for (auto i = 0u; i < internal_ellipses.size(); i++)
		{
			micro_block_check_area_bounds_.x += getEllipseSize(i) / 2;
			micro_block_check_area_bounds_.y += getEllipseSize(i) / 2;
		}
		
		const auto i_end = int(micro_block_check_area_bounds_.x / micro_block_size.x);
		const auto j_end = int(micro_block_check_area_bounds_.y / micro_block_size.y);
		
		for (auto cnt = 0u; cnt < internal_ellipses.size(); cnt++)
		{
			auto i = int(-micro_block_check_area_bounds_.x / micro_block_size.x);
			while (i < i_end)
			{
				auto j = int(-micro_block_check_area_bounds_.y / micro_block_size.y);
				while (j <= j_end)
				{
					const auto pos = Vector2f(position_.x + i * micro_block_size.x, position_.y + j * micro_block_size.y);
					auto const f1 = internal_ellipses[cnt].first;
					auto const f2 = internal_ellipses[cnt].second;
					if (helper::getDist(pos, f1) + helper::getDist(pos, f2) < getEllipseSize(cnt) - sqrt(2.0f * micro_block_size.x) * 1.2f)
						locked_micro_blocks_.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
					j++;
				}
				i++;
			}
		}
	}
	else
	{
		micro_block_check_area_bounds_ = { getEllipseSize() / 2.0f, getEllipseSize() / 2.0f };

		const auto i_end = micro_block_check_area_bounds_.x / micro_block_size.x;
		const auto j_end = micro_block_check_area_bounds_.y / micro_block_size.y;

		auto i = int(-micro_block_check_area_bounds_.x / micro_block_size.x);
		while (i <= i_end)
		{
			auto j = int(-micro_block_check_area_bounds_.y / micro_block_size.y);
			while (j <= j_end)
			{
				const auto pos = Vector2f(position_.x + i * micro_block_size.x, position_.y + j * micro_block_size.y);
				auto const f1 = this->get_focus1();
				auto const f2 = this->get_focus2();
				if (helper::getDist(pos, f1) + helper::getDist(pos, f2) < this->getEllipseSize() - sqrt(2.0f * micro_block_size.x) * 1.2f)
					locked_micro_blocks_.emplace_back(currentMicroBlock.x + i, currentMicroBlock.y + j);
				j++;
			}
			i++;
		}
	}
}

float terrain_object::getEllipseSize(int i)
{
	if (is_multi_ellipse)
		return helper::getDist(internal_ellipses[i].first, internal_ellipses[i].second) * ellipse_size_multipliers[i];

	return helper::getDist(focus1_, focus2_) * ellipse_size_multipliers[0];
}

void terrain_object::setFocuses(std::vector<Vector2f> focuses)
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

bool terrain_object::isIntersected(Vector2f cur_position, Vector2f new_position) //const
{
	if (this->is_multi_ellipse)
		return false;

	if (this->is_dots_adjusted)
	{
		const auto d1 = this->get_dot1();
		const auto d2 = this->get_dot2();

		auto const triangle1 = helper::triangleArea(d1.x, d1.y, cur_position.x, cur_position.y, d2.x, d2.y);
		auto const triangle2 = helper::triangleArea(d1.x, d1.y, new_position.x, new_position.y, d2.x, d2.y);

		return (!helper::checkSigns(triangle1, triangle2)) &&
			((new_position.x >= std::min(d1.x, d2.x) - 5 && new_position.x <= std::max(d1.x, d2.x) + 5)
				&& (new_position.y >= std::min(d1.y, d2.y) - 5 && new_position.y <= std::max(d1.y, d2.y) + 5));
	}

	auto const f1 = this->get_focus1();
	auto const f2 = this->get_focus2();

	auto const position = new_position;

	return sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)) <= helper::getDist(f1, f2) * ellipse_size_multipliers[0];
}

std::vector<int> terrain_object::get_multi_ellipse_intersect(Vector2f position) const
{
	std::vector<int> ans;
	if (this->internal_ellipses.empty())	
		return ans;

	for (auto i = 0u; i < this->internal_ellipses.size(); i++)
	{
		auto const f1 = this->internal_ellipses[i].first;
		auto const f2 = this->internal_ellipses[i].second;

		if (sqrt(pow(position.x - f1.x, 2) + pow(position.y - f1.y, 2)) + sqrt(pow(position.x - f2.x, 2) + pow(position.y - f2.y, 2)/* - dynamic.radius*/) <= helper::getDist(f1, f2) * ellipse_size_multipliers[i])
			ans.push_back(i);
	}

	return ans;
}

Vector2f terrain_object::new_slipping_position_for_dots_adjusted(const Vector2f motion_vector, const float speed, const long long elapsed_time) const
{
	const auto dot1 = Vector2f(this->get_dot1()), dot2 = Vector2f(this->get_dot2());

	const auto directiveVector = Vector2f((dot2.x - dot1.x), (dot2.y - dot1.y));

	const auto motionVectorLength = sqrt(pow(motion_vector.x, 2) + pow(motion_vector.y, 2)),
	           directiveVectorLength = sqrt(pow(directiveVector.x, 2) + pow(directiveVector.y, 2));

	const auto cosAlpha = (motion_vector.x * directiveVector.x + motion_vector.y * directiveVector.y) /
		(motionVectorLength * directiveVectorLength);

	const auto k = (speed * float(elapsed_time)) / sqrt(pow(directiveVector.x, 2) + pow(directiveVector.y, 2));

	return { directiveVector.x * cosAlpha * k, directiveVector.y * cosAlpha * k };
}
