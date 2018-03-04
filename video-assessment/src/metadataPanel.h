#pragma once
#include "ofxFontStash.h"
#include "VideoFile.h"

class metadataPanel
{
public:
	metadataPanel();
	~metadataPanel();

	void getData(const VideoFile *img);		//Get data from video and save it to metadaDisplay string
	void setup(int x, int y);
	void draw(int x, int y);
	float getWidth();
	float getHeight();
	float getCordX();
	float getCordY();

private:

	////////////////////////////////////Methods///////////////////////////////////////////

	//string createCommonString(const File* f);//Get all common features for image and video 
	string metadataString;		//Text to display
	ofxFontStash font;			//Displaying object
	ofRectangle bboxMultiline;	//Frame
	ofRectangle title;	//Frame
	ofRectangle middle;	//Frame
	ofRectangle middleRight;	//Frame
	ofRectangle middle2;	//Frame
	ofRectangle middleRight2;	//Frame
	ofRectangle middle3;	//Frame
	ofRectangle middle3Right;	//Frame
	ofRectangle advanced;	//Frame
	ofRectangle redRect, greenRect, blueRect;
	string toString(double x);	//Return string with 2 digits precision
	ofColor getColor1(const VideoFile *f);
	ofColor getColor2(const VideoFile *f);

	string edgeOrientIntToString(int orientation);

	/////////////////////////////////////Objects/////////////////////////////////////////

	float fontSize=14;	//font size
	float x;		//x coord where to draw
	float y;		//y coord where to draw
	float width;
	float height;
	int maxColumnSize;
	ofColor color1;
	ofColor color2;
	ofColor fontColor;
};

