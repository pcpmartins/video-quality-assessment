/*
 *  FlashItem.cpp
 *  buttons
 *
 *  Created by timknapen on 30/01/10.
 *  Copyright 2010 Indianen. All rights reserved.
 *
 */

#include "FlashItem.h"

//--------------------------------------------------------------
FlashItem::FlashItem(string theTitle, bool& theValue):ListItem(theTitle){
	// do nothing?
	w = 34 + 4+ title.length()*CHAR_W; //textw
	on = &theValue;
	steps = 10;
	counter = 20;
} 



//-------------------------------------------------
void FlashItem::saveToXML( ofxXmlSettings& XML){
	// Unneeded!
	XML.addTag("FlashItem");
	int num = XML.getNumTags("FlashItem") - 1;
	XML.setAttribute("FlashItem", "name", title, num);
	XML.setAttribute("FlashItem", "value", (*on)?"true":"false", num);
	
}

//--------------------------------------------------------------
bool FlashItem::checkClick(int x, int y){
	rely = y - ypos;
	selected = false;
	if(x > 0 && x < w && rely > 0 && rely < h){
		*on = true;
		counter = 0;
		selected = true;
	}
	return selected;
}

//--------------------------------------------------------------
void FlashItem::draw(){
	ofPushMatrix();
	ofTranslate(32, 0, 0);
	ListItem::draw();
	ofPopMatrix();
	//
	ofPushMatrix();
	ofTranslate(0, ypos,0);
	{
		ofSetHexColor(0xeeeeee);
		if(over){
			ofDrawRectangle(5, 5, 20, 10);
		}
		int steps = 20;
		if(counter < steps ){
			ofSetColor(255, 255*counter / steps, 255 * counter / steps);
			
			ofFill();
			ofDrawRectangle(5, 5, 20, 10);
			
			
		}else {
			ofSetHexColor(0xcccccc);
			ofNoFill();
			ofDrawRectangle(5, 5, 20, 10);
			
			
		}
		
	}
	
	ofPopMatrix();
	
	//
}

//--------------------------------------------------------------

void FlashItem::printValue(){
	cout << "  " << title << " = " <<*on<< endl;
}

//--------------------------------------------------------------
void FlashItem::update(){
	if(*on){
		counter = 0;
		*on = false;
	}
	ListItem::update();
}

