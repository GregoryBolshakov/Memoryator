//#pragma once
//
//#include "terrain_object.h"
//
//class totem : public terrain_object
//{
//public:
//	totem(std::string objectName, sf::Vector2f centerPosition, int typeOfObject);
//	sf::Vector2f calculate_texture_offset() override;
//	void init_pedestal() override;
//	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
//	int strength;
//	void setType(int typeOfObject) override;
//	sf::Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, sf::Vector2f cameraPosition) override;
//	int get_build_type(sf::Vector2f ounPos, sf::Vector2f otherPos) override;
//};
