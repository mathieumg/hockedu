
#ifndef MENU_OPTION_H
#define MENU_OPTION_H

#include <string>

using namespace std;

class MenuOption {
public:
	MenuOption(string pTitle);
	string getTitle() const;
	virtual void action();
private:
	string mTitle;
};


#endif