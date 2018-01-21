//
// Created by Pedro Martins on 11-07-2017.
// Processing library
//

#include "processing.h"
#include <iostream>
#include <stdio.h>
//#include <opencv2/opencv.hpp>
#include <queue>


using namespace std;
using namespace cv;

processing::processing() {
	//ctor
}

processing::~processing() {
	//dtor
}

double processing::processFocus(Mat colorMat) {

	Mat greyMat;
	cvtColor(colorMat, greyMat, CV_BGR2GRAY);

	Mat lap;
	Laplacian(greyMat, lap, CV_64F);

	Scalar mu, sigma;
	meanStdDev(lap, mu, sigma);

	double focusMeasure = sigma.val[0] * sigma.val[0];

	return focusMeasure;
}

vector<double> processing::processHues(Mat src) {

	vector<double> returnVec;
	returnVec.assign(3, 0.0);
	//histogram peak counter method
	Mat dst;
	/// Separate the image in 3 places ( H, S and V )
	vector<Mat> hsv_planes;
	split(src, hsv_planes);

	/// Establish the number of bins
	int histSize = 180;

	/// Set the ranges ( for Hue) )
	float range[] = { 0, 180 };
	const float *histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	Mat h_hist;

	/// Compute the histogram for hue
	calcHist(&hsv_planes[0], 1, 0, Mat(), h_hist, 1, &histSize, &histRange, uniform, accumulate);
	//lets count the peaks on the hue histogram, we dont care about the first and last value
	int hues = 0;
	for (int i = 1; i < h_hist.total() - 1; ++i) {
		float left = h_hist.at<float>(i - 1);
		float cent = h_hist.at<float>(i);
		float right = h_hist.at<float>(i + 1);

		// we have to set a boundary condition for 'plateaus',
		// so just decide to have the cutoff on the left side
		if (left < cent && right <= cent) {
			if ((cent - left) > 150.0) //we discard too similar hues
				hues++;
		}
	}

	//Scalar meanSaturation = mean(hsv_planes[1]);
	//Scalar meanBrigthness = mean(hsv_planes[2]);
	Scalar output = (hues, mean(hsv_planes[1])[0], mean(hsv_planes[2])[0], 0);
	//cout << " " << mean(hsv_planes[0])[0] << " " << mean(hsv_planes[2])[0] << " " << mean(hsv_planes[1])[0] << endl;

	returnVec[0] = hues;
	returnVec[1] = mean(hsv_planes[1])[0];
	returnVec[2] = mean(hsv_planes[2])[0];

	return returnVec;

}

vector<double> processing::processHaarCascade(Mat &colorMat, CascadeClassifier &finder, CascadeClassifier &aditional_cascade,
	bool insideFace, Mat &ruleImage) {
	vector<Rect> faces;
	vector<Rect> eyes;
	Mat frame_gray;
	int totalFaces = 0;
	int totalEyes = 0;
	double totalFaceArea = 0.0;
	double totalRof3 = 0.0;
	int frameRuleValue = 0;
	vector<double> returnVec;
	returnVec.assign(4, 0.0);

	cvtColor(colorMat, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	finder.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	totalFaces += faces.size();
	//-- detect smiles or other things
	aditional_cascade.detectMultiScale(frame_gray, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(20, 20));

	for (int i = 0; i < faces.size(); i++) {
		Point faceCenter(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
		//ellipse( colorMat, faceCenter, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360,
		// Scalar( 255, 0, 255 ), 4, 8, 0 );

		Mat faceROI = frame_gray(faces.at(i));

		Rect temp = faces.at(i);
		int w = temp.width;
		int h = temp.height;
		totalFaceArea += (float)(w * h) / (frame_gray.cols * frame_gray.rows);

		//compute rule of thirds
		Point pt;
		pt.x = temp.x + w / 2;
		pt.y = temp.y + h / 2;
		frameRuleValue = ruleImage.at<unsigned char>(pt.x, pt.y);
		totalRof3 += (double)frameRuleValue / faces.size();

		//if smiles center inside face rect count 1
		if (insideFace) {
			int eyeCount = 0;

			for (size_t j = 0; j < eyes.size(); j++) {

				Point eyeCenter(eyes[j].x + eyes[j].width * 0.5, eyes[j].y + eyes[j].height * 0.5);
				bool fineSmile = false;

				if (eyeCenter.inside(temp)) {
					if (eyeCenter.y > faceCenter.y && eyes[j].width < temp.width / 1.3 &&
						eyes[j].width > temp.width / 3)
						fineSmile = true;

					if (fineSmile) {
						// ellipse( colorMat, eyeCenter, Size( eyes[j].width*0.5, eyes[j].height*0.5), 0, 0, 360,
						// Scalar( 255, 255, 0 ), 2, 8, 0 );
						eyeCount++;
					}
					else {} //ellipse( colorMat, eyeCenter, Size( eyes[j].width*0.5, eyes[j].height*0.5), 0, 0, 360,
							// Scalar( 0, 255, 0 ), 1, 4, 0 );

				}
				else {}// ellipse( colorMat, eyeCenter, Size( eyes[j].width*0.5, eyes[j].height*0.5), 0, 0, 360,
					   // Scalar( 255, 0, 255 ), 1, 4, 0 );
				fineSmile = false;
			}
			if (eyeCount > 0) totalEyes++;
		}
		else {

			for (size_t j = 0; j < eyes.size(); j++) {
				totalEyes++;

				Point center(eyes[j].x + eyes[j].width * 0.5, eyes[j].y + eyes[j].height * 0.5);
				int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
				//circle( colorMat, center, radius, Scalar( 255, 0, 0 ), 2, 6, 0 );
			}

		}

	}

	returnVec[0] = totalFaces;
	returnVec[1] = totalFaceArea;
	returnVec[2] = totalRof3;
	returnVec[3] = totalEyes;
	return returnVec;

}

vector<Mat> processing::splitMat(Mat inMat, double divide, bool bgr2gray) {

	Mat allMat, region_frame;

	if (bgr2gray)
		cvtColor(inMat, allMat, CV_BGR2GRAY);
	else allMat = inMat;

	int block_width = allMat.cols * divide; //0.25
	int block_heigth = allMat.rows * divide; //0.25
	vector<Mat> regions;

	for (int y = 0; y <= (allMat.rows - block_heigth); y += block_heigth) {
		for (int x = 0; x <= (allMat.cols - block_width); x += block_width) {
			Rect region = Rect(x, y, block_width, block_heigth);
			region_frame = allMat(region);
			regions.push_back(region_frame);
		}
	}

	return regions;

}

//Compute Edge Distributions
vector<double> processing::processEdgeHistogram(Mat greyMat) {

	vector<double> orientations;
	orientations.assign(5, 0.0);

	GaussianBlur(greyMat, greyMat, Size(3, 3), 0, 0, BORDER_DEFAULT);

	//5 kernels
	Mat vert_edge_filter = (cv::Mat1f(2, 2) << 1, -1, 1, -1);
	Mat hor_edge_filter = (cv::Mat1f(2, 2) << 1, 1, -1, -1);
	Mat diagr_edge_filter = (cv::Mat1f(2, 2) << sqrt(2), 0, 0, -(sqrt(2)));
	Mat diagl_edge_filter = (cv::Mat1f(2, 2) << 0, sqrt(2), -(sqrt(2)), 0);
	Mat nond_edge_filter = (cv::Mat1f(2, 2) << 2, -2, -2, 2);

	Mat vEdges, hEdges, diagr_edges, diagl_edges, nondEdges;

	filter2D(greyMat, vEdges, CV_8U, vert_edge_filter);
	filter2D(greyMat, hEdges, CV_8U, hor_edge_filter);
	filter2D(greyMat, diagr_edges, CV_8U, diagr_edge_filter);
	filter2D(greyMat, diagl_edges, CV_8U, diagl_edge_filter);
	filter2D(greyMat, nondEdges, CV_8U, nond_edge_filter);

	//mean value for each filter2D result
	Scalar mVEdge = mean(vEdges);
	Scalar mHEdge = mean(hEdges);
	Scalar m45Edge = mean(diagr_edges);
	Scalar m135Edge = mean(diagl_edges);
	Scalar mNondEdge = mean(nondEdges);
	//cout << mVEdge << " " << mHEdge << " "<< m45Edge << " " << m135Edge << " " << mNondEdge << endl;

	int response = 0;
	double temp_max = 0;

	if (mVEdge[0] > temp_max) {
		temp_max = mVEdge[0];
		response = 1;
	}
	if (mHEdge[0] > temp_max) {
		temp_max = mHEdge[0];
		response = 2;
	}
	if (m45Edge[0] > temp_max) {
		temp_max = m45Edge[0];
		response = 3;
	}
	if (m135Edge[0] > temp_max) {
		temp_max = m135Edge[0];
		response = 4;
	}
	if (mNondEdge[0] > temp_max)
		response = 5;

	vector<double> output;
	output.assign(2, 0.0);
	output[0] = response;
	output[1] = temp_max / 100;
	//cout << output[0] << output[1] << endl;
	return output;

}

int processing::processEHGroup(vector<int> edgeComplete) {

	int zero = 0;
	int v = 0;
	int h = 0;
	int d45 = 0;
	int d135 = 0;
	int nond = 0;

	for (int i = 0; i < edgeComplete.size(); i++) {

		if (edgeComplete[i] == 0) zero++;
		if (edgeComplete[i] == 1) v++;
		if (edgeComplete[i] == 2) h++;
		if (edgeComplete[i] == 3) d45++;
		if (edgeComplete[i] == 4) d135++;
		if (edgeComplete[i] == 5) nond++;

	}

	int temp = 0;
	int maxi = 0;

	if (nond > zero) {
		maxi = nond;
		temp = 5;
	}
	if (d135 > maxi) {
		maxi = d135;
		temp = 4;
	}
	if (d45 > maxi) {
		maxi = d45;
		temp = 3;
	}
	if (h > maxi) {
		maxi = h;
		temp = 2;
	}
	if (v > maxi) {
		temp = 1;
	}

	return temp;

}

float processing::entropy(Mat seq, Size size, int index) {
	int cnt = 0;
	float entr = 0;
	float total_size = size.height * size.width; //total size of all symbols in an image

	for (int i = 0; i < index; i++) {
		float sym_occur = seq.at<float>(i, 0); //the number of times a symbol has occured
		if (sym_occur > 0) //log of zero goes to infinity
		{
			cnt++;
			entr += (sym_occur / total_size) * (log2(total_size / sym_occur));
		}
	}
	//cout<<"cnt: "<<cnt<<endl;
	return entr;

}

// myEntropy calculates relative occurrence of different symbols within given input sequence using histogram
Mat processing::myEntropy(Mat seq, int histSize) {

	float range[] = { 0, 255 };
	const float *histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	Mat hist;

	/// Compute the histograms:
	calcHist(&seq, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

	return hist;
}

//-------Precise Dominant color computation--------------------------------------
//https://github.com/aishack/dominant-colors/blob/master/dominant.cpp

typedef struct t_color_node {
	cv::Mat       mean;       // The mean of this node
	cv::Mat       cov;
	uchar         classid;    // The class ID

	t_color_node  *left;
	t_color_node  *right;
} t_color_node;

cv::Mat get_dominant_palette(std::vector<cv::Vec3b> colors) {
	const int tile_size = 64;
	cv::Mat ret = cv::Mat(tile_size, tile_size*colors.size(), CV_8UC3, cv::Scalar(0));

	for (int i = 0; i<colors.size(); i++) {
		cv::Rect rect(i*tile_size, 0, tile_size, tile_size);
		cv::rectangle(ret, rect, cv::Scalar(colors[i][0], colors[i][1], colors[i][2]), CV_FILLED);
	}

	return ret;
}

std::vector<t_color_node*> get_leaves(t_color_node *root) {
	std::vector<t_color_node*> ret;
	std::queue<t_color_node*> queue;
	queue.push(root);

	while (queue.size() > 0) {
		t_color_node *current = queue.front();
		queue.pop();

		if (current->left && current->right) {
			queue.push(current->left);
			queue.push(current->right);
			continue;
		}

		ret.push_back(current);
	}

	return ret;
}

std::vector<cv::Vec3b> get_dominant_colors(t_color_node *root) {
	std::vector<t_color_node*> leaves = get_leaves(root);
	std::vector<cv::Vec3b> ret;

	for (int i = 0; i<leaves.size(); i++) {
		cv::Mat mean = leaves[i]->mean;
		ret.push_back(cv::Vec3b(mean.at<double>(0)*255.0f,
			mean.at<double>(1)*255.0f,
			mean.at<double>(2)*255.0f));
	}

	return ret;
}

int get_next_classid(t_color_node *root) {
	int maxid = 0;
	std::queue<t_color_node*> queue;
	queue.push(root);

	while (queue.size() > 0) {
		t_color_node* current = queue.front();
		queue.pop();

		if (current->classid > maxid)
			maxid = current->classid;

		if (current->left != NULL)
			queue.push(current->left);

		if (current->right)
			queue.push(current->right);
	}

	return maxid + 1;
}

void get_class_mean_cov(cv::Mat img, cv::Mat classes, t_color_node *node) {
	const int width = img.cols;
	const int height = img.rows;
	const uchar classid = node->classid;

	cv::Mat mean = cv::Mat(3, 1, CV_64FC1, cv::Scalar(0));
	cv::Mat cov = cv::Mat(3, 3, CV_64FC1, cv::Scalar(0));

	// We start out with the average color
	double pixcount = 0;
	for (int y = 0; y<height; y++) {
		cv::Vec3b* ptr = img.ptr<cv::Vec3b>(y);
		uchar* ptrClass = classes.ptr<uchar>(y);
		for (int x = 0; x<width; x++) {
			if (ptrClass[x] != classid)
				continue;

			cv::Vec3b color = ptr[x];
			cv::Mat scaled = cv::Mat(3, 1, CV_64FC1, cv::Scalar(0));
			scaled.at<double>(0) = color[0] / 255.0f;
			scaled.at<double>(1) = color[1] / 255.0f;
			scaled.at<double>(2) = color[2] / 255.0f;

			mean += scaled;
			cov = cov + (scaled * scaled.t());

			pixcount++;
		}
	}

	cov = cov - (mean * mean.t()) / pixcount;
	mean = mean / pixcount;

	// The node mean and covariance
	node->mean = mean.clone();
	node->cov = cov.clone();

	return;
}

void partition_class(cv::Mat img, cv::Mat classes, uchar nextid, t_color_node *node) {
	const int width = img.cols;
	const int height = img.rows;
	const int classid = node->classid;

	const uchar newidleft = nextid;
	const uchar newidright = nextid + 1;

	cv::Mat mean = node->mean;
	cv::Mat cov = node->cov;
	cv::Mat eigenvalues, eigenvectors;
	cv::eigen(cov, eigenvalues, eigenvectors);

	cv::Mat eig = eigenvectors.row(0);
	cv::Mat comparison_value = eig * mean;

	node->left = new t_color_node();
	node->right = new t_color_node();

	node->left->classid = newidleft;
	node->right->classid = newidright;

	// We start out with the average color
	for (int y = 0; y<height; y++) {
		cv::Vec3b* ptr = img.ptr<cv::Vec3b>(y);
		uchar* ptrClass = classes.ptr<uchar>(y);
		for (int x = 0; x<width; x++) {
			if (ptrClass[x] != classid)
				continue;

			cv::Vec3b color = ptr[x];
			cv::Mat scaled = cv::Mat(3, 1,
				CV_64FC1,
				cv::Scalar(0));

			scaled.at<double>(0) = color[0] / 255.0f;
			scaled.at<double>(1) = color[1] / 255.0f;
			scaled.at<double>(2) = color[2] / 255.0f;

			cv::Mat this_value = eig * scaled;

			if (this_value.at<double>(0, 0) <= comparison_value.at<double>(0, 0)) {
				ptrClass[x] = newidleft;
			}
			else {
				ptrClass[x] = newidright;
			}
		}
	}
	return;
}

cv::Mat get_quantized_image(cv::Mat classes, t_color_node *root) {
	std::vector<t_color_node*> leaves = get_leaves(root);

	const int height = classes.rows;
	const int width = classes.cols;
	cv::Mat ret(height, width, CV_8UC3, cv::Scalar(0));

	for (int y = 0; y<height; y++) {
		uchar *ptrClass = classes.ptr<uchar>(y);
		cv::Vec3b *ptr = ret.ptr<cv::Vec3b>(y);
		for (int x = 0; x<width; x++) {
			uchar pixel_class = ptrClass[x];
			for (int i = 0; i<leaves.size(); i++) {
				if (leaves[i]->classid == pixel_class) {
					ptr[x] = cv::Vec3b(leaves[i]->mean.at<double>(0) * 255,
						leaves[i]->mean.at<double>(1) * 255,
						leaves[i]->mean.at<double>(2) * 255);
				}
			}
		}
	}

	return ret;
}

cv::Mat get_viewable_image(cv::Mat classes) {
	const int height = classes.rows;
	const int width = classes.cols;

	const int max_color_count = 12;
	cv::Vec3b *palette = new cv::Vec3b[max_color_count];
	palette[0] = cv::Vec3b(0, 0, 0);
	palette[1] = cv::Vec3b(255, 0, 0);
	palette[2] = cv::Vec3b(0, 255, 0);
	palette[3] = cv::Vec3b(0, 0, 255);
	palette[4] = cv::Vec3b(255, 255, 0);
	palette[5] = cv::Vec3b(0, 255, 255);
	palette[6] = cv::Vec3b(255, 0, 255);
	palette[7] = cv::Vec3b(128, 128, 128);
	palette[8] = cv::Vec3b(128, 255, 128);
	palette[9] = cv::Vec3b(32, 32, 32);
	palette[10] = cv::Vec3b(255, 128, 128);
	palette[11] = cv::Vec3b(128, 128, 255);

	cv::Mat ret = cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
	for (int y = 0; y<height; y++) {
		cv::Vec3b *ptr = ret.ptr<cv::Vec3b>(y);
		uchar *ptrClass = classes.ptr<uchar>(y);
		for (int x = 0; x<width; x++) {
			int color = ptrClass[x];
			if (color >= max_color_count) {
				printf("You should increase the number of predefined colors!\n");
				continue;
			}
			ptr[x] = palette[color];
		}
	}

	return ret;
}

t_color_node* get_max_eigenvalue_node(t_color_node *current) {
	double max_eigen = -1;
	cv::Mat eigenvalues, eigenvectors;

	std::queue<t_color_node*> queue;
	queue.push(current);

	t_color_node *ret = current;
	if (!current->left && !current->right)
		return current;

	while (queue.size() > 0) {
		t_color_node *node = queue.front();
		queue.pop();

		if (node->left && node->right) {
			queue.push(node->left);
			queue.push(node->right);
			continue;
		}

		cv::eigen(node->cov, eigenvalues, eigenvectors);
		double val = eigenvalues.at<double>(0);
		if (val > max_eigen) {
			max_eigen = val;
			ret = node;
		}
	}

	return ret;
}

std::vector<cv::Vec3b> processing::find_dominant_colors(cv::Mat img, int count,int filecount, int framecount, bool imwrite) {
	const int width = img.cols;
	const int height = img.rows;

	cv::Mat classes = cv::Mat(height, width, CV_8UC1, cv::Scalar(1));
	t_color_node *root = new t_color_node();

	root->classid = 1;
	root->left = NULL;
	root->right = NULL;

	t_color_node *next = root;
	get_class_mean_cov(img, classes, root);
	for (int i = 0; i<count - 1; i++) {
		next = get_max_eigenvalue_node(root);
		partition_class(img, classes, get_next_classid(root), next);
		get_class_mean_cov(img, classes, next->left);
		get_class_mean_cov(img, classes, next->right);
	}

	std::vector<cv::Vec3b> colors = get_dominant_colors(root);
	if (imwrite) {
		cv::Mat quantized = get_quantized_image(classes, root);
		cv::Mat viewable = get_viewable_image(classes);
		cv::Mat dom = get_dominant_palette(colors);
		/*
		std::string s;
		s.append("./data/dominant/classification_");
		s.append(std::to_string(filecount));
		s.append("_");
		s.append(std::to_string(framecount));
		s.append(".png");
		cv::imwrite(s, viewable);

		std::string s2;
		s2.append("./data/dominant/quantized_");
		s2.append(std::to_string(filecount));
		s2.append("_");
		s2.append(std::to_string(framecount));
		s2.append(".png");
		cv::imwrite(s2, quantized);
		*/
		//write PNG file with dominant colors
		std::string s3;
		s3.append("./data/dominant/");
		s3.append(std::to_string(filecount));
		s3.append("_");
		s3.append(std::to_string(framecount));
		s3.append("_palette.png");
		cv::imwrite(s3, dom);
	}
	return colors;
}

//---end dominant color computation-------------------------------------------