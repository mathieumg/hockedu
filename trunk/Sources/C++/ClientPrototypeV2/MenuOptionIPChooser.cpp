
#include "MenuOptionIPChooser.h"

MenuOptionIPChooser::MenuOptionIPChooser(string pTitle)
	:MenuOption(pTitle){

}

void MenuOptionIPChooser::action() {
	*mIP = getTitle();
}

void MenuOptionIPChooser::setIPVariable(string* pIP) {
	mIP = pIP;
}