#pragma once

#include  "monster.h"

class nightmare_first : public monster
{
public:
	nightmare_first(std::string objectName, Vector2f centerPosition);
	~nightmare_first();
	Vector2f calculateTextureOffset() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
private:
	void doAttack(world_object* target) override;
};

