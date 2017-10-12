#pragma once

#include "ofMain.h"
#include "File.h"
#include "VideoFile.h"
#include "ImageFile.h"
#include "metadataPanel.h"
#include "filtersPanel.h"
#include "cctype"
#include "extractor.h"
#include "mlclass.h"

#include <iostream>
#include <chrono>

class Gallery {
public:
	Gallery();
	Gallery(double width, double height);

	void test();
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	//void mouseEntered(int x, int y);
	//void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	bool isLocked();
	void lock();
	void unLock();
	bool extractVideoData();

	extractor ex; //instantiate feature extractor class
	mlclass mlc; //instantiate SVM classifier
	vector<String> fileNames;

private:
	/////////////////////////////////Methods////////////////////////////////
	string dataOutputPath = "data/output/output.csv"; //output from extraction process
	string inputFolder = "data/files/";               //video input files
	string xmlFolderPath = "/xml/";                   //Path to folder with metadata

	bool loadFiles();								//Load data to allFiles vector 	
	bool checkIfThumbnailClicked(int x, int y);		//Saves index to choosenFileIndex. Sets thumbnailClicked flag
	bool toolBarClicked(int x, int y);				//Check if "click" was over toolbar
	ofRectangle spaceForFileDisplay();

	bool parseCsvFeatureVector();					//Load data to allFiles vector 
	vector<string> getIndividualSample(string name);

	//////////////////////////////////Objects//////////////////////////////

	vector <VideoFile>	allFiles;				//Vector of files loaded at the initialization
	ImageFile choosenImage;					//Object of choosen image
	VideoFile choosenVideo;					//Object of choosen video
	metadataPanel metadataPanel;			//Object to display metadata
	filtersPanel filtersPanel;				//Object to filter displayed data
	ofRectangle fileSpace;					//Space avaiable for displaying image/video
	bool videoPlay;							//Flag if video should play
	vector <vector <string> >	csvData;
	int numberOfselectedFiles;

	/*Thumbnails parameteres*/
	int thumbnailsWidth;
	int thumbnailsHeight;
	int choosenFileIndex;					//Index of thumnbail pointed with mouse
	bool thumbnailClicked;

	/* Parameters for the panel */
	int gap;
	int margin;
	int downMargin;
	int scrollBarWidth;

	/*Buttons parameters*/
	//int buttonMargin;
	//int buttonRadius;

	//Text
	vector <ofTrueTypeFont> names;
	ofTrueTypeFont *name;
	ofTrueTypeFont *counter;
	ofTrueTypeFont *playerText;

	double percentageWidth;			//How many percent of screen width the panel takes [0..1]
	double percentageHeight;		//How many percent of screen height the panel takes [0..1]
	int panelWidth;
	int panelHeight;
	int availableWidth;
	int contentScrollY;
	bool isScrollBarVisible;
	ofRectangle scrollBarRectangle;
	ofRectangle gripRectangle;
	bool isDraggingGrip;
	bool isMouseOverGrip;
	int mousePreviousY;
	vector<ofRectangle> rectangles;

};
