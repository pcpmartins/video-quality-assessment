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

using json = nlohmann::json;


class extractor {

public:
	extractor();

	virtual ~extractor();

	void initVectors(unsigned long nFiles);

	double processStaticSaliency(Ptr<Saliency> staticSaliencyAlgorithm, Mat image);

	void processColors(Mat colorMat);

	void extractFromVideo(string filePath, int nv);

	json getJson4ML();
	json getJsonExtra();

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

	String static_saliency_algorithm;
	Ptr<BackgroundSubtractor> bg_model;
	vector<double> bgSubVec;
	float percentBg, percentShadow, percentForegorund, percentFocus, percentCameraMove;

	//background subtraction
	bool bgSub = true;
	Mat fgmask, fgimg, backimg, bgmask;
	bool smoothMask = true;
	bool update_bg_model = true;
	int method = 1;

	//opticflow
	bool opticalFlow = true;
	float totalFlowX;
	float totalFlowY;
	//float totalFlowXBorder;
	// float totalFlowYBorder;
	double flowxVec, flowyVec, flowxAvgVec, flowyAvgVec, magFlowVec, shackinessVec; //total and average flow;
	double shackiness;

	float accumStaticSaliency;
	double staticSaliencyVec;

private:
	string configPath = "extractor_config.xml";               //Path to configuration file
	void getConfigParams();


};