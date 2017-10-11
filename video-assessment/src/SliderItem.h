/*
 *  SliderItem.h
 *  buttons
 *
 *  Created by timknapen on 30/01/10.
 *  Copyright 2010 Indianen. All rights reserved.
 *
 */

#ifndef _SLIDERITEM
#define _SLIDERITEM

#include "ListItem.h"
#include "ofxButtons.h"

class SliderItem : public ListItem{
private:
	
public:
	SliderItem( string theTitle, float theMin, float theMax, float& theValue);
	void draw();
	void drag(int x, int y);
	bool checkClick(int x, int y);
	void printValue();
	void saveToXML( ofxXmlSettings& XML);
	void setValue(float val);
	
	float * value;
	float top, bottom;
};

#endif