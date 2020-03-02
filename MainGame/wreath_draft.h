#pragma once

#include <SFML/Graphics.hpp>

#include "WorldObject.h"

using namespace sf;

struct DraftInfo
{
	std::vector<std::pair<Tag, std::pair<int, int>>> plants;
	std::vector<int> rings;
	Tag id;
};

class wreath_draft
{
	DraftInfo originalSetup;
public:
	wreath_draft();
	~wreath_draft();
	void init(const DraftInfo& originalSetup);
	[[nodiscard]] DraftInfo getOriginalSetup() const { return originalSetup; }

	DraftInfo currentSetup;
};
