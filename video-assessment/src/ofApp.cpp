#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::setup() {
	
	//Set the backgrounds color
	colorOne = ofColor(ofColor::darkGray);
	colorTwo = ofColor(ofColor::black);
	gallery = Gallery(0.8, 1);	//Gallery proportions Parameters

	gallery.setup();

}

//--------------------------------------------------------------
void ofApp::update(){
	gallery.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
 
	ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_CIRCULAR);
	gallery.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	gallery.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	gallery.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	gallery.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	gallery.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	gallery.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	gallery.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	gallery.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}


//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
