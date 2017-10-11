/*
 *  ToggleItem.cpp
 *  buttons
 *
 *  Created by timknapen on 30/01/10.
 *  Copyright 2010 Indianen. All rights reserved.
 *
 */

#include "ToggleItem.h"

//--------------------------------------------------------------
ToggleItem::ToggleItem(string theTitle, bool& theValue):ListItem(theTitle){
	// do nothing?
	w = 30 + 4+ title.length()*CHAR_W; //textw
	w = (w <200)?200:w;
	on = &theValue;
} 

//-------------------------------------------------
void ToggleItem::saveToXML( ofxXmlSettings& XML){
	// nothing yet
	XML.addTag("ToggleItem");
	int num = XML.getNumTags("ToggleItem") - 1;
	XML.setAttribute("ToggleItem", "name", title, num);
	XML.setAttribute("ToggleItem", "value", (*on)?"1":"0", num);
	
}

//----------------------------------------------------------------
void ToggleItem::setValue(float val){
	*on= (bool)val;
}

//--------------------------------------------------------------
void ToggleItem::printValue(){
	cout << "  " << title << " = " <<*on<< endl;
}

//--------------------------------------------------------------
bool ToggleItem::checkClick(int x, int y){
	rely = y - ypos;
	selected = false;
	if(x > 0 && x < w && rely > 0 && rely < h){
		selected = true;
		*on = !*on;
	}
	return selected;
}

//--------------------------------------------------------------
void ToggleItem::draw(){
	ofPushMatrix();
	ofTranslate(0, ypos,0);
    ofFill();

	if(*on) {
		if(over){
			ofSetHexColor(BUTTONS_COLOR_HIGHLIGHT);
		}else{
            ofSetHexColor(BUTTONS_COLOR_MEDIUM);
		}
        ofDrawRectangle(0, 2,(w - 5), h-2);
	}else {
        ofNoFill();
		ofSetHexColor(BUTTONS_COLOR_LIGHT);
        ofDrawRectangle(0, 2,(w - 5), h-2);
	}

    if(over){
        ofSetHexColor(0x333333);
    }else{
        ofSetHexColor(0x666666);
    }
    ofDrawBitmapString(title , (w-5)/2 - (title.length()*CHAR_W)/2, h-4); // title + " " + (*on ? "ON" : "OFF" )

	ofPopMatrix();
	
	//
}

