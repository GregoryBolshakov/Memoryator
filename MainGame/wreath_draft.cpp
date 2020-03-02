#include "wreath_draft.h"


wreath_draft::wreath_draft()
= default;

wreath_draft::~wreath_draft()
= default;

void wreath_draft::init(const DraftInfo& originalSetup)
{
	this->originalSetup = originalSetup;
	this->currentSetup = originalSetup;
}
