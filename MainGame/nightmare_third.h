#pragma once

#include  "monster.h"

class nightmare_third : public monster
{
public:
	nightmare_third(std::string objectName, Vector2f centerPosition);
	~nightmare_third();
	Vector2f calculateTextureOffset() override;
	std::vector<SpriteChainElement*> prepareSprites(long long elapsedTime) override;
private:
	void doAttack(WorldObject* target) override;
	void endingPreviousAction() override;
	void onSpriteChange() override;
};
