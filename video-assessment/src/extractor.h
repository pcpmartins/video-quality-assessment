#pragma once
//
// Created by Pedro Martins on 12-07-2017.
//
#pragma once
#include "ofMain.h"
#include "processing.h"
#include "utility.h"
#include "mlclass.h"
#include "opencv2/objdetect.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/videoio.hpp"
#include <json.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/saliency.hpp>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace saliency;
using json = nlohmann::json;


class extractor {

public:
	extractor();

	virtual ~extractor();

	void init();

	void initVectors(unsigned long nFiles);

	double processStaticSaliency(Ptr<Saliency> staticSaliencyAlgorithm, Mat image);

	void processColors(Mat colorMat);

	vector <pair<double, int > > getSemanticMap();
	vector <double > getAudioMap();

	void extractFromVideo(string filePath, int nv);

	String getClassTextFromID(int ID);

	double ensureFormat(double input);

	void extract(int frameCount);


	json getJsonA();
	json getJsonI();
	json getJsonAll();

	int nFiles;                     //number of files to process
	VideoCapture cap;               //current videoCapture object
	double facesVec;
	double eyesVec;
	float facesAreaVec;
	double facesRof3Vec;
	Mat ruleImage;

	//our face classifier is not invariant to rotation
	//using smile detection as additional cascade classifier
	String face_cascade_name = "../bin/data/haar/haarcascade_frontalface_alt.xml";
	String aditional_cascade_name = "../bin/data/haar/smiled_05.xml";
	CascadeClassifier face_cascade;
	CascadeClassifier aditional_cascade;
	bool insideFace = true;

	int widthVec;
	int heightVec;
	double fpsVec;
	double rank_sum;

	vector<vector<int> > edgeHistogramVec;
	vector<vector<int> > edgeDistributionVec;
	vector<int> EH_edges_distribution;
	double edgeStrenght;

	String static_saliency_algorithm;
	Ptr<BackgroundSubtractor> bg_model;
	vector<double> bgSubVec;
	float percentBg, percentShadow, percentForegorund, percentFocus, percentCameraMove;

	//background subtraction
	Mat fgmask, fgimg, backimg, bgmask;

	double shackiness;

	float accumStaticSaliency;
	double staticSaliencyVec;
	string configPath = "extractor_config.xml";               //Path to configuration file
	static string thumbnailFolderPath;				          //Path to thumbnails
	int thumbnailHeight = 100;
	int thumbnailWidth = 120;

private:
	void getConfigParams();
	//void extract(int frameCount);
	

};


