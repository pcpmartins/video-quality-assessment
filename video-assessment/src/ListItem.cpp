#include "ListItem.h"


//----------------------------------------------------------------
ListItem::ListItem(string theTitle){
	
	// cout<< "  ListItem: '" << theTitle << "'" << endl;
	counter = 100;
	title = theTitle;
	w = 4+ title.length()*CHAR_W; //textw
	ypos = 0;
	rely = 0;
	
	w = (w<100) ? 100 : w;
	h = 18;
	
	selected = false;
	over = false;
}


//----------------------------------------------------------------
ListItem::~ListItem(){
	// destructor
}


//----------------------------------------------------------------
void ListItem::saveToXML( ofxXmlSettings& XML){
	// nothing yet
	XML.addTag("ListItem");
	int num = XML.getNumTags("ListItem") - 1;
	XML.setAttribute("ListItem", "name", title, num);
}



//----------------------------------------------------------------
void ListItem::printValue(){
	cout << "  " << title << endl;
}



//----------------------------------------------------------------
void ListItem::draw(){
	ofPushMatrix();
	ofTranslate(0, ypos, 0);
	ofFill();
	// bg color
	// ofSetColor(240, 255, 240);
	// ofRect(0,0, w, h);
	if(over){
		ofSetHexColor(0x333333);
	}else{
		ofSetHexColor(0x666666);
	}
	ofDrawBitmapString(title, 2, h-2);
	ofPopMatrix();
}

//----------------------------------------------------------------
void ListItem::setValue(float val){
	//
}

//----------------------------------------------------------------
void ListItem::drag(int x, int y){
	// alleen virtual voor sliders
	//cout<< "dragging: "<<title<< endl;
}
//


//----------------------------------------------------------------
void ListItem::update(){
	counter ++;
}


//----------------------------------------------------------------
bool ListItem::checkClick(int x, int y){
	rely = y - ypos;
	selected =(x > 0 && x < w && rely > 0 && rely < h);
	return selected;
}


//----------------------------------------------------------------
bool ListItem::checkOver(int x, int y){
	rely = y - ypos;
	over =(x > 0 && x < w && rely > 0 && rely < h);
	return over;
}


//----------------------------------------------------------------
void ListItem::unSelect(){
	selected =false;
}


//----------------------------------------------------------------
void ListItem::setName(string s){
	title = s;
}



//----------------------------------------------------------------
bool ListItem::hasName(string s){
	return !(strcmp(s.c_str(), title.c_str()));
}