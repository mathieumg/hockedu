
#include "MenuOption.h"

MenuOption::MenuOption(string pTitle) {
	mTitle = pTitle;
}

void MenuOption::action() {
	// Do nothing
}

string MenuOption::getTitle() const {
	return mTitle;
}


