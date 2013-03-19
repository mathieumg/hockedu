
#include "MenuOptionCallVoid.h"

MenuOptionCallVoid::MenuOptionCallVoid(string pTitle, MenuFunc pFunction)
	:MenuOption(pTitle)
{
        setActionFunction(pFunction);
}

void MenuOptionCallVoid::action()
{
    if(mFunction)
    {
        mFunction();
    }
}

void MenuOptionCallVoid::setActionFunction(MenuFunc pIP)
{
	mFunction = pIP;
}