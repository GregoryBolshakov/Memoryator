//#pragma once
//
//#include "dynamic_object.h"
//#include "helper.h"
//
//using namespace sf;
//
//class noose : public dynamic_object
//{
//public:
//	noose(const std::string& objectName, sf::Vector2f centerPosition, world_object* owner);
//	~noose();
//	sf::Vector2f calculate_texture_offset() override;
//	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
//    void behavior_with_dynamic(dynamic_object* target, long long elapsedTime) override;
//	void behavior_with_static(world_object* target, long long elapsedTime) override;
//	void behavior(long long elapsedTime) override;
//	sf::Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition) override;
//	int get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos) override;	
//	void setOwner(world_object* owner) { this->owner = owner; };
//	void set_target(dynamic_object& object) override;
//	void jerkInteract(long long elapsedTime);
//	void jerk(float power, float deceleration, sf::Vector2f destinationPoint = sf::Vector2f(-1, -1)) override;
//	void fight_interact(long long elapsedTime, dynamic_object* target = nullptr) override;
//
//private:
//	void stopping(bool doStand = false, bool forgetBoundTarget = false);
//    void endingPreviousAction();
//	void rotateAndExtend(sprite_chain_element* rope, sprite_chain_element* loop) const;
//
//    long long timeForNewSprite;
//    float ropeElongation = 0;
//    float maximumLength = 1500;
//	int animationLength;
//	world_object* owner;
//	sf::Vector2f ownerPos = { 0, 0 };
//	sf::Vector2f ownerGlobalBounds = { 0, 0 };
//};
