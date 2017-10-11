#ifndef _BUTTON_MANAGER
#define _BUTTON_MANAGER

#include "ofMain.h"
#include "ofxButtons.h"
#include "ofxXmlSettings.h"

class ButtonPanel;

class ButtonManager{	
public:
	
	ButtonManager();
	~ButtonManager();
	
	void printValues();
	void setup();
	void loadXML(string URL);
	void saveToXML(string URL);
	void setButtonPanel(string name);
	
	// PANELS
	ButtonPanel * lastPanel;
	ButtonPanel * addButtonPanel(string title);
	void addListItem(string title);
	void addToggleItem(string title, bool& value); //onoff
	void addSliderItem(string title, float bottom, float top, float& value); // slider
	void addFlashItem(string name, bool& value); // flash button
	void addSelectionItem(string name, int& state, const int stateValue); // multi selection item


	//
	void addButton(SimpleButton * aButton);
	void addButton(string title);
	
	void MoveToFront(int i); // put this button on top of the drawing stack
	void MoveToFront(SimpleButton * b);
	void MoveMessageToFront(SimpleButton * b);
	//
	// EVENTS
	bool mousePressed(int x, int y, int button); // mouse click event
	bool mousePressed(int x, int y); // mouse click event
	bool mouseDragged(int x, int y, int button); // dragging
	bool mouseDragged(int x, int y); // dragging
	void mouseMoved(int x, int y); // roll-over checking!
	void mouseReleased(int x, int y, int button); // release the mousebutton
	void mouseReleased(); // release the mousebutton
	
	// OF EVENTS
    void draw(ofEventArgs &e);
    void exit(ofEventArgs &e);
	void update(ofEventArgs &e);
	void mouseMoved(ofMouseEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);	
	void mouseDragged(ofMouseEventArgs &e);	
	void mouseReleased(ofMouseEventArgs &e);
	void keyPressed(ofKeyEventArgs &e);
	void keyReleased(ofKeyEventArgs &e);
	//
	void addEvents();
	
	bool isDragging();		// am i dragging?
	void draw(); // draw the buttons
	void drawToolBar(); // draw a toolbar on top!
	void update(); // update the buttons (als ze een counter hebben ofzo?)
	//
	void toggleVisible();
	void show();
	void hide();
	void readsKeys(bool _listen_to_keys);
	//
	bool visible;
	SimpleButton * selButton; 
	vector <SimpleButton*> mssg;
	vector <SimpleButton*> buttons; // een vector van pointers naar simplebuttons
	vector <SimpleButton*> buttonorder; // een vector van pointers naar simplebuttons, w niet herschikt om tekenorde te laten kloppen
	SimpleButton * lastbutton;
	//
	float stdRowWidth; // breedte van een kollom wanneer die niet moet aangepast worden
	float rowwidth;
	float rowx;
	float rowheight;
	
	//
	int toolbarlineheight;
	int toolbarw;
	int toolbarh;
	
	//
	bool bReadKeys; // to be able to switch key input off
	bool bIsDragging;
	ofxXmlSettings XML;
};


#endif