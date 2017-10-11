/*
 *  ButtonPanel.h
 *  buttons
 *
 
 a window containing a group of sliders, flashbuttons, multiselection buttons and on-off buttons.
 this window looks like a SimpleButton
 has a title (on top)
 and is dragable
 Every item in the window is a subclass of ListItem
 
 *  Created by timknapen on 30/01/10.
 *  Copyright 2010 Indianen. All rights reserved.
 *
 */


#ifndef _BUTTON_WINDOW
#define _BUTTON_WINDOW

#include "SimpleButton.h"
#include "ofxButtons.h"

class ListItem;
class ToggleItem;
class SliderItem;
class FlashItem;
class SelectionItem;

class ButtonPanel : public SimpleButton {
public:
	ButtonPanel(string theTitle, int x, int y);
	~ButtonPanel();
	void update();
	void draw();
	void drag(int x, int y );
	
	void addListItem(string title);
	void addToggleItem(string title, bool& value); //onoff
	SliderItem * addSliderItem(string title, float bottom, float top, float& value); // slider
	void addFlashItem(string name, bool& value); // flash button
	void addSelectionItem(string name, int& state, const int stateValue); // multi selection item

	bool checkClick(int x, int y);
	bool checkTitleClick(int x, int y);
	bool checkOver(int x, int y);
	void saveToXML( ofxXmlSettings& XML);
	void setFromXML( ofxXmlSettings& XML);
	void setByName(string name, float value);
	void clearItems();

	//
	vector <ListItem *> items;
	int sel;
	
private:
	void addListItem(ListItem * item);
	void drawItems();
	int itemheight;
	void printValue();
	
};

#endif
