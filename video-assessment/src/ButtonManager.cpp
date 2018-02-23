#include "ButtonManager.h"

//--------------------------------------------------------------
ButtonManager::ButtonManager(){
	// constructor
	visible = true;
	selButton = NULL;
	lastbutton = NULL;
	//
	stdRowWidth  = 310;
	//rowwidth = 180;
	//rowx = 20;
	toolbarlineheight = 25; //
	toolbarw = 180; // width of an item in the toolbar 200
	toolbarh = 20;
	bIsDragging = false;
    bReadKeys = true;
	lastPanel = NULL;
}


//--------------------------------------------------------------
void ButtonManager::setup(){
    // register events
    addEvents();
}


//--------------------------------------------------------------
ButtonManager::~ButtonManager(){
	selButton = NULL;
	lastbutton = NULL;
	for(int i = 0; i < buttons.size(); i++){
		delete buttons[i];
	}
	buttonorder.clear();
	buttons.clear();
}



//--------------------------------------------------------------
void ButtonManager::printValues(){
	cout << "== BUTTON SETTINGS ======" <<endl<<endl;
	
	for(int i =0; i < buttons.size(); i++){
		buttons[i]->printValue();
	}
	
	cout << endl << "== BUTTON SETTINGS ======" <<endl<<endl;
	
}

#pragma mark - DRAW

//--------------------------------------------------------------
void ButtonManager::draw(){
	if(visible){
		ofPushStyle();
		ofDisableSmoothing();
		ofDisableDepthTest();
        ofSetLineWidth(1);
		drawToolBar();
		//ofEnableAlphaBlending();
		for(int i = 0; i <buttons.size(); i++){
			buttons[i]-> draw();
		}
		//ofDisableAlphaBlending();
		ofPopStyle();
	}
}


//--------------------------------------------------------------
void ButtonManager::drawToolBar(){
	
	if(visible){
		//int ypos = 0;
        int xpos = 0;
		int margin = 5;
		int maxchars = (int)(toolbarw/ CHAR_W)-1;
		ofFill();
		ofSetColor(255);
		ofDrawRectangle(0, 0, ofGetWidth(), toolbarh); // bg
		//
		ofSetHexColor(0xcccccc);// bg border
		ofDrawLine(0, toolbarh, ofGetWidth(), toolbarh);
		
		// titels
		for(int i = 0 ; i < buttonorder.size(); i++) {
			// ypos = toolbarlineheight * (i+1);
			xpos = 10 + i * toolbarw;
			ofNoFill();
			ofSetHexColor(0x999999);
            
			ofFill();
			if (buttonorder[i]->visible) {
				ofSetHexColor(BUTTONS_COLOR_HIGHLIGHT);
				ofDrawRectangle(xpos+2, 2, toolbarw-4, 16 );
				ofSetHexColor(0x000000);
			}else{
				ofSetHexColor(0x000000);
			}
            
				ofDrawBitmapString((string)(buttonorder[i]->title).substr(0,maxchars),  xpos+CHAR_W/2, 16);

		}
	}
}


//--------------------------------------------------------------
void ButtonManager::update(){
	// update the buttons: increment counters and switch of flash buttons
	for(int i = 0; i <buttons.size(); i++){
		buttons[i]->update();
	}
	for(int i = 0; i <mssg.size(); i++){
		mssg[i]->update();
	}
	
}


//--------------------------------------------------------------
void ButtonManager::toggleVisible(){
	visible = !visible;
}



//--------------------------------------------------------------
void ButtonManager::show(){
	visible = true;
}


//--------------------------------------------------------------
void ButtonManager::hide(){
	visible = false;
}


#pragma mark - SAVING / LOADING

//--------------------------------------------------------------
void ButtonManager::saveToXML(string URL){
	cout << "== Saving Button Setting to xml file!==" << endl;
	XML.clear();	// clear cause we are building a new xml file
	XML.addTag("document");
	XML.pushTag("document");
	{
		XML.addTag("buttons");
		XML.pushTag("buttons");
		for(int i=0; i < buttons.size(); i++) {
			buttons[i]->saveToXML(XML);
		}
		XML.popTag();
	}
	XML.popTag();
	XML.saveFile(URL);
}

//--------------------------------------------------------------
void ButtonManager::loadXML(string URL){
	cout << "== Loading Button Setting from xml file: "<< URL<<" =="<<endl;
	XML.clear();	// clear because we are building a new xml file
	if(!XML.loadFile(URL)){
		cout << "I could not load "+URL+" into Buttons"<<endl;
		return;
	}
	if(!XML.pushTag("document", 0)){
        cout << "no document tag!" << endl;
    }
    
	if(!XML.pushTag("buttons", 0)){
        cout << "no buttons tag!" << endl;
    }
    
	
	int num = 0;
	
	// then ButtonPanels
	num = XML.getNumTags("ButtonPanel"); // case sensitive!
	if(num > 0){
		for(int i = 0; i < num; i++){
			string name = XML.getAttribute("ButtonPanel", "name", "", i);
			XML.pushTag("ButtonPanel", i);
			setButtonPanel(name);
			XML.popTag();
		}
	}
	XML.popTag(); // out of "buttons"
	XML.popTag(); // out of "document"
}


//--------------------------------------------------------------
void ButtonManager::setButtonPanel(string name){
    // first let's find the window!
	for (int i = 0; i < buttons.size(); i++) {
		if(buttons[i]->hasName(name)){
			buttons[i]->setFromXML(XML);
			return;
		}
	}
	
}






#pragma mark - ADDING BUTTONS


//--------------------------------------------------------------
void ButtonManager::addButton(SimpleButton * b){
	int i = buttons.size();
	int x, y;
	//
	lastbutton = b;
	// cout << " - add button at " << ofToString(b->xpos) << ", "<< ofToString(b->ypos) << endl;
	buttons.push_back(b);
	buttonorder.push_back(b);
	// maak al de onzin hierboven ongedaan en gebruik toolbaar schikking :)
	x = 10 + toolbarw * (i);
	y = toolbarh;
	b->setPos(x, y);
}


//--------------------------------------------------------------
void ButtonManager::addButton(string title ){
	addButton(new SimpleButton(title, 0, 0));
}

/********************* PANELS ********************/

//--------------------------------------------------------------
ButtonPanel * ButtonManager::addButtonPanel(string title){
	ButtonPanel * b = new ButtonPanel(title, 0, 0 );
	lastPanel = b;
	addButton(b);
	return b;
}

//--------------------------------------------------------------
void ButtonManager::addListItem(string title){
	if(lastPanel == NULL){
		addButtonPanel("Settings");
	}
	lastPanel->addListItem(title);
}

//--------------------------------------------------------------
void ButtonManager::addToggleItem(string title, bool& value){
	if(lastPanel == NULL){
		addButtonPanel("Settings");
	}
	lastPanel->addToggleItem(title, value);
}

//--------------------------------------------------------------
void ButtonManager::addSliderItem(string title, float bottom, float top, float& value){
	if(lastPanel == NULL){
		addButtonPanel("Settings");
	}
	lastPanel->addSliderItem(title, bottom, top, value);
}

//--------------------------------------------------------------
void ButtonManager::addFlashItem(string name, bool& value){
	if(lastPanel == NULL){
		addButtonPanel("Settings");
	}
	lastPanel->addFlashItem(name, value);
}

//--------------------------------------------------------------
void ButtonManager::addSelectionItem(string name, int& state, const int stateValue){
	if(lastPanel == NULL){
		addButtonPanel("Settings");
	}
	lastPanel->addSelectionItem(name, state, stateValue);
}





//--------------------------------------------------------------
void ButtonManager::MoveToFront(int i){
	// move a button to the first layer
	if(i >= 0 && i < buttons.size()){
		SimpleButton * p = buttons[i];
		buttons.erase(buttons.begin()+i);
		buttons.push_back(p);
	}
	
}

//--------------------------------------------------------------
void ButtonManager::MoveToFront(SimpleButton * b){
	// move a button to the first layer
	if(b != NULL ){
		for(int i = 0; i < buttons.size(); i++){
			if(buttons[i] == b){
				buttons.erase(buttons.begin()+i);
				buttons.push_back(b);
			}
		}
	}
}

//--------------------------------------------------------------
void ButtonManager::MoveMessageToFront(SimpleButton * b){
	// move a message to the first layer
	if(b != NULL ){
		for(int i = 0; i < mssg.size(); i++){
			if(mssg[i] == b){
				mssg.erase(mssg.begin()+i);
				mssg.push_back(b);
			}
		}
	}
}


#pragma mark - EVENTS

//--------------------------------------------------------------
void ButtonManager::draw(ofEventArgs &e){
    draw();
}

//--------------------------------------------------------------
void ButtonManager::update(ofEventArgs &e){
    update();
}

//--------------------------------------------------------------
void ButtonManager::exit(ofEventArgs &e){
    // handle exiting
}
//--------------------------------------------------------------
void ButtonManager::addEvents(){
    ofAddListener( ofEvents().update, this, &ButtonManager::update);
	ofAddListener( ofEvents().draw, this, &ButtonManager::draw);
    ofAddListener( ofEvents().exit, this, &ButtonManager::exit);
	ofAddListener( ofEvents().mousePressed, this, &ButtonManager::mousePressed);
	ofAddListener( ofEvents().mouseMoved, this, &ButtonManager::mouseMoved);
	ofAddListener( ofEvents().mouseDragged, this, &ButtonManager::mouseDragged);
	ofAddListener( ofEvents().mouseReleased, this, &ButtonManager::mouseReleased);
	ofAddListener( ofEvents().keyPressed, this, &ButtonManager::keyPressed);
	ofAddListener( ofEvents().keyReleased, this, &ButtonManager::keyReleased);
}

//--------------------------------------------------------------
void ButtonManager::mouseMoved(ofMouseEventArgs &e){
	mouseMoved(e.x, e.y);
}

//--------------------------------------------------------------
void ButtonManager::mousePressed(ofMouseEventArgs &e){
	mousePressed(e.x, e.y, e.button);
}

//--------------------------------------------------------------
void ButtonManager::mouseDragged(ofMouseEventArgs &e){
	mouseDragged(e.x, e.y, e.button);
}

//--------------------------------------------------------------
void ButtonManager::mouseReleased(ofMouseEventArgs &e){
	mouseReleased(e.x, e.y, e.button);
}

//--------------------------------------------------------------
void ButtonManager::keyPressed(ofKeyEventArgs &e){
	if(bReadKeys){
		switch (e.key) {
			case 'f':
				ofToggleFullscreen();
				break;
			case 'i':
				toggleVisible();
				break;
				
			default:
				break;
		}
	}
}

//--------------------------------------------------------------
void ButtonManager::keyReleased(ofKeyEventArgs &e){
	//unused
}


//--------------------------------------------------------------
void ButtonManager::readsKeys(bool _listen_to_keys){
	bReadKeys = _listen_to_keys;
}


#pragma mark - MOUSE EVENTS

//--------------------------------------------------------------
bool ButtonManager::isDragging(){
	// return bIsDragging;
	return visible && selButton != NULL;
}

//--------------------------------------------------------------
bool ButtonManager::mouseDragged(int x, int y){ // dragging
	bIsDragging = false;
	if(visible){
		if(selButton != NULL){
			//cout << "dragging nr. " << buttons[selButton]->title << endl;
			selButton->drag(x,y);
			bIsDragging = true;
			return true;
		}else {
			return false;
		}
		
	}else {
		return false;
	}
}


//--------------------------------------------------------------
bool ButtonManager::mouseDragged(int x, int y, int button){ // dragging
	// already reserved for some more complex behaviour in the future?
	return mouseDragged(x, y);
}

//--------------------------------------------------------------
bool ButtonManager::mousePressed(int x, int y){// mouse click event
	bool found = false;
	if(visible){
		selButton = NULL;
		for(int i = buttons.size()-1; i >=0; i--){
			if(!found ){
				if(buttons[i]->checkClick(x,y)){
					found = true;
					selButton = buttons[i];
					MoveToFront(selButton);
				}
			}else {
				buttons[i]->selected = false;
			}
			
		}
		if(!found && x >= 0 && y >=0 && y < toolbarh){
			int sel = (int) ((x-10)/toolbarw);
			if(sel>= 0 && sel < buttonorder.size()){
				found = true;
				buttonorder[sel]->showHide();
				// en die wordt op tekenvolgorde gesorteerd!
				for (int i =0; i < buttons.size(); i++) {
					if(buttonorder[sel] == buttons[i]){
						MoveToFront(i);
						break;
					}
				}
				
			}
		}
		
		
		
		
		
	}
	
	return found;
}

//--------------------------------------------------------------
bool ButtonManager::mousePressed(int x, int y, int button){
    return mousePressed(x, y);
}

//--------------------------------------------------------------
void ButtonManager::mouseMoved(int x, int y){// rollover event
	if(visible){
		bool found = false;
		for(int i = mssg.size()-1; i >=0; i--){
			if(!found) {
				found = mssg[i]->checkOver(x,y);
			}	else{
				mssg[i]->over = false;
			}
		}
		
		for(int i = buttons.size()-1; i >=0; i--){
			if(!found) {
				found = buttons[i]->checkOver(x,y);
			}	else{
				buttons[i]->over = false;
			}
		}
	}
}


//--------------------------------------------------------------
void ButtonManager::mouseReleased(){ // release the mousebutton
	for(int i = 0; i < buttons.size(); i++){
		buttons.at(i)->unSelect();
	}
	for(int i = mssg.size()-1; i >= 0; i--){
		mssg.at(i)->unSelect();
	}
}

//--------------------------------------------------------------
void ButtonManager::mouseReleased(int x, int y, int button){ // release the mousebutton
	mouseReleased();
}
