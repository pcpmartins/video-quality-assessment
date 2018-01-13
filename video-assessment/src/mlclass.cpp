//
// Created by pedro on 21-07-2017.
//

#include <chrono>
#include "mlclass.h"

using namespace std;
using namespace cv;
using namespace cv::ml;
using namespace chrono;

//Aesthetics classifier
string a_binary_scores = "../bin/data/SVM/A2018_21/A2018_binary21.csv";
string a_feature_vector = "../bin/data/SVM/A2018_21/A2018_train21_norm.csv";
string a_maxmin_path = "../bin/data/SVM/A2018_21/A2018_train21_maxmin.csv";

//Interestingness classifier
string i_binary_scores = "../bin/data/SVM/i_beach_gui/i_beach_bin_gui.csv";
string i_feature_vector = "../bin/data/SVM/i_beach_gui/i_beach_train_gui_nonorm_norm.csv";
string i_maxmin_path = "../bin/data/SVM/i_beach_gui/i_beach_train_gui_nonorm_maxmin.csv";

vector< vector<double> > trainingData, a_maxmin, i_maxmin;
vector< int > trainingLabels;
Ptr<SVM> svm_a;
Ptr<SVM> svm_i;

mlclass::mlclass() { 
	//ctor
}

mlclass::~mlclass() {
	//dtor
}

void mlclass::init() {

	auto start = chrono::high_resolution_clock::now();

	svm_a = processSVM(a_binary_scores, a_feature_vector, "Aesthetic", 2.56, 0.107374);
	svm_i = processSVM(i_binary_scores, i_feature_vector, "Interestingness", 2.56, 0.429497);

	auto end = chrono::high_resolution_clock::now();

	cout << "\n [!] Classifiers created in " << duration_cast<chrono::milliseconds>(end - start).count()
		<< " ms" << endl;

	a_maxmin.assign(2, vector<double>(21, 0));
	i_maxmin.assign(2, vector<double>(30, 0));

	ifstream inputData(a_maxmin_path);
	string current_line;
	int y = 0;

	while (getline(inputData, current_line)) {

		stringstream temp(current_line);
		std::string single_value;
		int z = 0;
		while (getline(temp, single_value, ',')) {

			a_maxmin[y][z] = atof(single_value.c_str()); 		
			z++;
		}
		y++;
	}
	inputData.close();

	ifstream inData(i_maxmin_path);
	string curr_line;
	int y2 = 0;

	while (getline(inData, curr_line)) {

		stringstream temp(curr_line);
		std::string single_value;
		int z = 0;
		while (getline(temp, single_value, ',')) {

			i_maxmin[y2][z] = atof(single_value.c_str());
			z++;
		}
		y2++;
	}
	inData.close();
};

Ptr<SVM> mlclass::processSVM(string binary_scores, string feature_vector,
	string classifier_name, double C, double G) {

	vector <int> countResult = mlclass::countData(feature_vector);
	int numberOfFeatures = countResult[1];
	int total_items = countResult[0];

	//allocate the arrays
	trainingData.assign(total_items, vector<double>(numberOfFeatures, 0));
	trainingLabels.assign(total_items, 0);

	///Print current classifier stats
	cout << "\n [M] dataset items: " << total_items << endl;
	cout << " [M] features: " << numberOfFeatures << endl;

	/// Set up label data----------------------------------------------------
	ifstream inputBinaryData(binary_scores);
	std::string current_binary_line;
	int x = 0;

	while (getline(inputBinaryData, current_binary_line)) {

		int temp = atoi(current_binary_line.c_str());
		trainingLabels[x] = temp;
		x++;
	}

	/// Set up feature data------------------------------------------------------
	ifstream inputFeatureData(feature_vector);
	string current_training_line;
	int y = 0;

	// Start reading lines as long as there are lines in the file
	while (getline(inputFeatureData, current_training_line)) {

		stringstream temp(current_training_line);
		std::string single_value;
		int z = 0;
		while (getline(temp, single_value, ',')) {

			trainingData[y][z] = atof(single_value.c_str()); // convert the string element to a float value
			z++;
		}
		y++;
	}

	///Setup the input matrixes for training and evaluating of the svm /////////////
	Mat trainingDataMat = Mat(total_items, numberOfFeatures, CV_32FC1);

	for (int i = 0; i < total_items; ++i)
		for (int j = 0; j < numberOfFeatures; ++j)
		{
			trainingDataMat.at<float>(i, j) = trainingData.at(i).at(j);
		}
	Mat trainingLabelsMat = Mat(total_items, 1, CV_32SC1);
	memcpy(trainingLabelsMat.data, trainingLabels.data(), trainingLabels.size() * sizeof(int));

	cout << " [M] " << trainingDataMat.size() << " feature vector" << endl;
	cout << " [M] " << trainingLabels.size() << " labels vector" << endl;

	// Create the SVM
	Ptr<SVM> s = SVM::create();
	s->setType(SVM::C_SVC);
	s->setKernel(SVM::RBF);
	s->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 10000, 1e-6));
	s->train(trainingDataMat, ROW_SAMPLE, trainingLabelsMat);
	s->setC(C);
	s->setGamma(G);

	cout << " [M] " << classifier_name << " Classifier generated!" << " C: "<<C<<" G: "<<G<< endl;
	return s;
}

//Predict sample
int mlclass::predictSample(json jSample, int c) {


	int sSize = jSample.size();
	float* sampleData = new float[sSize];
	float* sampleNormData = new float[sSize];
	int ps = 0;

	for (json::iterator it = jSample.begin(); it != jSample.end(); ++it) {
		//std::cout << it.key() << " : " << it.value() << "\n";
		sampleData[ps] = it.value();
		ps++;
	}

	if (c == 0) {
		
	for (int p = 0; p < sSize; p++)
			{
				double fMax = a_maxmin[0][p];
				//cout << " max " << fMax;
				double fMin = a_maxmin[1][p];
				//cout << " min " << fMin;
				double cValue = sampleData[p];
				//cout << " value " << cValue;
				double denominator = (fMax - fMin);
				double numerator = (cValue - fMin);
				//cout << "dois " << dois << " um " << um <<" ";
				float result = (float)numerator / denominator;
				result = (result > 1) ? 1 : result;
				result = (result < 0) ? 0 : result;
				sampleNormData[p] = result;
				//cout <<" res "<< sampleNormData[p] << " ";
			}
		
	//cout << endl;
	}
	else if (c == 1) {

		for (int p = 0; p < sSize; p++)
		{
			double fMax = i_maxmin[0][p];
			double fMin = i_maxmin[1][p];
			double cValue = sampleData[p];
			double denominator = (fMax - fMin);
			double numerator = (cValue - fMin);
			float result = (float)numerator / denominator;
			result = (result > 1) ? 1 : result;
			result = (result < 0) ? 0 : result;
			sampleNormData[p] = result;
		}

		//cout << endl;
	}

	Mat sampleDataMat(1, sSize, CV_32FC1, sampleNormData);

	///Show the decision given by the SVM

	int finalResponse = 0;
	if (c == 0) {
		finalResponse = (int)svm_a->predict(sampleDataMat);
		cout << " [M] aesthetics: " << finalResponse << endl;
	}
	else if (c == 1) {
		finalResponse = (int)svm_i->predict(sampleDataMat);
		cout << " [M] interestingness: " << finalResponse << endl;
	}
	delete[] sampleData;
	return finalResponse;

}
vector <int> mlclass::countData(string f_vector)
{

	/// Count features and samples //////////////////////////////////////////////////////
	ifstream inputFeatureCountData(f_vector.c_str());
	string current_line;
	int sampleCount = 0;     //number of samples
	int featureCount = 0;    //number of features

	// Start reading lines as long as there are lines in the file
	while (getline(inputFeatureCountData, current_line))
	{
		stringstream temp(current_line);
		string single_value;

		if (sampleCount == 0)
		{
			while (getline(temp, single_value, ','))
				featureCount++;
		}
		sampleCount++;
	}

	inputFeatureCountData.close();
	vector<int> result;
	result.assign(2, 0);

	result[0] = sampleCount;
	result[1] = featureCount;
	return result;
}