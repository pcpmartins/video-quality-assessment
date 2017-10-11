/*
 *  ToggleItem.h
 *  buttons
 *
 *  Created by timknapen on 30/01/10.
 *  Copyright 2010 Indianen. All rights reserved.
 *
 */

#ifndef _ONOFFITEM
#define _ONOFFITEM

#include "ListItem.h"
#include "ofxButtons.h"

class ToggleItem : public ListItem{
private:
	
public:
	ToggleItem( string theTitle, bool& value);
	void draw();
	bool checkClick(int x, int y);
	void printValue();
	void saveToXML( ofxXmlSettings& XML);
	void setValue(float val);
	//
	bool * on;
};

#endif