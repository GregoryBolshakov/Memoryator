#pragma once
#ifndef NIGHTMARE2_H
#define  NIGHTMARE2_H

#include  "Monster.h"

class Nightmare2 : public Monster
{
public:
	Nightmare2(std::string objectName, Vector2f centerPosition);
	~Nightmare2();
	Vector2i calculateTextureOffset() override;
	std::vector<SpriteChainElement> prepareSprites(long long elapsedTime) override;
private:
	void doAttack(WorldObject* target) override;
};

#endif

