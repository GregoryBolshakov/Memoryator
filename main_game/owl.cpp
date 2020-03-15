#include "owl.h"

#include "fern.h"
#include "forest_tree.h"

using namespace sf;

owl::owl(const std::string& object_name, Vector2f center_position) : neutral_mob(object_name, center_position)
{
	conditional_size_units_ = { 280, 200 };
	move_system.default_speed = 0.0006f;
	move_system.speed = 0.0006f;
	radius_ = 70;
	sight_range = 720;
	health_point_ = 50;
	time_for_new_hit_self = long(6e5);
	to_save_name_ = "owl";
	tag = entity_tag::owl;
	move_system.can_crash_into_static = false;
	z_coordinate_ = 10;
}

owl::~owl()
= default;

Vector2f owl::calculate_texture_offset()
{
	texture_box_.width = texture_box_.width * get_scale_ratio().x;
	texture_box_.height = texture_box_.height * get_scale_ratio().y;
	return { texture_box_.width / 2, texture_box_.height * 7 / 8 };
}

void owl::behavior_with_static(world_object* target, long long elapsed_time)
{
	/*if (current_action_ == absorbs)
		return;

	if (target->tag == entity_tag::fern && helper::getDist(position_, target->get_position()) <= sight_range * 2 && time_after_fear_ >= fear_time_)
	{
		if (!(bound_target_ && bound_target_->tag == entity_tag::hero && (helper::getDist(position_, bound_target_->get_position()) <= sight_range ||
			helper::getDist(target->get_position(), bound_target_->get_position()) <= sight_range)))
		{
			bound_target_ = target;
			distance_to_nearest_ = helper::getDist(position_, target->get_position());
		}
	}
	else*/
	if (target->tag == entity_tag::tree)
	{
		const auto nearestTreeCasted = dynamic_cast<forest_tree*>(target);
		if (nearestTreeCasted && !(bound_target_ && bound_target_->tag == entity_tag::fern))
			if (bound_target_ != nullptr && bound_target_->tag == entity_tag::hero && helper::getDist(nearestTreeCasted->get_owl_landing_pos(), position_) < helper::getDist(nearestTreeCasted->get_owl_landing_pos(), bound_target_->get_position()) &&
				helper::getDist(nearestTreeCasted->get_owl_landing_pos(), bound_target_->get_position()) > helper::getDist(position_, bound_target_->get_position()))
				if ((nearest_tree_ != nullptr && helper::getDist(nearestTreeCasted->get_owl_landing_pos(), position_) < helper::getDist(nearest_tree_->get_position(), position_)) || nearest_tree_ == nullptr)
					if (helper::getDist(nearestTreeCasted->get_owl_landing_pos(), bound_target_->get_position()) > sight_range * 1.2)
						nearest_tree_ = target;							
	}
}

void owl::behavior_with_dynamic(dynamic_object* target, long long elapsedTime)
{
	const auto distance_to_target = helper::getDist(position_, target->get_position());

	if (distance_to_target <= sight_range && target->tag == entity_tag::hero)
		bound_target_ = target;
}

void owl::behavior(long long elapsed_time)
{
	ending_previous_action();
	fight_interact(elapsed_time);
	
	if (die_check())
		return;

	// first-priority actions
	if (bound_target_ && bound_target_->tag == entity_tag::hero && helper::getDist(position_, bound_target_->get_position()) <= sight_range)
			time_after_fear_ = 0;	
	else
		time_after_fear_ += elapsed_time;

	if (current_action_ == absorbs)
	{
		move_system.lax_move_position = position_;
		return;
	}
	//-----------------------

	if (bound_target_ == nullptr || bound_target_ && bound_target_->tag == entity_tag::hero && helper::getDist(position_, bound_target_->get_position()) > sight_range)
	{
		const auto nearest_tree_casted = dynamic_cast<forest_tree*>(nearest_tree_);
		if (nearest_tree_ && helper::getDist(position_, nearest_tree_casted->get_owl_landing_pos()) > radius_)
			{
				change_action(soar, false, true);
				move_system.lax_move_position = nearest_tree_casted->get_owl_landing_pos();
				direction_system.side = direction_system.calculate_side(position_, move_system.lax_move_position, elapsed_time);
				return;
			}

		change_action(relax, true, true);
		direction_system.direction = direction::STAND;
		move_system.lax_move_position = position_;
		return;
	}

	// bouncing to a trap
	if (bound_target_ && bound_target_->tag == entity_tag::fern)
	{
		direction_system.side = direction_system.calculate_side(position_, bound_target_->get_position(), elapsed_time);
		if (helper::getDist(position_, bound_target_->get_position()) <= radius_)
		{
			const auto trap = dynamic_cast<fern*>(bound_target_);
			if (direction_system.side == right)
				mirrored_ = true;
			else
				mirrored_ = false;
			position_ = trap->getEnterPosition();
			change_action(absorbs, true, false);
			move_system.lax_move_position = position_;
		}
		else
		{
			change_action(soar, false, true);
			move_system.lax_move_position = bound_target_->get_position();
		}
	}
	//-------------------

	// runaway from enemy
	if (bound_target_ && bound_target_->tag == entity_tag::hero)
	{
		const float distanceToTarget = helper::getDist(this->position_, bound_target_->get_position());
		direction_system.side = direction_system.calculate_side(position_, move_system.lax_move_position, elapsed_time);
		move_system.speed = std::max(move_system.default_speed, (move_system.default_speed * 10) * (1 - (helper::getDist(position_, bound_target_->get_position()) / sight_range * 1.5f)));
		animation_speed_ = std::max(0.0008f, 0.0008f * move_system.speed / move_system.default_speed);
		if (distanceToTarget <= sight_range)
		{
			change_action(move, false, true);
			float k = (sight_range * 1.3f / (sqrt(pow(bound_target_->get_position().x - position_.x, 2) + pow(bound_target_->get_position().y - position_.y, 2))));
			move_system.lax_move_position = Vector2f(position_.x + (position_.x - bound_target_->get_position().x) * k, position_.y + (position_.y - bound_target_->get_position().y) * k);
		}
	}	
	//-------------------

	if (current_action_ != absorbs)
	{
		distance_to_nearest_ = 10e6;
		bound_target_ = nullptr;
		nearest_tree_ = nullptr;
	}
}

Vector2f owl::get_build_position(std::vector<world_object*> visible_items, float scale_factor, Vector2f camera_position)
{
	return { -1, -1 };
}

int owl::get_build_type(Vector2f oun_pos, Vector2f other_pos)
{
	return 1;
}

void owl::ending_previous_action()
{
	if (last_action_ == common_hit)
		current_action_ = relax;
	if (last_action_ == absorbs)
	{
		auto trap = dynamic_cast<fern*>(bound_target_);
		trap->inventory.emplace_back(entity_tag::owl, 1);
		delete_promise_on();
	}
	last_action_ = relax;
}

void owl::jerk(float power, float deceleration, Vector2f destination_point)
{
	return;
}

std::vector<sprite_chain_element*> owl::prepare_sprites(long long elapsed_time)
{
	auto body = new sprite_chain_element(pack_tag::owl, pack_part::head, direction::DOWN, 1, position_, conditional_size_units_, texture_box_offset_, color, mirrored_, false);
	body->z_coordinate = z_coordinate_;
	animation_speed_ = 12;

	body->mirrored = mirrored_;

	switch (current_action_)
	{
	case relax:
	{
		body->animation_length = 1;
		current_sprite_[0] = 1;		
	}
	case head:
	{
		body->animation_length = 16;
		body->pack_part = pack_part::head;
		break;
	}
	case landing:
	{
		body->animation_length = 14;
		body->pack_part = pack_part::landing;
		break;
	}
	case startle:
	{
		body->animation_length = 14;
		body->pack_part = pack_part::startle;
		break;
	}
	case takeoff:
	{
		body->animation_length = 10;
		body->pack_part = pack_part::takeoff;
		break;
	}
	case flap:
	{
		body->animation_length = 8;
		direction_system.set_mob_direction(move_system.move_offset, elapsed_time);
		body->direction = direction_system.direction;
		body->pack_part = pack_part::flap;
		break;
	}
	case soar:
	{
		body->animation_length = 7;
		direction_system.set_mob_direction(move_system.move_offset, elapsed_time);
		body->direction = direction_system.direction;
		body->pack_part = pack_part::soar;
		break;
	}
	case dead:
	{
		body->animation_length = 1;
		current_sprite_[0] = 1;
		break;
	}
	default:;
	}

	body->number = current_sprite_[0];

	time_for_new_sprite_ += elapsed_time;

	if (time_for_new_sprite_ >= long(1e6 / animation_speed_))
	{
		time_for_new_sprite_ = 0;

		if (++current_sprite_[0] > body->animation_length)
		{
			last_action_ = current_action_;
			current_sprite_[0] = 1;
		}
	}

	return { body };
}