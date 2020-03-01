#pragma once

#include  "Monster.h"

class Nightmare2 : public Monster
{
public:
	Nightmare2(std::string objectName, Vector2f centerPosition);
	~Nightmare2();
	Vector2f calculateTextureOffset() override;
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
private:
	void doAttack(WorldObject* target) override;
};
