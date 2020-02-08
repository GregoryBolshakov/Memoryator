#pragma once
#ifndef NIGHTMARE3_H
#define  NIGHTMARE3_H

#include  "Monster.h"

class Nightmare3 : public Monster
{
public:
	Nightmare3(std::string objectName, Vector2f centerPosition);
	~Nightmare3();
	Vector2i calculateTextureOffset() override;
	std::vector<SpriteChainElement> prepareSprites(long long elapsedTime) override;
private:
	void doAttack(WorldObject* target) override;
	void endingPreviousAction() override;
	void onSpriteChange() override;
};

#endif

