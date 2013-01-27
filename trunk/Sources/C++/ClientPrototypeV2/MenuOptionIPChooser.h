

#ifndef MENU_OPTIONS_IP_CHOOSER_H
#define MENU_OPTIONS_IP_CHOOSER_H

#include "MenuOption.h"

class MenuOptionIPChooser : public MenuOption {
public:
	MenuOptionIPChooser(string pTitle);
	virtual void action();
	void setIPVariable(string* pIP);
private:
	string* mIP;
};

#endif