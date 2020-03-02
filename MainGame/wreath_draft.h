#pragma once

#include <SFML/Graphics.hpp>

#include "world_object.h"

using namespace sf;

struct draft_info
{
	std::vector<std::pair<entity_tag, std::pair<int, int>>> plants;
	std::vector<int> rings;
	entity_tag id;
};

class wreath_draft
{
	draft_info originalSetup;
public:
	wreath_draft();
	~wreath_draft();
	void init(const draft_info& originalSetup);
	[[nodiscard]] draft_info getOriginalSetup() const { return originalSetup; }

	draft_info currentSetup;
};
