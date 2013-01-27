
#ifndef MENU_H
#define MENU_H
#define TITLE_WIDTH 75

#include "MenuOption.h"
#include <map>

using namespace std;

class Menu {
public: 
	Menu(string pTitle);
	~Menu();
	bool addMenuOption(int pPosition, MenuOption* pMenuOption);
	void display();
	string displayAndPick();
	void clean();
private:
	map<int, MenuOption*> mMenuOptions; // Menu options
	string mTitle; // Title of the menu
};

#endif


