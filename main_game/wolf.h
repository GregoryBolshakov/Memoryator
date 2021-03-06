//#pragma once
//
//#include "neutral_mob.h"
//#include "helper.h"
//
//using namespace sf;
//
//class wolf : public neutral_mob
//{
//	float timeForNewSprite;
//
//	int animationLength;
//	bool wasHit = false;
//	float timeAfterHit = 0, timeForNewHit = 100000;
//	int strikingSprite = 4;
//public:
//	wolf(std::string objectName, sf::Vector2f centerPosition);
//	~wolf();
//	sf::Vector2f calculate_texture_offset() override;
//	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
//	void behavior_with_static(world_object* target, long long elapsedTime) override;
//	void behavior(long long elapsedTime) override;
//	sf::Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition) override;
//	int get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos) override;
//	void set_target(dynamic_object& object) override;
//	void jerk(float power, float deceleration, sf::Vector2f destinationPoint) override;
//};
