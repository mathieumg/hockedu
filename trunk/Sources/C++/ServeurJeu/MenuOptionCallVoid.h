

#ifndef MENU_OPTIONS_CALL_VOID_H
#define MENU_OPTIONS_CALL_VOID_H

#include "MenuOption.h"

typedef void (*MenuFunc) ();

class MenuOptionCallVoid : public MenuOption {
public:
	MenuOptionCallVoid(string pTitle, MenuFunc pFunction);
	virtual void action();
	void setActionFunction(MenuFunc pFunction);
private:
	MenuFunc mFunction;
};

#endif