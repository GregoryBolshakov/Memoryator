#pragma once

#include  "Monster.h"

class Nightmare1 : public Monster
{
public:
	Nightmare1(std::string objectName, Vector2f centerPosition);
	~Nightmare1();
	Vector2f calculateTextureOffset() override;
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
private:
	void doAttack(WorldObject* target) override;
};

