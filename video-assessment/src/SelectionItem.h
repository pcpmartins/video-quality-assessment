/*
 *  SelectionItem.h
 *  buttons
 *
 *  Created by timknapen on 30/01/10.
 *  Copyright 2010 Indianen. All rights reserved.
 *
 */

#ifndef _SELECTION_ITEM
#define _SELECTION_ITEM

#include "ListItem.h"
#include "ofxButtons.h"

class SelectionItem : public ListItem{
private:
	
public:
	SelectionItem( string theTitle, int& _state, const int _stateValue);
	void draw();
	bool checkClick(int x, int y);
	void printValue();
	void saveToXML( ofxXmlSettings& XML);
	void setValue(float val);
	//
	int * state; // pointer to the multiselection state
	int stateValue; // the value of my state (when I'm clicked, state will change into this value)
};

#endif