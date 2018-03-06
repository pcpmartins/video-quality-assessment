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
	bool extractVideoThumbnails();
	bool extractVideoData();
	bool parseOnly = false;

	//bool savePreSortProcessing(int size);

	static string thumbnailFolderPath;				          //Path to thumbnails
	int thumbnailHeight = 150;
	int thumbnailWidth = 180;

	extractor ex; //instantiate feature extractor class
	mlclass mlc; //instantiate SVM classifier
	vector<String> fileNames;
	int totalFiles = 700;
	vector<String> clNames;

	//cheater sort
	//vector <vector< int  > > cheaterSortData;
	//bool loadPreSort();


private:
	/////////////////////////////////Methods////////////////////////////////
	string audioDataOutputPath = "data/audio/audio_result.csv";
	string semanticDataOutputPath = "data/output/semantic_data.csv"; //output from extraction process
	string dataOutputPath = "data/output/output.csv"; //output from extraction process
	string cheaterDataOutputPath = "data/output/cheatersort.csv"; //pre processing sort
	string inputFolder = "data/files/";               //video input files
	string xmlFolderPath = "/xml/";                   //Path to folder with metadata

	void getConfigParams();
	bool loadFiles();								//Load data to allFiles vector 	
	bool checkIfThumbnailClicked(int x, int y);		//Saves index to choosenFileIndex. Sets thumbnailClicked flag
	bool checkIfVideoPreviewClicked(int x, int y);	
	bool toolBarClicked(int x, int y);				//Check if "click" was over toolbar
	ofRectangle spaceForFileDisplay();

	bool parseSemanticVector();
	bool parseAudioVector();
	int parseCsvFeatureVector();					//Load data to allFiles vector 
	vector<string> getIndividualSample(string name);

	std::vector<String> readClassNames();

	//////////////////////////////////Objects//////////////////////////////

	vector <VideoFile>	allFiles;				//Vector of files loaded at the initialization
	//ImageFile choosenImage;					//Object of choosen image
	VideoFile choosenVideo;					//Object of choosen video
	metadataPanel metadataPanel;			//Object to display metadata
	filtersPanel filtersPanel;				//Object to filter displayed data
	ofRectangle fileSpace;					//Space avaiable for displaying image/video
	bool videoPlay;							//Flag if video should play
	vector <vector <string> >csvData;
	int numberOfselectedFiles;
	vector <vector< pair <double, int > > > semanticData;
	vector <vector< double > > audioData;

	/*Thumbnails parameteres*/
	int thumbnailsWidth;
	int thumbnailsHeight;
	int choosenFileIndex;					//Index of thumnbail pointed with mouse
	bool thumbnailClicked;
	bool videoPreviewClicked;

	/* Parameters for the panel */
	int gap;
	int margin;
	int downMargin;
	int scrollBarWidth;
	int topMargin;

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

	struct sort_pred {
		bool operator()(const std::pair<int, double> &left, const std::pair<int, double> &right) {
			return left.second < right.second;
		}
	};
};
