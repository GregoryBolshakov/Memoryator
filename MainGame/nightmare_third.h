#pragma once

#include  "monster.h"

class nightmare_third : public monster
{
public:
	nightmare_third(std::string objectName, Vector2f centerPosition);
	~nightmare_third();
	Vector2f calculate_texture_offset() override;
	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
private:
	void doAttack(world_object* target) override;
	void endingPreviousAction() override;
	void on_sprite_change() override;
};
