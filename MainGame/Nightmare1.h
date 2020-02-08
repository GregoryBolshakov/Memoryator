#pragma once
#ifndef NIGHTMARE1_H
#define  NIGHTMARE1_H

#include  "Monster.h"

class Nightmare1 : public Monster
{
public:
	Nightmare1(std::string objectName, Vector2f centerPosition);
	~Nightmare1();
	Vector2i calculateTextureOffset() override;
	std::vector<SpriteChainElement> prepareSprites(long long elapsedTime) override;
private:
	void doAttack(WorldObject* target) override;
};

#endif

