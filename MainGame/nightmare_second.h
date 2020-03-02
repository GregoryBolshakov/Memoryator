#pragma once

#include  "monster.h"

class nightmare_second : public monster
{
public:
	nightmare_second(std::string objectName, Vector2f centerPosition);
	~nightmare_second();
	Vector2f calculateTextureOffset() override;
	std::vector<sprite_chain_element*> prepareSprites(long long elapsedTime) override;
private:
	void doAttack(WorldObject* target) override;
};
