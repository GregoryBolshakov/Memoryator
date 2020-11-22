#pragma once

#include  "monster.h"

class nightmare_first : public monster
{
public:
	nightmare_first(std::string objectName, Vector2f centerPosition);
	~nightmare_first();
	Vector2f calculate_texture_offset() override;
	std::vector<unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsed_time) override;
private:
	void doAttack(world_object* target) override;
};

