//
// Created by Pedro Martins on 12-07-2017.
//

#include "extractor.h"

using json = nlohmann::json;
using namespace cv;
using namespace cv::ml;
using namespace saliency;

int samplingFactor = 1;         //the rate of processing
bool quietMode = true;          //do we want low feedback?
bool edgeHist = true;            //edge histograms
int resizeMode = 1;             //resize video input

processing p;                   //processing class object
utility u;                      //utility class object

json jMLExtracted = {};         //output json object
json jMLExtracted2 = {};         //output json object

double focusVec;        //to save each item focus value
double luminanceVec;    //to save each item luminance value
double luminanceStdVec;    //to save each item luminance std value

//Color Moments (first and second)
pair<double, double> red_moments;
pair<double, double> green_moments;
pair<double, double> blue_moments;

double rm1, rm2, gm1, gm2, bm1, bm2;
//color ratios
double redRatio;
double greenRatio;
double blueRatio;
double redRatioVec;
double greenRatioVec;
double blueRatioVec;

//luminance value
double luminance_center;
double luminance_center_std;

//Color distributions (average,standard deviation) if (image)size = 1 else size > 1
vector<pair<double, double> > red_distribution;
vector<pair<double, double> > green_distribution;
vector<pair<double, double> > blue_distribution;
vector<pair<double, double> > luminance_distribution;

//hues
double differentHues;
double totalHues;
//double simplicityVec;

///face recognition
int totalFaces;
int totalEyes;
float totalFaceArea;
double totalRof3;

//entropy
double entropyVec;
vector<double> frame_entropy_distribution;


extractor::extractor() {
	//ctor

}

extractor::~extractor() {
	//dtor
}

void extractor::initVectors(unsigned long nFiles) {

	//lets start processing files, can be videos or images
	//we initialize vectors where metadata will be saved with the number of files to process
	rm1 = 0, rm2 = 0, gm1 = 0, gm2 = 0, bm1 = 0, bm2 = 0;
	focusVec = 0;
	facesVec = 0;
	facesAreaVec = 0.0;
	facesRof3Vec = 0;
	eyesVec = 0;
	luminanceVec = 0;
	luminanceStdVec = 0;
	redRatioVec = 0;
	greenRatioVec = 0;
	blueRatioVec = 0;
	entropyVec = 0;
	differentHues = 0;
	fpsVec = 0.0;
	widthVec = 0;
	heightVec = 0;
	staticSaliencyVec = 0;
	flowxAvgVec = 0;
	flowyAvgVec = 0;
	magFlowVec = 0;
	shackinessVec = 0;
	edgeHistogramVec.assign(nFiles, vector<int>(17, 0));
	edgeDistributionVec.assign(17, vector<int>(20, 0));

	//-- 1. Load the cascades for object detection
	if (!face_cascade.load(face_cascade_name)) {
		cout << "--(!)Error loading\n";
		exit(0);
	};
	if (!aditional_cascade.load(aditional_cascade_name)) {
		cout << "--(!)Error loading\n";
		exit(0);
	};
	if (bgSub) {
		if (method == 1) bg_model = createBackgroundSubtractorKNN().dynamicCast<BackgroundSubtractor>();
		else if (method == 2)
			bg_model = createBackgroundSubtractorMOG2(200, 16.0, true).dynamicCast<BackgroundSubtractor>();
	}

}

double extractor::processStaticSaliency(Ptr<Saliency> staticSaliencyAlgorithm, Mat image) {

	Mat saliencyMap;
	staticSaliencyAlgorithm->computeSaliency(image, saliencyMap);
	cv::Scalar avgPixelIntensity = cv::mean(saliencyMap);
	return isnan(avgPixelIntensity.val[0]) ? 0.0 : avgPixelIntensity.val[0];
}

void extractor::processColors(Mat colorMat) {
	Scalar colAvg, colStds, greyAvg, greyStds;

	//Compute moments of color for the image
	meanStdDev(colorMat, colAvg, colStds);
	blue_distribution.emplace_back(make_pair(colAvg[0], colStds[0]));
	green_distribution.emplace_back(make_pair(colAvg[1], colStds[1]));
	red_distribution.emplace_back(make_pair(colAvg[2], colStds[2]));

	//Compute moments of luminance for the image (grey-scale)
	Mat greyMat;
	cvtColor(colorMat, greyMat, CV_BGR2GRAY);
	meanStdDev(greyMat, greyAvg, greyStds);
	luminance_distribution.emplace_back(make_pair(greyAvg[0], greyStds[0]));
}

void extract(int frameCount) {

	luminance_center_std = p.processMoments(luminance_distribution).second;
	luminance_center = p.processMoments(luminance_distribution).first;
	red_moments = p.processMoments(red_distribution);
	green_moments = p.processMoments(green_distribution);
	blue_moments = p.processMoments(blue_distribution);
	double total = red_moments.first + green_moments.first + blue_moments.first;
	redRatio = red_moments.first / total;
	greenRatio = green_moments.first / total;
	blueRatio = blue_moments.first / total;
	luminanceStdVec = luminance_center_std / 255; //luminance std normalization
	luminanceVec = luminance_center / 255; //luminance normalization
	rm1 = red_moments.first;
	rm2 = red_moments.second;
	gm1 = green_moments.first;
	gm2 = green_moments.second;
	bm1 = blue_moments.first;
	bm2 = blue_moments.second;
	redRatioVec = redRatio;
	greenRatioVec = greenRatio;
	blueRatioVec = blueRatio;
	double hue_norm = totalHues / 360;
	differentHues = abs(redRatio - greenRatio) < 0.0006 ? 0 : hue_norm;
	processing::Mean meanE(frame_entropy_distribution.size());
	entropyVec = std::accumulate(frame_entropy_distribution.begin(),
		frame_entropy_distribution.end(), 0.0, meanE) / 12;

	///print results
	double divider = frameCount / samplingFactor;

	if (!quietMode) {
		cout << "\n [?] Focus: " << focusVec << " lum: " << luminanceVec << " lumStd: "
			<< luminanceStdVec << "\n";
		cout << " [?] Faces: " << (double)totalFaces / (divider) << " ";
		cout << " Face area: " << (double)totalFaceArea / (divider) << " Smiles: "
			<< (double)totalEyes / (divider) << "\n";
		cout << " [?] hues: " << differentHues << " " << " rule of thirds: " << (totalRof3 / (divider)) / 255;
	}
}

void extractor::extractFromVideo(string filePath, int nv) {

	getConfigParams();

	string path = "../" + filePath;

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
	//else cout << "rule image loaded" << endl;

	initVectors(1);

	edgeStrenght = 0.0;
	totalHues = 0;
	totalFaces = 0;
	totalFaceArea = 0;
	totalEyes = 0;
	totalRof3 = 0;
	accumStaticSaliency = 0.0;
	totalFlowX = 0.0;
	totalFlowY = 0.0;

	//bg subtraction parameters
	smoothMask = true;
	update_bg_model = true;

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
		std::cout << "exception!: " << err_msg << std::endl;
	}
	if (!cap.isOpened()) {
		cout << "Error opening video file" << endl;
	}
	int length = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
	fpsVec = cap.get(CV_CAP_PROP_FPS);
	widthVec = int(cap.get(CV_CAP_PROP_FRAME_WIDTH));
	heightVec = int(cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	if (samplingFactor > 1) {
		bgSub = false;
		opticalFlow = false;

	}

	cout << " [" << nv << "] " << fpsVec << "fps " << widthVec << "x" << heightVec << " " << "length:" << length << " " << "SF:"
		<< samplingFactor << endl;

	if (length < 90000) { //50 minutes at 30 fps

		float totalFocus = 0.0;
		int frameCount = 0;
		bool once = false;
		shackiness = 0.0;
		int shakes = 0;

		Mat flow, cflow;
		UMat grayFrame, prevgray, uflow; //optic flow
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
				ruleImage = u.resizeRuleImg(ruleImage, frame);
				once = true;
				v1 = Point2f(0.0, 0.0);
			}

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

				frameFocus = (float)p.processFocus(fgimg);
				bgFocus = (float)p.processFocus(backimg);
				float focusDiff = frameFocus / bgFocus;

				if (isinf(focusDiff))focusDiff = 30;
				if (isnan(focusDiff))focusDiff = 0;
				if (focusDiff > 30) focusDiff = 30;
				percentFocus += focusDiff;
			}

			if (opticalFlow) {

				cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

				Point2f frameFlow = Point2f(0.0, 0.0);
				Point2f frameFlowBorder = Point2f(0.0, 0.0);
				Point2f signedFrameFlow = Point2f(0.0, 0.0);
				int w = grayFrame.cols;
				int h = grayFrame.rows;

				if (!prevgray.empty()) {

					cv::calcOpticalFlowFarneback(prevgray, grayFrame, uflow, 0.5, 3, 15, 3, 5, 1.2, 0);
					cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
					uflow.copyTo(flow);

					for (int y = 0; y < h; y += 5) {
						for (int x = 0; x < w; x += 5) {

							// get the flow from y, x position * 10 for better visibility
							const Point2f flowatxy = flow.at<Point2f>(y, x);//*10

							frameFlow.x += abs(flowatxy.x);
							frameFlow.y += abs(flowatxy.y);
							signedFrameFlow.x += flowatxy.x;
							signedFrameFlow.y += flowatxy.y;

							if (x >= 20 && x <= w - 20 && y >= 20) {
								// line(cflow, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)),
								//     Scalar(255, 0, 0));
							}

							if (x < 20 || x > w - 20 || y < 20) {
								frameFlowBorder.x += flowatxy.x;
								frameFlowBorder.y += flowatxy.y;
								// circle(cflow, Point(x, y), 1, Scalar(0, 0, 0), -1); // draw initial point
							}
						}
					}
				}

				totalFlowX += frameFlow.x;
				totalFlowY += frameFlow.y;
				v2.x = (int)frameFlow.x;
				v2.y = (int)frameFlow.y;

				float angle = u.innerAngle(v1.x, v1.y, v2.x, v2.y, w / 2, h / 2); //compute angle between two vectors
				auto frameMagnitude = (float)((sqrt(pow(totalFlowX, 2) + pow(totalFlowY, 2))) / 70000);
				//cout<< "angle: "<<angle<<" mag: "<<frameMagnitude<<endl;

				if (angle >= 45.0 && frameMagnitude > 10.0) //check shake based on vectors angle with threshold of 10
				{
					shakes++;
					// cout<< "shakes: "<<shakes<<endl;
				}

				std::swap(prevgray, grayFrame);
				v1.x = v2.x;
				v1.y = v2.y;
			}

			accumStaticSaliency += processStaticSaliency(staticSaliencyAlgorithm, frame);

			double focusMeasure = p.processFocus(frame);
			totalFocus += focusMeasure;
			double frameHues = p.processHues(frame);
			totalHues += frameHues / (frameCount / samplingFactor);

			processColors(frame);

			/*Need to fix entropy processing, MSC compiler vs GCC !*/
			//entropy
			//Mat src, hist;
			//cvtColor(frame, src, CV_BGR2GRAY);
			// Establish the number of bins
			//int histSize = 256;
			//hist = p.myEntropy(src, histSize);
			//cout << "hist " << hist << endl;
			//float entropy = p.entropy(hist, src.size(), histSize);
			//frame_entropy_distribution.push_back(entropy);
			frame_entropy_distribution.push_back(0);

			if (edgeHist) {
				vector<Mat> cutImage = p.splitMat(frame, 0.25, true); //0.25(=1/4) <=> split in 16 squares(4x4)

				 //for each piece compute edge histogram coefficients
				vector<int> edgeComplete;
				edgeComplete.assign(17, 0);

				for (int i = 0; i < 16; i++) {
					vector<double> edgeData = p.processEdgeHistogram(cutImage[i]);

					int edgeHistogramExtracted = edgeData[0];
					edgeComplete[i] = edgeHistogramExtracted;
					edgeDistributionVec[i].push_back(edgeComplete[i]);
					edgeStrenght += edgeData[1];
				}

				int edgeOrientation = p.processEHGroup(edgeComplete);
				EH_edges_distribution.push_back(edgeOrientation);

			}
			vector<double> faceData = p.processHaarCascade(frame, face_cascade, aditional_cascade, insideFace,
				ruleImage);
			if (faceData[0] >= 1)
				totalFaces += 1;

			totalFaceArea += faceData[1];
			totalRof3 += faceData[2];
			totalEyes += faceData[3];
		}

		double divider = frameCount / samplingFactor;

		for (int i = 0; i < 16; i++) {
			edgeHistogramVec[0][i] = p.processEHGroup(edgeDistributionVec[i]);
		}

		edgeHistogramVec[0][16] = p.processEHGroup(EH_edges_distribution);
		edgeStrenght = (((double)edgeStrenght) / divider / 4);

		double medianFocus = totalFocus / divider;

		if (medianFocus > 8000)
			medianFocus = 8000;

		focusVec = medianFocus / 8000; //cut-off at 8000, very few records over that value

		double facePercent = (double)totalFaces / divider;
		double eyePercent = (double)totalEyes / divider;
		double areaPercent = totalFaceArea / divider;
		double rof3computed = totalRof3 / divider;
		double staticSaliencyComputed = (double)accumStaticSaliency / divider;

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
		differentHues = (int)totalHues;
		staticSaliencyVec = staticSaliencyComputed;

		extract(frameCount);

		flowxVec = totalFlowX;
		flowyVec = totalFlowY;
		flowxAvgVec = (totalFlowX / divider);
		flowyAvgVec = (totalFlowY / divider);
		magFlowVec = (sqrt(pow(flowxAvgVec, 2) + pow(flowyAvgVec, 2))) / 70000;
		shackinessVec = (double)shakes / frameCount;

		vector<double> frameBgData = vector<double>(5, 0);
		frameBgData.at(0) = percentForegorund / divider;
		frameBgData.at(1) = percentShadow / divider;
		frameBgData.at(2) = percentBg / divider;
		frameBgData.at(3) = percentCameraMove / divider;
		frameBgData.at(4) = percentFocus / (divider) / 24;
		bgSubVec = frameBgData;

		double vSize = (widthVec*heightVec) / 3000000;
		double vFps = (fpsVec / 100);
		rank_sum = (vSize + vFps) / 4;

		if (!quietMode) {

			string localOrient = " ";
			for (int i = 0; i < 16; i++) {
				localOrient += std::to_string(edgeHistogramVec[0][i]);
			}
			cout << "\n";
			cout << " [?] Orientation: " << edgeHistogramVec[0][16];
			cout << localOrient << endl;
			cout << " [?] entropy: " << entropyVec << endl;
			cout << " [?] static saliency: " << staticSaliencyComputed << endl;
			if (opticalFlow) {
				cout << " [?] flow: " << flowxVec << " " << flowyVec << " " << flowxAvgVec
					<< " " << flowyAvgVec << " " << magFlowVec << endl;
				cout << " [?] shaking: " << shakes << " " << shackinessVec << endl;
			}
			else cout << "\n\n";

			if (bgSub) {
				cout << " [?] bgsub: " << frameBgData.at(0) << " " << frameBgData.at(1) << " " << frameBgData.at(2)
					<< " " << frameBgData.at(3) << " " << frameBgData.at(4) << endl;
			}
		}

		luminance_distribution.clear();
		blue_distribution.clear();
		green_distribution.clear();
		red_distribution.clear();
		EH_edges_distribution.clear();
		frame_entropy_distribution.clear();

		cap.release();

		vector<double> bgsubdata = (vector<double> &&) bgSubVec;

		/* size 27 sample for SVM classification*/
		jMLExtracted = {

			{ "red_ratio",       redRatioVec },
			{ "green_ratio",     greenRatioVec },
			{ "blue_ratio",      blueRatioVec },
			{ "focus",           focusVec },
			{ "luminance",       luminanceVec },
			{ "luminance_std",   luminanceStdVec },
			{ "dif_hues",        differentHues },
			{ "faces",           facesVec },
			{ "faces_area",      facesAreaVec },
			{ "smiles",          eyesVec },
			{ "rule_of_thirds",  facesRof3Vec },
			{ "static_saliency", staticSaliencyVec },
			{ "shackiness",      shackinessVec },
			{ "motion_mag",      magFlowVec },
			{ "fg_area",         bgsubdata.at(0) },
			{ "shadow_area",     bgsubdata.at(1) },
			{ "bg_area",         bgsubdata.at(2) },
			{ "camera_move",     bgsubdata.at(3) },
			{ "focus_diff",      bgsubdata.at(4) }

		};

		jMLExtracted2 = {
			{ "eh_0",           (double)(edgeHistogramVec[0][0]) },
			{ "eh_1",           (double)(edgeHistogramVec[0][1])  },
			{ "eh_2",           (double)(edgeHistogramVec[0][2])  },
			{ "eh_3",           (double)(edgeHistogramVec[0][3])  },
			{ "eh_4",           (double)(edgeHistogramVec[0][4])  },
			{ "eh_5",           (double)(edgeHistogramVec[0][5])  },
			{ "eh_6",           (double)(edgeHistogramVec[0][6])  },
			{ "eh_7",           (double)(edgeHistogramVec[0][7])  },
			{ "eh_8",           (double)(edgeHistogramVec[0][8])  },
			{ "eh_9",           (double)(edgeHistogramVec[0][9])  },
			{ "eh_10",          (double)(edgeHistogramVec[0][10])  },
			{ "eh_11",          (double)(edgeHistogramVec[0][11])  },
			{ "eh_12",          (double)(edgeHistogramVec[0][12])  },
			{ "eh_13",          (double)(edgeHistogramVec[0][13])  },
			{ "eh_14",          (double)(edgeHistogramVec[0][14])  },
			{ "eh_15",          (double)(edgeHistogramVec[0][15])  },
			{ "eh_16",          (double)(edgeHistogramVec[0][16])  },
			{ "entropy",         entropyVec },
			{ "red_moments1",   rm1 },
			{ "red_moments2",   rm2 },
			{ "green_moments1", gm1 },
			{ "green_moments2", gm2 },
			{ "blue_moments1",  bm1 },
			{ "blue_moments2",  bm2 },
			{ "fps",   fpsVec },
			{ "width",   widthVec },
			{ "height",   heightVec },
			{ "rank_sum",   rank_sum },
			{ "edge_strenght",   edgeStrenght }

		};

	}
	else { cout << " [!] Large Video!" << endl; }
}

json extractor::getJson4ML() {

	return jMLExtracted;
}

json extractor::getJsonExtra() {

	return jMLExtracted2;
}

void extractor::getConfigParams() {

	ofXml* xml = new ofXml(configPath);
	if (xml->load(configPath)) {
		samplingFactor = xml->getValue<int>("//SAMPLING_FACTOR");
		edgeHist = xml->getValue<bool>("//EDGE_HIST");
		quietMode = xml->getValue<bool>("//QUIET");
		resizeMode = xml->getValue<int>("//RESIZE");
		bgSub = xml->getValue<bool>("//BGSUB");
		opticalFlow = xml->getValue<bool>("//FLOW");
	}

}
