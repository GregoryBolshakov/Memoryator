//#pragma once
//
//#include "terrain_object.h"
//#include "build_system.h"
//
//class wreath_table : public terrain_object
//{
//public:
//	wreath_table(std::string objectName, Vector2f centerPosition, int typeOfObject);
//	Vector2f calculate_texture_offset() override;
//	void init_pedestal() override;
//	void initCraftRecipes();
//	std::vector<sprite_chain_element*> prepare_sprites(long long elapsedTime) override;
//	void setType(int typeOfObject) override;
//	Vector2f get_build_position(std::vector<world_object*> visibleItems, float scaleFactor, Vector2f cameraPosition) override;
//	int get_build_type(Vector2f ounPos, Vector2f otherPos) override;
//
//	// craft
//	//void linkWithBuildSystem(BuildSystem* buildSystem) { this->buildSystem = buildSystem; }
//	void putItemToCraft(entity_tag id);
//	void clearCurrentCraft() { currentCraft.clear(); }
//	[[nodiscard]] Vector2f getPlatePosition() const { return { position_.x + conditional_size_units_.x * 0.06f, position_.y - conditional_size_units_.x * 0.025f }; }
//	entity_tag checkCraftResult();
//
//	[[nodiscard]] int getPlateRadius() const { return plateRadius; }
//
//	//BuildSystem* buildSystem = nullptr;
//private:
//	void resultAnalysis() const;
//
//	std::string craftRecipesPath = "Game/craftRecipes.txt";
//	std::map<entity_tag, std::vector<std::vector<std::pair<entity_tag, int>>>> craftRecipes;
//	std::map<entity_tag, int> currentCraft;
//	entity_tag craftResult = entity_tag::emptyCell;
//	int plateRadius;
//};
