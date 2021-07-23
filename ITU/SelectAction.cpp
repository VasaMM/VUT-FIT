#include "SelectAction.h"

void ITU::SelectAction::actionSelected( ITU::Shutdown::actionType action ) {
	parent->shutdown->action = action;

	this->Close();
}