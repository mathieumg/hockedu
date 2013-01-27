

#include "Menu.h"
#include <iostream>

using namespace std;

Menu::Menu(string pTitle) {
	mTitle = pTitle;
}

Menu::~Menu() {
	clean();
}


bool Menu::addMenuOption(int pPosition, MenuOption* pMenuOption) {
	if(mMenuOptions.find(pPosition) != mMenuOptions.end())
		return false;
	mMenuOptions.insert(pair<int, MenuOption*>(pPosition, pMenuOption));
	return true;
}


void Menu::display() {
	// Display title
	system("cls");
	int wNbTirests = TITLE_WIDTH-(mTitle.length());
	int wDemi = wNbTirests/2;
	for(int i=0; i<wDemi; i++) {
		cout << "-";
	}
	cout << mTitle;
	for(int i=0; i<(wNbTirests-wDemi); i++) {
		cout << "-";
	}
	cout << endl;

	// Display elements
	map<int, MenuOption*>::const_iterator wIt;
	for(wIt = mMenuOptions.begin(); wIt!=mMenuOptions.end(); ++wIt) {
		cout << "  " << (*wIt).first << ". " << (*wIt).second->getTitle() << endl;
	}

	// Display the footer
	for(int i=0; i<TITLE_WIDTH; i++) {
		cout << "-";
	}
	cout << endl << endl;
}

string Menu::displayAndPick() {
	bool wValid = false;
	string wTemp = "";
	while(!wValid) {
		display();
		char wInput [256];
		cout << endl << "Enter your choice: ";
		fgets ( wInput, 256, stdin );
		int wIndex = atoi(wInput);
		map<int, MenuOption*>::iterator wChoice = mMenuOptions.find(wIndex);
		if(wChoice!=mMenuOptions.end()) {
			wValid = true;
			wTemp = (*wChoice).second->getTitle();
			(*wChoice).second->action();
		}
	}
	return wTemp;
}

void Menu::clean() {
	map<int, MenuOption*>::const_iterator wIt;
	for(wIt = mMenuOptions.begin(); wIt!=mMenuOptions.end(); ++wIt) {
		delete (*wIt).second;
	}
	mMenuOptions.empty();
}