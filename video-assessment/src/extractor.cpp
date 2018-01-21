//
// Created by Pedro Martins on 12-07-2017.
//

#include "extractor.h"
#include "RunningStats.h"
#include <chrono>

using namespace std;
using namespace cv;
using namespace cv::ml;
using namespace saliency;
using namespace cv::dnn;

#ifndef isnan
#define isnan(a) (a != a)
#endif

//feature extraction configuration parsed from extractor_config.xml

int samplingFactor = 1;                     
bool edgeHist = true;              
int resizeMode = 1;               
bool entro = true;              
bool opticalFlow = true;          
bool semanticAnalysis = true;     
bool dominantColors = false;
int numberOfDominantColors = 5;
bool saveDominantPallete = false;
bool sSaliency = true;
bool haar = true;
bool hsv = true;
bool focus = true;
bool bgSub = true;
bool colorfullness = true;

//bg subtraction parameters

bool smoothMask = true;
bool update_bg_model = true;
int method = 1;

//dominant colors, frame number
std::vector< std::pair<std::vector<cv::Vec3b>, int> > dominantDataVector; 

// running statistics class is used to compute statistics in one pass trough the data
RunningStats runstatRed, runstatGreen, runstatBlue, runstatLuminance,
runstatEntropy, runstatHues, runstatFocus, runstatUflowx, runstatUflowy,
runstatSflowx, runstatSflowy, runstatMag, runstatMeanColorfullness,
runstatStdColorfullness,runstatColofull, runstatSaturation, runstatBrightness;

//variables to save Mean, Std. deviation, Skewness and Kurtosis of distributions
double R1, R2, R3, R4, G1, G2, G3, G4, B1, B2, B3, B4, LU1, LU2, LU3, LU4,
E1, E2, E3, E4, H1, H2, H3, H4, F1, F2, F3, F4, UFLOWX1, UFLOWX2, UFLOWX3, UFLOWX4,
UFLOWY1, UFLOWY2, UFLOWY3, UFLOWY4, SFLOWX1, SFLOWX2, SFLOWX3, SFLOWX4,
SFLOWY1, SFLOWY2, SFLOWY3, SFLOWY4, MAG1, MAG2, MAG3, MAG4, RG1, RG2,
YB1, YB2, SAT1, SAT2, BRI1, BRI2, CF1, CF2;

// semantic analysis variables and data structures

std::vector<String> classNames;
std::vector<double> probabilities;
int numberOfMatches = 5; // find top 5 best matches

// build a map of {probability, classId} in decreasing order, highest probability first
vector< pair<double, int> >  semanticMap;

String modelTxt = "../bin/data/dnn/bvlc_googlenet.prototxt";
String modelBin = "../bin/data/dnn/bvlc_googlenet.caffemodel";
Net net;

processing pp;                   //processing class object
utility uu;                      //utility class object

json jAestheticSample = {};         
json jInterestSample = {};
json jsonAll = {};            

//color ratios
double redRatio, greenRatio, blueRatio;

///face recognition
int totalFaces;
int totalEyes;
float totalFaceArea;
double totalRof3;

extractor::extractor() {} 	//ctor

extractor::~extractor() {}  	//dtor

/* Find best class for the blob (i. e. class with maximal probability) */
void getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
	Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
	Point classNumber;
	minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
	*classId = classNumber.x;
}
std::vector<String> readClassNames(const char *filename = "../bin/data/dnn/synset_words.txt")
{
	std::ifstream fp(filename);
	if (!fp.is_open())
	{
		std::cerr << "File with classes labels not found: " << filename << std::endl;
		exit(-1);
	}
	std::string name;
	while (!fp.eof())
	{
		std::getline(fp, name);
		if (name.length())
			classNames.push_back(name.substr(name.find(' ') + 1));
	}
	cout << filename << " loaded!" << endl;
	fp.close();
	return classNames;
}

void extractor::init() {
	cv::dnn::initModule();  //Required if OpenCV is built as static libs
	net = dnn::readNetFromCaffe(modelTxt, modelBin);
	classNames = readClassNames();

	//-- 1. Load the cascades for object detection
	if (!face_cascade.load(face_cascade_name)) {
		cout << "--(!)Error loading\n";
		exit(0);
	};
	if (!aditional_cascade.load(aditional_cascade_name)) {
		cout << "--(!)Error loading\n";
		exit(0);
	};
	if (method == 1) bg_model = createBackgroundSubtractorKNN().dynamicCast<BackgroundSubtractor>();
	else if (method == 2)
		bg_model = createBackgroundSubtractorMOG2(200, 16.0, true).dynamicCast<BackgroundSubtractor>();

}

void extractor::initVectors(unsigned long numFiles) {

	//var initialization

	facesVec = facesAreaVec = facesRof3Vec = eyesVec = redRatio = greenRatio = 
    blueRatio = fpsVec = widthVec = heightVec = staticSaliencyVec = shackiness = 0.0;
	edgeHistogramVec.assign(numFiles, vector<int>(17, 0));
	edgeDistributionVec.assign(17, vector<int>(5, 0));
	probabilities.assign(1000, 0);
	bgSubVec.clear();
	dominantDataVector.clear();
	runstatRed.Clear();
	runstatGreen.Clear();
	runstatBlue.Clear();
	runstatLuminance.Clear();
	runstatEntropy.Clear();
	runstatHues.Clear();
	runstatFocus.Clear();
	runstatUflowx.Clear();
	runstatUflowy.Clear();
	runstatSflowx.Clear();
	runstatSflowy.Clear();
	runstatMag.Clear();
	runstatMeanColorfullness.Clear();
	runstatStdColorfullness.Clear();
	runstatSaturation.Clear();
	runstatBrightness.Clear();
	runstatColofull.Clear();
	R1 = R2 = R3 = R4 = G1 = G2 = G3 = G4 = B1 = B2 = B3 = B4 = LU1 = LU2 =
	LU3 = LU4 = E1 = E2 = E3 = E4 = H1 = H2 = H3 = H4 = F1 = F2 = F3 = F4 = 
	UFLOWX1 = UFLOWX2 = MAG1 = MAG2 = MAG3 = MAG4 = UFLOWX3 = UFLOWX4 = UFLOWY1 =
	UFLOWY2 = UFLOWY3 = UFLOWY4 = SFLOWX1 = SFLOWX2 = SFLOWX3 = SFLOWX4 =
	SFLOWY1 = SFLOWY2 = SFLOWY3 = SFLOWY4 = RG1 = RG2 = YB1 = YB2 = SAT1 = SAT2 =
	BRI1 = BRI2 = CF1 = CF2 = 0.0;
	EH_edges_distribution.clear();
}

double extractor::processStaticSaliency(Ptr<Saliency> staticSaliencyAlgorithm, Mat image) {

	Mat saliencyMap;
	staticSaliencyAlgorithm->computeSaliency(image, saliencyMap);
	cv::Scalar avgPixelIntensity = cv::mean(saliencyMap);
	return isnan(avgPixelIntensity.val[0]) ? 0.0 : avgPixelIntensity.val[0];
}

void extractor::processColors(Mat colorMat) {

	Scalar colAvg, colStds, greyAvg, greyStds;
	meanStdDev(colorMat, colAvg, colStds);
	runstatRed.Push(colAvg[2]);
	runstatGreen.Push(colAvg[1]);
	runstatBlue.Push(colAvg[0]);

	Mat greyMat;
	cvtColor(colorMat, greyMat, CV_BGR2GRAY);
	meanStdDev(greyMat, greyAvg, greyStds);
	runstatLuminance.Push(greyAvg[0]);

	//colorfullness vars
	if (colorfullness)
	{
		// C++ algorithm implementation of the
	    // "Measuring colourfulness in natural images"
		// (Hasler and Susstrunk, 2003)

		double RGmean = colAvg[2] - colAvg[1];
		double YBmean = ((RGmean) / 2) - colAvg[0];
		double RGstd = colStds[2] - colStds[1];
		double YBstd = ((RGstd) / 2) - colStds[0];

		double colorfullnessMean = sqrt(pow(RGmean, 2) + pow(YBmean, 2));
		runstatMeanColorfullness.Push(colorfullnessMean);
		double colorfullnessStd = sqrt(pow(RGstd, 2) + pow(YBstd, 2));
		runstatStdColorfullness.Push(colorfullnessStd);

		double ColorFull = colorfullnessStd + 0.3*colorfullnessMean;
		runstatColofull.Push(ColorFull);
	}
}

void extractor::extract(int frameCount) {

	R1 = runstatRed.Mean() / 255;
	R2 = runstatRed.StandardDeviation() / 255;
	R3 = ensureFormat( runstatRed.Skewness() / 255 );
	R4 = ensureFormat( runstatRed.Kurtosis() / 255 );
	G1 = runstatGreen.Mean() / 255;
	G2 = runstatGreen.StandardDeviation() / 255;
	G3 = ensureFormat( runstatGreen.Skewness() / 255 );
	G4 = ensureFormat( runstatGreen.Kurtosis() / 255 );
	B1 = runstatBlue.Mean() / 255;
	B2 = runstatBlue.StandardDeviation() / 255;
	B3 = ensureFormat( runstatBlue.Skewness() / 255 );
	B4 = ensureFormat( runstatBlue.Kurtosis() / 255 );
	LU1 = runstatLuminance.Mean() / 255;
	LU2 = runstatLuminance.StandardDeviation() / 255;
	LU3 = ensureFormat( runstatLuminance.Skewness() / 255 );
	LU4 = ensureFormat( runstatLuminance.Kurtosis() / 255 );
	E1 = runstatEntropy.Mean() /12 ;
	E2 = ensureFormat( runstatEntropy.StandardDeviation() / 5 ); //adhoc norm [0,1]
	E3 = ensureFormat( runstatEntropy.Skewness() / 20 ); //adhoc norm [0,1] 
	E4 = ensureFormat( runstatEntropy.Kurtosis() / 1000 ); //adhoc norm [0,1]
	H1 = ensureFormat( runstatHues.Mean() / 50 );
	H2 = ensureFormat( runstatHues.StandardDeviation() /50);
	H3 = ensureFormat( runstatHues.Skewness() / 50);
	H4 = ensureFormat( runstatHues.Kurtosis() /50);
	F1 = ensureFormat( runstatFocus.Mean() / 4000 );
	F2 = ensureFormat( runstatFocus.StandardDeviation() /10000 ); //adhoc norm [0,1]
	F3 = ensureFormat( runstatFocus.Skewness() / 50 );
	F4 = ensureFormat( runstatFocus.Kurtosis() /300 ); //adhoc norm [0,1]

	double total = R1 + G1 + B1;
	redRatio = R1 / total;
	greenRatio = G1 / total;
	blueRatio = B1 / total;

	MAG1 = ensureFormat( runstatMag.Mean() / 20000 );
	MAG2 = ensureFormat( runstatMag.StandardDeviation()/20000 );
	MAG3 = ensureFormat( runstatMag.Skewness() / 100 );
	MAG4 = ensureFormat( runstatMag.Kurtosis() / 3500 );
	UFLOWX1 = ensureFormat( runstatUflowx.Mean()/20000 );
	UFLOWX2 = ensureFormat( runstatUflowx.StandardDeviation()/20000 );
	UFLOWX3 = ensureFormat(runstatUflowx.Skewness() / 100 );
	UFLOWX4 = ensureFormat( runstatUflowx.Kurtosis() / 3500 );
	UFLOWY1 = ensureFormat( runstatUflowy.Mean() / 20000 );
	UFLOWY2 = ensureFormat( runstatUflowy.StandardDeviation() / 20000 );
	UFLOWY3 = ensureFormat( runstatUflowy.Skewness() / 100 );
	UFLOWY4 = ensureFormat( runstatUflowy.Kurtosis() / 2000 ); //adhoc norm [0,1]
	SFLOWX1 = ensureFormat( runstatSflowx.Mean() / 20000 );
	SFLOWX2 = ensureFormat( runstatSflowx.StandardDeviation() / 20000 );
	SFLOWX3 = ensureFormat( runstatSflowx.Skewness() / 100 );
	SFLOWX4 = ensureFormat( runstatSflowx.Kurtosis() / 3500 ); //adhoc norm [0,1]
	SFLOWY1 = ensureFormat( runstatSflowy.Mean() / 20000 );
	SFLOWY2 = ensureFormat( runstatSflowy.StandardDeviation() / 20000 );
	SFLOWY3 = ensureFormat( runstatSflowx.Skewness() / 100 );
	SFLOWY4 = ensureFormat( runstatSflowx.Kurtosis() / 3500 ); //adhoc norm [0,1]

	RG1 = runstatMeanColorfullness.Mean() / 255;
	RG2 = runstatMeanColorfullness.StandardDeviation() / 255;
	YB1 = runstatStdColorfullness.Mean() / 255;
	YB2 = runstatStdColorfullness.StandardDeviation() / 255;

	SAT1 = runstatSaturation.Mean() / 255;
	SAT2 = runstatSaturation.StandardDeviation() / 255;
	BRI1 = runstatBrightness.Mean() / 255;
	BRI2 = runstatBrightness.StandardDeviation() / 255;
	CF1 = runstatColofull.Mean();
	CF2 = runstatColofull.StandardDeviation();


	//cout << "\n [C] colorfullness: " << RG1 << " " << YB1 << " " << RG2 << " " << YB2 << " " << endl;
	//cout << "\n [E] entropy stats: " << E1 << " " << E2 << " " << E3 << " " << E4 << " " << endl;
	//cout << " [H] hues stats: " << H1 << " " << H2 << " " << H3 << " " << H4 << " " << endl;
	//cout << " [F] focus stats: " << F1 << " " << F2 << " " << F3 << " " << F4 << " " << endl;
	//cout << "\n [FLOW] mag stats: " << MAG1 << " " << MAG2 << " " << MAG3 << " " << MAG4 << " " << endl;
	//cout << " [FLOW] uflowx stats: " << UFLOWX1 << " " << UFLOWX2 << " " << UFLOWX3 << " " << UFLOWX4 << " " << endl;
	//cout << " [FLOW] uflowy stats: " << UFLOWY1 << " " << UFLOWY2 << " " << UFLOWY3 << " " << UFLOWY4 << " " << endl;
	//cout << " [FLOW] sflowx stats: " << SFLOWX1 << " " << SFLOWX2 << " " << SFLOWX3 << " " << SFLOWX4 << " " << endl;
	//cout << " [FLOW] sflowy stats: " << SFLOWY1 << " " << SFLOWY2 << " " << SFLOWY3 << " " << SFLOWY4 << " " << endl;

}
vector <pair <double, int > > extractor::getSemanticMap() {

	return semanticMap;
}

void extractor::extractFromVideo(string filePath, int nv) {

	getConfigParams();

	static_saliency_algorithm = "SPECTRAL_RESIDUAL";
	//instantiates the specific static Saliency
	Ptr<Saliency> staticSaliencyAlgorithm = Saliency::create(static_saliency_algorithm);

	//load rule of thirds template
	ruleImage = imread("../bin/data/templates/rule.jpg", CV_LOAD_IMAGE_GRAYSCALE);   // Read the iamge from file

	if (ruleImage.data == nullptr)                              // Check for invalid input
	{
		std::cout << "Could not open or find the rule image" << endl;
		exit(0);
	}

	initVectors(1);

	edgeStrenght = 0.0;
	totalFaces = 0;
	totalFaceArea = 0;
	totalEyes = 0;
	totalRof3 = 0;
	accumStaticSaliency = 0.0;
	percentBg = 0.0;
	percentShadow = 0.0;
	percentForegorund = 0.0;
	percentCameraMove = 0.0;
	percentFocus = 0.0;

	try {
		cap.open(filePath.c_str());
	}
	catch (Exception &e) {
		const char *err_msg = e.what();
		cout << "exception!: " << err_msg << std::endl;
	}

	int length = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
	fpsVec = cap.get(CV_CAP_PROP_FPS);
	widthVec = int(cap.get(CV_CAP_PROP_FRAME_WIDTH));
	heightVec = int(cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	if (samplingFactor > 1) {
		opticalFlow = false;
		bgSub = false;
	}

	if (nv == 1) {

		cout << " [*] Sampling factor:" << samplingFactor << endl;
		cout << " [*] Resize:" << resizeMode << endl;
		cout << " [*] Background subtraction:" << bgSub << endl;
		cout << " [*] Haar cascade:" << haar << endl;
		cout << " [*] Edge histograms:" << edgeHist << endl;
		cout << " [*] Entropy:" << entro << endl;
		cout << " [*] Hues:" << hsv << endl;
		cout << " [*] Focus:" << focus << endl;
		cout << " [*] Dominant colors:" << dominantColors << endl;
		cout << " [*] Save pallete:" << saveDominantPallete << endl;
		cout << " [*] Static saliency:" << sSaliency << endl;
		cout << " [*] Optical flow:" << opticalFlow << endl;
		cout << " [*] Semantic analysis:" << semanticAnalysis << endl;
		cout << " [*] Colorfullness:" << colorfullness << endl;
		cout << endl;
	}

	cout << std::setprecision(3) << std::fixed << " [" << nv << "] " << fpsVec << "fps " << widthVec << "x" << heightVec << " "
		<< "frames:" << length << " duration:" << length / fpsVec << endl;


	if (length < 90000) { //50 minutes at 30 fps

		//float totalFocus = 0.0;
		int frameCount = 0;
		bool once = false;
		bool onceTwice = false;
		int shakes = 0;
		string name = "";

		Mat flow, cflow;
		UMat grayFrame, prevgray, uflow;
		Point v1, v2; //for flow angle measure

		for (;;) //loop trough the video frames
		{
			Mat frame; //matrix to save pixel data
			auto n = 0;

			while (n < samplingFactor)   //we will jump as much frames as the sampling factor
			{
				cap >> frame;
				n++;
				frameCount++;
			}

			if (frame.data == nullptr) break;  //if frame doesnt contain data exit loop						

			cout << "\r [P] processed frames: " << frameCount;
			Mat frameTemp;

			switch (resizeMode)    //resize acording to -r argument
			{
			case 1:
				resize(frame, frameTemp, Size(320, 240), 0, 0, INTER_NEAREST);
				frameTemp.copyTo(frame);
				break;
			case 2:
				resize(frame, frameTemp, Size(480, 360), 0, 0, INTER_NEAREST);
				frameTemp.copyTo(frame);
				break;
			case 3:
				resize(frame, frameTemp, Size(640, 480), 0, 0, INTER_NEAREST);
				frameTemp.copyTo(frame);
				break;
			default:
				break;
			}

			if (!once) //this will run only once
			{
				//we need to resize the rule of thirds template to exactly the same size of the video frame
				ruleImage = uu.resizeRuleImg(ruleImage, frame);
				once = true;
				v1 = Point2f(0.0, 0.0);
			}

			if (!onceTwice && frameCount >= length / 5) { //creating thumbnails at 1/5th of video duration

				double tempHeigth;
				if (frame.rows > frame.cols)
					tempHeigth = (thumbnailWidth / frame.rows*frame.cols);			
				else tempHeigth = thumbnailHeight; 

				Mat thumbnailImage;
				resize(frame, thumbnailImage, Size(thumbnailWidth, tempHeigth), 0, 0, INTER_NEAREST);
				size_t lastindex1 = filePath.find_last_of("\\");
				name = filePath.substr(lastindex1);
				size_t lastindex2 = name.find_last_of(".");
				name = name.substr(0, lastindex2);
				string path = thumbnailFolderPath + name + ".jpg";
				imwrite(path, thumbnailImage);
				onceTwice = true;
			}
			if (dominantColors && frameCount % 30 == 0) {

				std::pair<std::vector<cv::Vec3b>, int> pairTemp;
				pairTemp = std::make_pair(pp.find_dominant_colors(frame, numberOfDominantColors, nv, frameCount, saveDominantPallete), frameCount);
				dominantDataVector.push_back(pairTemp);
				//cout << " " << dominantDataVector.size() << " " << pairTemp.first[0] << " " << pairTemp.first[1] << " " <<
				//	pairTemp.first[2] << " " << pairTemp.first[3] << " " << pairTemp.first[4] << endl;
			}

			if (semanticAnalysis && frameCount % 30 == 0) {

				Mat googleSizedMat;
				frame.copyTo(googleSizedMat);                                  //copy current video frame
				resize(googleSizedMat, googleSizedMat, Size(224, 224));        //GoogLeNet accepts only 224x224 RGB-images
				dnn::Blob inputBlob = dnn::Blob::fromImages(googleSizedMat);   //Convert Mat to dnn::Blob batch of images

				net.setBlob(".data", inputBlob); //set the network input
				net.forward(); //compute output

				dnn::Blob prob = net.getBlob("prob");   //gather output of "prob" layer
				int classId;
				double classProb;
				getMaxClass(prob, &classId, &classProb);//find the best class
				//std::cout << " Best class: #" << classId << " '" << classNames.at(classId) << "'" << std::endl;
				//std::cout << " Probability: " << classProb * 100 << "%" << std::endl;

				probabilities.at(classId) = (probabilities.at(classId) + (classProb / (length / 30)));

			}

			//auto start = chrono::high_resolution_clock::now();

			if (bgSub) {
				if (fgimg.empty())
					fgimg.create(frame.size(), frame.type());
				if (backimg.empty())
					backimg.create(frame.size(), frame.type());

				//update the model
				bg_model->apply(frame, fgmask, update_bg_model ? -1 : 0);

				int w = frame.cols;
				int h = frame.rows;
				double total = w * h;
				float frameFG = 0.0;
				float frameShadow = 0.0;
				float frameBg;
				float frameFocus;
				float bgFocus;

				for (int y = 0; y < h; y++) {

					for (int x = 0; x < w; x++) {

						const int pixelValue = fgmask.at<uchar>(y, x);

						if (pixelValue == 255)
							frameFG += 1;

						if (pixelValue == 127)
							frameShadow += 1;
					}
				}

				if (smoothMask) {
					GaussianBlur(fgmask, fgmask, Size(11, 11), 3.5, 3.5);
					threshold(fgmask, fgmask, 10, 255, THRESH_BINARY);
				}

				fgimg = Scalar::all(0);
				frame.copyTo(fgimg, fgmask);
				backimg = Scalar::all(0);
				frame.copyTo(backimg, bgmask);

				Mat bgimg;
				bg_model->getBackgroundImage(bgimg);

				frameFG = (float)(frameFG / total);
				frameShadow = (float)(frameShadow / total);
				frameBg = 1 - frameFG - frameShadow;
				percentForegorund += frameFG;
				percentShadow += frameShadow;
				percentBg += frameBg;

				if (frameFG >= 0.80) percentCameraMove += 1;

				frameFocus = (float)pp.processFocus(fgimg);
				bgFocus = (float)pp.processFocus(backimg);
				float focusDiff = frameFocus / bgFocus;

				if (isinf(focusDiff))focusDiff = 30;
				if (isnan(focusDiff))focusDiff = 0;
				if (focusDiff > 30) focusDiff = 30;
				percentFocus += focusDiff;

			}
			//	auto end = chrono::high_resolution_clock::now();
			//	cout << "bgsub processed in: " << std::chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

			if (opticalFlow) {

				frame.copyTo(grayFrame);
				cvtColor(grayFrame, grayFrame, COLOR_BGR2GRAY);

				//auto start2 = chrono::high_resolution_clock::now();

				Point2f unsignedFrameFlow = Point2f(0.0, 0.0);
				Point2f signedFrameFlow = Point2f(0.0, 0.0);
				int w = grayFrame.cols;
				int h = grayFrame.rows;

				if (!prevgray.empty()) {

					cv::calcOpticalFlowFarneback(prevgray, grayFrame, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
					cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
					uflow.copyTo(flow);

					for (int y = 0; y < h; y += 5) {
						for (int x = 0; x < w; x += 5) {

							// get the flow from y, x position 
							const Point2f flowatxy = flow.at<Point2f>(y, x);
							//cout << "flow x,y: " << flowatxy.x << "," << flowatxy.y << endl;
							unsignedFrameFlow.x += abs(flowatxy.x);
							unsignedFrameFlow.y += abs(flowatxy.y);
							signedFrameFlow.x += flowatxy.x;
							signedFrameFlow.y += flowatxy.y;

						}
					}
				}

				auto frameMagnitude = (float)((sqrt(pow(unsignedFrameFlow.x, 2) + pow(unsignedFrameFlow.y, 2))));
				runstatMag.Push(frameMagnitude);
				runstatUflowx.Push(unsignedFrameFlow.x);
				runstatUflowy.Push(unsignedFrameFlow.y);
				runstatSflowx.Push(signedFrameFlow.x);
				runstatSflowy.Push(signedFrameFlow.y);

				v2.x = (int)unsignedFrameFlow.x;
				v2.y = (int)unsignedFrameFlow.y;

				float angle = uu.innerAngle(v1.x, v1.y, v2.x, v2.y, w / 2, h / 2); //compute angle between two vectors		
				if (angle >= 70.0 ) //check shake based on vectors angle with threshold of 10
				{
					shakes++;
					//cout << "angle: " << angle << endl;
					// cout<< "shakes: "<<shakes<<endl;
				}

				std::swap(prevgray, grayFrame);
				v1.x = v2.x;
				v1.y = v2.y;
				//auto end2 = chrono::high_resolution_clock::now();
				//cout << "flow processed in: " << std::chrono::duration_cast<chrono::milliseconds>(end2 - start2).count() << " ms" << endl;
			}

			if (sSaliency)
				accumStaticSaliency += processStaticSaliency(staticSaliencyAlgorithm, frame);

			if (focus) {
				double focusMeasure = pp.processFocus(frame);
				runstatFocus.Push(focusMeasure);
			}
			if (hsv) {

				vector<double> scalartemp = pp.processHues(frame);

				runstatHues.Push(scalartemp[0]);
				runstatSaturation.Push(scalartemp[1]);
				runstatBrightness.Push(scalartemp[2]);
				//cout << " "<<scalartemp[0] << " " << scalartemp[1] << " " << scalartemp[2] << endl;
				//cout << " "<< scalartemp[1] << endl;
			}			

			processColors(frame);

			if (entro) {
				Mat src, hist;
				cvtColor(frame, src, CV_BGR2GRAY);
				// Establish the number of bins
				int histSize = 256;
				hist = pp.myEntropy(src, histSize);
				//cout << "hist " << hist << endl;
				float entropy = pp.entropy(hist, src.size(), histSize);
				//frame_entropy_distribution.push_back(entropy);
				runstatEntropy.Push(entropy);
			}

			if (edgeHist) {
				vector<Mat> cutImage = pp.splitMat(frame, 0.25, true); //0.25(=1/4) <=> split in 16 squares(4x4)

			   //for each piece compute edge histogram coefficients
				vector<int> edgeComplete;
				edgeComplete.assign(17, 0);

				for (int i = 0; i < 16; i++) {
					vector<double> edgeData = pp.processEdgeHistogram(cutImage[i]);

					int edgeHistogramExtracted = edgeData[0];
					edgeComplete[i] = edgeHistogramExtracted;
					edgeDistributionVec[i].push_back(edgeComplete[i]);
					edgeStrenght += edgeData[1];
				}

				int edgeOrientation = pp.processEHGroup(edgeComplete);
				EH_edges_distribution.push_back(edgeOrientation);

			}

			if (haar) {
				vector<double> faceData = pp.processHaarCascade(frame, face_cascade, aditional_cascade, insideFace,
					ruleImage);
				if (faceData[0] >= 1)
					totalFaces += 1;

				totalFaceArea += faceData[1];
				totalRof3 += faceData[2];
				totalEyes += faceData[3];
			}

		} //end for(;;) cycle

		double divider = frameCount / samplingFactor;

		if (edgeHist) {
			for (int i = 0; i < 16; i++) {
				edgeHistogramVec[0][i] = pp.processEHGroup(edgeDistributionVec[i]);
			}

			edgeHistogramVec[0][16] = pp.processEHGroup(EH_edges_distribution);
			edgeStrenght = (((double)edgeStrenght) / divider / 8);
		}

		if (haar) {
			double facePercent = (double)totalFaces / divider;
			double eyePercent = (double)totalEyes / divider;
			double areaPercent = totalFaceArea / divider;
			double rof3computed = totalRof3 / divider;

			//eliminate false positives
			if (facePercent < 0.01) {
				facePercent = 0.0;
				areaPercent = 0.0;
				rof3computed = 0.0;
			}
			if (facePercent >= 1.0) facePercent = 1.0;

			facesVec = facePercent;
			eyesVec = eyePercent;
			facesAreaVec = (float)areaPercent;
			facesRof3Vec = rof3computed / 255; //normalized		
		}

		if (sSaliency) {
			staticSaliencyVec = (double)accumStaticSaliency / divider;
		}
	
		if (opticalFlow) {
			shackiness = (double)shakes / frameCount;
		}
	/*	else {
			UFLOWX1 = UFLOWX2 = MAG1 = MAG2 = MAG3 = MAG4 = UFLOWX3 = UFLOWX4 = UFLOWY1 =
			UFLOWY2 = UFLOWY3 = UFLOWY4 = SFLOWX1 = SFLOWX2 = SFLOWX3 = SFLOWX4 =
			SFLOWY1 = SFLOWY2 = SFLOWY3 = SFLOWY4 = 0.0;
		}*/
		
	    vector<double> frameBgData = vector<double>(5, 0);
		if (bgSub) {

			frameBgData.at(0) = percentForegorund / divider;
			frameBgData.at(1) = percentShadow / divider;
			frameBgData.at(2) = percentBg / divider;
			frameBgData.at(3) = percentCameraMove / divider;
			frameBgData.at(4) = percentFocus / (divider) / 24;		
		}
		bgSubVec = frameBgData;

		double vSize = (widthVec*heightVec) / 3000000;
		double vFps = (fpsVec / 100);
		rank_sum = (vSize + vFps) / 4;

		if (semanticAnalysis) {

			std::multimap<double, int, greater<double> > semanticTempMap;

			int h = 0;
			while (h < probabilities.size()) {

				if (probabilities.at(h) != 0)
					semanticTempMap.insert(std::pair<double, int>(probabilities.at(h), h));
				h++;
			}

			vector< pair<double, int> > semanticFinalMap;
			std::multimap<double, int, greater<double> >::iterator mapIt;

			// select the best matches, which are in the first positions in the sorted map
			int index = 0;
			cout<<"\n";
			for (mapIt = semanticTempMap.begin(); mapIt != semanticTempMap.end(); mapIt++)
			{
				pair<double, int> x = *mapIt;
				semanticFinalMap.push_back(x);
				cout << " [S] " << x.second << " " << x.first * 100 << "% " << classNames.at(x.second) << endl;
				if (++index == numberOfMatches)
					break; // stop when first bestMatches are copied	
			}

			semanticMap = semanticFinalMap;

		}
		extract(frameCount);
		cap.release();

		/* samples for SVM classification*/
		jInterestSample = {

			{ "01",		redRatio },		        //red_ratio
			{ "02",		SFLOWX2 },		        //sflowx_std
			{ "03",     H1 },		            //dif_hues
			{ "04",		length / fpsVec },		//duration
			{ "05",     H4 },		            //hues_kurtosis
			{ "06",     widthVec },		        //width
			{ "07",     UFLOWY2 },              //uflowy_std
			{ "08",     UFLOWY4 },              //uflowy_kurtosis
			{ "09",     RG1 },                  //colorfullness_rg1
			{ "10",     R3 },                   //red_moments3
			{ "11",     F1 },                   //focus
			{ "12",     greenRatio },           //green_ratio
			{ "13",     YB1 },                  //colorfullness_yb1
			{ "14",     F4 },                   //focus_kurtosis
			{ "15",     UFLOWX1 },              //uflowx_mean
			{ "16",     MAG4 },                 //mag_kurtosis
			{ "17",     UFLOWX4 },              //uflowx_kurtosis
			{ "18",     blueRatio },            //blue_ratio
			{ "19",     facesVec },             //faces
			{ "20",     E3 },                   //entropy_skewness
			{ "21",     H2 },                   //hues_std
			{ "22",     E1 },                   //entropy
			{ "23",     heightVec },            //height
			{ "24",     B3 },                   //blue_moments3
			{ "25",     fpsVec / 60 },          //fps
			{ "26",     H3 },                   //hues_skewness
			{ "27",     UFLOWY3 },              //uflowy_skewness
			{ "28",     B1 },                   //blue_moments1
			{ "29",     F3 },                   //focus_skewness
			{ "30",     eyesVec }               //smiles

		};


		jAestheticSample = {

			{ "01",		SFLOWX2 },          //sflowx_std
			{ "02",		widthVec },         //width
			{ "03",		E2 },		        //entropy_std
			{ "04",		F1 },		        //focus
			{ "05",		facesRof3Vec },		//rule_of_thirds
			{ "06",		MAG4 },		        //mag_kurtosis
			{ "07",     fpsVec / 60 },		    //fps
			{ "08",     SFLOWY2 },		    //sflowy_std
			{ "09",     E3 },		        //entropy_skewness
			{ "10",     SFLOWX3 },		    //sfx3
			{ "11",     eyesVec },		    //smiles
			{ "12",     SFLOWY1 },	     	//sflowy_mean
			{ "13",     heightVec },		//height
			{ "14",     H1},		        //dif_hues
			{ "15",     SFLOWX1 },		     //sflowx mean
			{ "16",     G2 },	          	//green moment 2
			{ "17",     B4 },		        //blue_moments4
			{ "18",     R1 },		        //red moment 1
			{ "19",     shackiness },		//shackiness
			{ "20",     MAG1 },		        //mag 1
			{ "21",     SFLOWY4 },		    //sfy4
			{ "22",     bgSubVec.at(4) }    //focus diff.

		};

		

		jsonAll = {

			{ "width",   widthVec },
			{ "height",   heightVec },
			{ "red_ratio",       redRatio },
			{ "red_moments1",   R1 },
			{ "red_moments2",   R2 },
			{ "green_ratio",     greenRatio },
			{ "green_moments1", G1 },
			{ "green_moments2", G2 },
			{ "blue_ratio",      blueRatio },
			{ "blue_moments1",  B1 },
			{ "blue_moments2",  B2 },
			{ "focus",           F1 },
			{ "luminance",       LU1 },
			{ "luminance_std",   LU2 },
			{ "red_moments3",   R3 },
			{ "red_moments4",   R4 },
			{ "green_moments3", G3 },
			{ "green_moments4", G4 },
			{ "blue_moments3",  B3 },
			{ "blue_moments4",  B4 },
			{ "dif_hues",        H1 },
			{ "faces",           facesVec },
			{ "faces_area",      facesAreaVec },
			{ "smiles",          eyesVec },
			{ "rule_of_thirds",  facesRof3Vec },
			{ "static_saliency", staticSaliencyVec },
			{ "rank_sum",   rank_sum },
			{ "fps",   fpsVec / 60 },
			{ "shackiness",      shackiness },
			{ "motion_mag",      MAG1 },
			{ "fg_area",         bgSubVec.at(0) },
			{ "shadow_area",     bgSubVec.at(1) },
			{ "bg_area",         bgSubVec.at(2) },
			{ "camera_move",     bgSubVec.at(3) },
			{ "focus_diff",      bgSubVec.at(4) },
		    { "hues_std",   H2 },
		    { "hues_skewness",   H3 },
		    { "hues_kurtosis",   H4 },
			{ "eh_0",           (double)(edgeHistogramVec[0][0]) },
			{ "eh_1",           (double)(edgeHistogramVec[0][1]) },
			{ "eh_2",           (double)(edgeHistogramVec[0][2]) },
			{ "eh_3",           (double)(edgeHistogramVec[0][3]) },
			{ "eh_4",           (double)(edgeHistogramVec[0][4]) },
			{ "eh_5",           (double)(edgeHistogramVec[0][5]) },
			{ "eh_6",           (double)(edgeHistogramVec[0][6]) },
			{ "eh_7",           (double)(edgeHistogramVec[0][7]) },
			{ "eh_8",           (double)(edgeHistogramVec[0][8]) },
			{ "eh_9",           (double)(edgeHistogramVec[0][9]) },
			{ "eh_10",          (double)(edgeHistogramVec[0][10]) },
			{ "eh_11",          (double)(edgeHistogramVec[0][11]) },
			{ "eh_12",          (double)(edgeHistogramVec[0][12]) },
			{ "eh_13",          (double)(edgeHistogramVec[0][13]) },
			{ "eh_14",          (double)(edgeHistogramVec[0][14]) },
			{ "eh_15",          (double)(edgeHistogramVec[0][15]) },
			{ "eh_16",          (double)(edgeHistogramVec[0][16]) },
			{ "entropy",        E1 },
			{ "edge_strenght",   edgeStrenght },
			{ "luminance_skewness",   LU3 },
			{ "luminance_kurtosis",   LU4 },
		    { "entropy_std",   E2 },
			{ "entropy_skewness",   E3 },
			{ "entropy_kurtosis",   E4 },
			{ "focus_std",   F2 },
			{ "focus_skewness",   F3 },
			{ "focus_kurtosis",   F4 },
			{ "mag_std",   MAG2 },
			{ "mag_skewness",   MAG3 },
			{ "mag_kurtosis",   MAG4 },
			{ "uflowx_mean",   UFLOWX1 },
			{ "uflowx_std",   UFLOWX2 },
			{ "uflowx_skewness",   UFLOWX3 },
			{ "uflowx_kurtosis",   UFLOWX4 },
			{ "uflowy_mean",   UFLOWY1 },
			{ "uflowy_std",   UFLOWY2 },
			{ "uflowy_skewness",   UFLOWY3 },
			{ "uflowy_kurtosis",   UFLOWY4 },
			{ "sflowx_mean",   SFLOWX1 },
			{ "sflowx_std",   SFLOWX2 },
			{ "sflowx_skewness",   SFLOWX3 },
			{ "sflowx_kurtosis",   SFLOWX4 },
			{ "sflowy_mean",   SFLOWY1 },
			{ "sflowy_std",   SFLOWY2 },
			{ "sflowy_skewness",   SFLOWY3 },
			{ "sflowy_kurtosis",   SFLOWY4 },
			{ "colorfullness_rg1",   RG1 },
			{ "colorfullness_rg2",   RG2},
			{ "colorfullness_yb1",   YB1 },
			{ "colorfullness_yb2",   YB2 },
			{ "duration",   length / fpsVec },
	    	{ "saturation_1",   SAT1},
	    	{ "saturation_2",   SAT2 },
	    	{ "brightness_1",   BRI1 },
	    	{ "brightness_2",   BRI2 },
	    	{ "colorfull_1",   CF1 },
		    { "colorfull_2",   CF2 }
			
		};

	}
	else { cout << " [!] Large Video!" << endl; }
}

String extractor::getClassTextFromID(int ID)
{
	cout << "passed " << classNames.at(ID) << endl;
	return classNames.at(ID);
}

json extractor::getJsonA() {

	return jAestheticSample;
}

json extractor::getJsonI() {

	return jInterestSample;
}

json extractor::getJsonAll() {

	return jsonAll;
}

void extractor::getConfigParams() {

	ofXml* xml = new ofXml(configPath);
	if (xml->load(configPath)) {
		samplingFactor = xml->getValue<int>("//SAMPLING_FACTOR");
		resizeMode = xml->getValue<int>("//RESIZE");
		bgSub = xml->getValue<bool>("//BGSUB");
		haar = xml->getValue<bool>("//HAAR");
		edgeHist = xml->getValue<bool>("//EDGE_HIST");
		hsv = xml->getValue<bool>("//HSV");
		focus = xml->getValue<bool>("//FOCUS");
		dominantColors = xml->getValue<int>("//DOMINANT_COLORS");
		opticalFlow = xml->getValue<bool>("//FLOW");
		entro = xml->getValue<bool>("//ENTROPY");
		semanticAnalysis = xml->getValue<bool>("//SEMANTIC");
		sSaliency = xml->getValue<bool>("//SSALIENCY");
		saveDominantPallete = xml->getValue<bool>("//SAVEPALLETE");
		colorfullness = xml->getValue<bool>("//COLORFULLNESS");
	}

}

double extractor::ensureFormat(double input) {
	
	double output = input;
	if (isnan(input)) output = 0.0;
	if (output > 1) output = 1;

	return output;

}
string extractor::thumbnailFolderPath = "../bin/data/thumbnails/videos/";
