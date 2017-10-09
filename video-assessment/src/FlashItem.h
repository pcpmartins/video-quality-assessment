/*
 *  FlashItem.h
 *  buttons
 *
 *  Created by timknapen on 30/01/10.
 *  Copyright 2010 Indianen. All rights reserved.
 *
 */

#ifndef _FLASHITEM
#define _FLASHITEM

#include "ListItem.h"
#include "ofxButtons.h"

class FlashItem : public ListItem{
private:
	
public:
	FlashItem( string theTitle, bool& value);
	void draw();
	void update();
	void printValue();
	bool checkClick(int x, int y);
	void saveToXML( ofxXmlSettings& XML);

	//
	bool * on;
	float steps;

};

#endif