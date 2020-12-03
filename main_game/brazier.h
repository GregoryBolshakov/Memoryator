#pragma once

#include "terrain_object.h"

class brazier : public terrain_object
{
public:
	brazier(std::string name, sf::Vector2f position, int kind);
	void init_pedestal() override;
	void initCraftRecipes();
	std::vector<unique_ptr<sprite_chain_element>> prepare_sprites(long long elapsed_time) override;

	// craft
	//void linkWithBuildSystem(BuildSystem* buildSystem) { this->buildSystem = buildSystem; }
	void putItemToCraft(entity_tag id);
	void clearCurrentCraft() { currentCraft.clear(); }
	[[nodiscard]] sf::Vector2f getPlatePosition() const { return { position_.x + size_.x * 0.06f, position_.y - size_.x * 0.025f }; }
	entity_tag checkCraftResult();

	[[nodiscard]] float getPlateRadius() const { return plateRadius; }

	//BuildSystem* buildSystem = nullptr;
	//------
private:
	void resultAnalysis() const;

	std::string craftRecipesPath = "Game/craftRecipes.txt";
	std::map<entity_tag, std::vector<std::vector<std::pair<entity_tag, int>>>> craftRecipes;
	std::map<entity_tag, int> currentCraft;
	entity_tag craftResult;
	float plateRadius;
};
