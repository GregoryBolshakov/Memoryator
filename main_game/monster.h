//#pragma once
//
//#include "dynamic_object.h"
//#include "helper.h"
//
//using namespace sf;
//
//class monster : public dynamic_object
//{
//public:
//	monster(std::string objectName, sf::Vector2f centerPosition);
//	~monster();
//	void behavior_with_dynamic(dynamic_object* target, long long elapsedTime) override;
//	void behavior_with_static(world_object* target, long long elapsedTime) override;
//	void behavior(long long elapsedTime) override;
//	virtual void endingPreviousAction();
//	void stopping(bool doStand, bool forgetBoundTarget);
//	sf::Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition) override;
//	int get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos) override;
//	[[nodiscard]] world_object* getVictim() const { return victim; }
//	void set_target(dynamic_object& object) override;
//	void jerk(float power, float deceleration, sf::Vector2f destinationPoint) override;
//	void fight_interact(long long elapsedTime, dynamic_object* target = nullptr) override;
//	virtual void doAttack(world_object* target) = 0;
//protected:
//	world_object* victim = nullptr;
//	std::map<action, bool> isAttack = { {common_hit, true}, {direct_hit, true} };
//	float hitDistance = 30;
//	int animationLength{};
//	bool wasHit = false;
//
//	int strikingSprite = 0;
//};
