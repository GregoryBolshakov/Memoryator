#include "WreathDraft.h"


WreathDraft::WreathDraft()
= default;

WreathDraft::~WreathDraft()
= default;

void WreathDraft::init(const DraftInfo& originalSetup)
{
	this->originalSetup = originalSetup;
	this->currentSetup = originalSetup;
}
