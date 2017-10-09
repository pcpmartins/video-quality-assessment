//
// Created by Pedro Martins on 11-07-2017.
//
#pragma once

#include <iostream>
#include <string>
#include<numeric>
#include <opencv2/core.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

class utility {
public:
	utility();

	virtual ~utility();

	Mat resizeRuleImg(Mat ruleImage, Mat frame);

	float innerAngle(float px1, float py1, float px2, float py2, float cx1, float cy1);

protected:

private:


};

