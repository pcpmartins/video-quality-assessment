//
// Created by pedro on 21-07-2017.
//

#pragma once
#include <opencv2/ml.hpp>
#include <fstream>
#include <json.hpp>
using namespace std;
using namespace cv;
using namespace cv::ml;

using json = nlohmann::json;

class mlclass {

public:

	mlclass();

	virtual ~mlclass();

	void init();
	int predictSample(json jSample, int c);


protected:
	Ptr<SVM> processSVM(string binary_scores, string feature_vector,
		string classifier_name, double C, double G);

	vector <int> countData(string f_vector);
private:

};