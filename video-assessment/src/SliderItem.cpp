/*
 *  SliderItem.cpp
 *  buttons
 *
 *  Created by timknapen on 30/01/10.
 *  Copyright 2010 Indianen. All rights reserved.
 *
 */

#include "SliderItem.h"


//--------------------------------------------------------------
SliderItem::SliderItem(string theTitle, float theMin, float theMax, float& theValue):ListItem(theTitle){
	// cout << "making new sliderItem: "<< theTitle << endl;
	top = theMax;
	bottom = theMin;
	w = 30 + 4+ title.length()*CHAR_W; //textw
	w = (w <200)?200:w;
	value = &theValue;
    *value = MIN(theMax, MAX(theMin, *value));
} 

//-------------------------------------------------
void SliderItem::saveToXML( ofxXmlSettings& XML){
	// nothing yet
	XML.addTag("SliderItem");
	int num = XML.getNumTags("SliderItem") - 1;
	XML.setAttribute("SliderItem", "name", title, num);
	XML.setAttribute("SliderItem", "value", ofToString(*value, 2), num);
	XML.setAttribute("SliderItem", "min", ofToString(bottom, 2), num);
	XML.setAttribute("SliderItem", "max", ofToString(top, 2), num);

}

//----------------------------------------------------------------
void SliderItem::setValue(float val){
	*value= val;
}



//--------------------------------------------------------------
bool SliderItem::checkClick(int x, int y){
	rely = y - ypos;
	selected = false;
	if(x > 0 && x < (w -5) && rely > 0 && rely < h){
		selected = true;
	}
	if (selected) {
		float t = (float)x/(float)(w -5);
		t = (t >1) ? 1: t;
		t = (t <0) ? 0: t;
		*value = bottom + (top-bottom)*t;
	}
	return selected;
}
//



//--------------------------------------------------------------
void SliderItem::drag(int x, int y){
	rely = y -ypos;
	float t = (float)x/(float)(w -5);
	t = (t >1) ? 1: t;
	t = (t <0) ? 0: t;
	*value = bottom + (top-bottom)*t;
}


//--------------------------------------------------------------
void SliderItem::printValue(){
	cout << "  " << title << " = " <<*value<< endl;
}


//--------------------------------------------------------------
void SliderItem::draw(){
	float limVal = MAX(bottom , MIN( top, *value)); // limited val
	
	ofPushMatrix();
	ofTranslate(0, ypos,0);
	ofFill();
	ofSetHexColor(BUTTONS_COLOR_LIGHT);
	ofDrawRectangle(0, 2,(w -5), h-2);
	if(over){
		ofSetHexColor(BUTTONS_COLOR_HIGHLIGHT);
		ofDrawRectangle(0, 2, (int)((w -5)*(limVal-bottom)/(top-bottom)), h-2);
	}else {
		ofSetHexColor(BUTTONS_COLOR_MEDIUM);
		ofDrawRectangle(0, 2,(int)((w -5)*(limVal-bottom)/(top-bottom)), h-2);
	}
	if(over){
		ofSetHexColor(0x333333);
	}else{
		ofSetHexColor(0x666666);
	}
	ofDrawBitmapString(title+" "+ofToString(*value, 2), 2, h-2);
	ofPopMatrix();
	
	//
}

