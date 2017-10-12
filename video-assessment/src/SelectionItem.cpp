/*
 *  SelectionItem.cpp
 *  buttons
 *
 *  Created by timknapen on 30/01/10.
 *  Copyright 2010 Indianen. All rights reserved.
 *
 */

#include "SelectionItem.h"

//--------------------------------------------------------------
SelectionItem::SelectionItem(string theTitle, int& _state, const int _stateValue):ListItem(theTitle){
	// do nothing?
	w = 15 + 4+ title.length()*CHAR_W; //textw
	state = &_state;
	stateValue = _stateValue;
} 

//-------------------------------------------------
void SelectionItem::saveToXML( ofxXmlSettings& XML){
	// nothing yet
	XML.addTag("SelectionItem");
	int num = XML.getNumTags("SelectionItem") - 1;
	XML.setAttribute("SelectionItem", "name", title, num);
	XML.setAttribute("SelectionItem", "value", *state, num);
	
}

//----------------------------------------------------------
void SelectionItem::setValue(float val){
	*state = (int)val;
}

//--------------------------------------------------------------
void SelectionItem::printValue(){
	cout << "  " << title << " = " <<state<< endl;
}

//--------------------------------------------------------------
bool SelectionItem::checkClick(int x, int y){
	rely = y - ypos;
	selected = false;
	if(x > 0 && x < w && rely > 0 && rely < h){
		selected = true;
		*state = stateValue;
	}
	return selected;
}

//--------------------------------------------------------------
void SelectionItem::draw(){
	ofPushMatrix();
	ofTranslate(32, 0, 0);
	ListItem::draw();
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(0, ypos,0);
	ofFill();
	if (*state == stateValue) {
		if(over){
			ofSetHexColor(BUTTONS_COLOR_HIGHLIGHT);
		}else{
			ofSetHexColor(0x999999);
		}
		ofDrawEllipse(12, 9, 11, 11);

	}else {
		ofSetHexColor(0xcccccc);
		ofNoFill();
		ofDrawEllipse(12, 9, 9, 9);

	}

	ofPopMatrix();
	
	//
}

