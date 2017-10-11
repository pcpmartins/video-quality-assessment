#ifndef _LIST_ITEM
#define _LIST_ITEM

/*************************
 
 super class voor alle buttons die in een venstertje zitten
 
 *************************/

#include "ofMain.h"
#include "ofxXmlSettings.h"

#define CHAR_W 8


class ListItem {
public:
	
	ListItem(string theTitle);
	~ListItem();
	// virtuals
	virtual void update();
	virtual void draw();
	virtual void printValue();
	virtual void drag(int x, int y); 
	virtual bool checkClick(int x, int y);
	virtual bool checkOver(int x, int y);
	virtual void unSelect();
	virtual void saveToXML( ofxXmlSettings& XML);
	virtual void setValue( float val);
	//
	void setName(string s);
	bool hasName(string s);
	string title;
	int ypos, rely;
	int w;
	int h;
	int buttonheight;
	bool selected; // has been clicked
	bool over; // has been rolled over
	//
	int counter; // for animation etc..
};

#endif


