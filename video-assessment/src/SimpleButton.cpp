#include "SimpleButton.h"

//-------------------------------------------------
SimpleButton::SimpleButton(string name, int x, int y){
	
	// cout<< "  button: '" << name << "'" << endl;
	counter = 0;
	title = name;
	xpos = x;
	ypos = y;
	w = 4+ title.length() *CHAR_W; //textw
	w = (w<90) ? 90 : w;
	h = 18;
	buttonheight= 2*h;
	relx = 0;
	rely = 0;
	selected = false;
	over = false;
	visible =false;
}


//-------------------------------------------------
SimpleButton::~SimpleButton(){
	// destructor
}

//----------------------------------------------------------------
void SimpleButton::printValue(){
	cout << title << endl;
}

//----------------------------------------------------------------
void SimpleButton::saveToXML( ofxXmlSettings& XML){
	// nothing yet
	XML.addTag("Button");
	int num = XML.getNumTags("Button") - 1;

	//XML.pushTag("Button", num);
	XML.setAttribute("Button", "name", title, num);
	XML.setAttribute("Button", "value", "value", num);
	//XML.popTag();
}

//----------------------------------------------------------------
void SimpleButton::draw(){
	if(visible){
		ofPushMatrix();
		ofTranslate(xpos, ypos, 0);
		//schaduw
		ofFill();
		//ofSetColor(100, 100, 100,10); // << NO alpha!
		ofSetColor(0, 0, 0,20); // << NO alpha!
		
		ofDrawRectangle(4, 4, w, h*2);
		//
		ofSetHexColor(0x000000);
		if(over){
			ofSetHexColor(0xffffff);
		}else{
			ofSetHexColor(0xfafafa);
		}
		ofDrawRectangle(0, 0, w, h*2); // bg
		//
		if(over){
			ofSetHexColor(0x666666);
		}else{
			ofSetHexColor(0x999999);
		}
		ofNoFill(); // bg border
		ofDrawRectangle(0, 0, w, h*2);
		ofDrawBitmapString(title, 2, h/2 +6);
		ofPopMatrix();	
	}
	
}

//----------------------------------------------------------------
void SimpleButton::showHide(){
	if(visible){
		hide();
	}else {
		show();
	}
}

//----------------------------------------------------------------
void SimpleButton::show(){
	visible = true;
	//stay in frame
	xpos = (xpos < 0) ? 0 : xpos;
	xpos = (xpos > ofGetWidth()) ? ofGetWidth() - 200 : xpos;
	ypos = (ypos < 0) ? 0 : ypos;
	ypos = (ypos > ofGetHeight()) ? ofGetHeight() - 200 : ypos;

}

//----------------------------------------------------------------
void SimpleButton::hide(){
	visible = false;
}


//----------------------------------------------------------------
void SimpleButton::update(){
	counter ++;
}


//----------------------------------------------------------------
bool SimpleButton::checkClick(int x, int y){
	relx = x-xpos;
	rely = y-ypos;
	selected =(relx > 0 && relx < w && rely > 0 && rely < 2*h);
	return selected;
}


//----------------------------------------------------------------
bool SimpleButton::checkOver(int x, int y){
	relx = x-xpos;
	rely = y-ypos;
	over =(relx > 0 && relx < w && rely > 0 && rely < 2*h);
	return over;
}


//----------------------------------------------------------------
void SimpleButton::unSelect(){
	selected =false;
}


//----------------------------------------------------------------
void SimpleButton::drag(int x, int y){
	xpos = x -relx;
	ypos = y -rely;
}


//----------------------------------------------------------------
void SimpleButton::setPos(int x, int y){
	xpos = x;
	ypos = y;
}

//----------------------------------------------------------------
void SimpleButton::setTitle(string s){
	title = s;
}


//----------------------------------------------------------------
bool SimpleButton::hasName(string s){
	return !(strcmp(s.c_str(), title.c_str()));
}


//----------------------------------------------------------------
void SimpleButton::setFromXML(ofxXmlSettings &XML){
	//just for ButtonPanel...
}
