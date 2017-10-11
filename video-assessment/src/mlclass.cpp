//
// Created by pedro on 21-07-2017.
//

#include <chrono>
#include "mlclass.h"

using namespace std;
using namespace cv;
using namespace cv::ml;
using namespace chrono;

bool binout = false;            //Do we want to output to file the aditional feature vector prediction results?
bool showPrediction = false;   //Do we want to see and compare each prediction alongside with experimental value?
const int features = 19;
int final_evaluation_items = 200;
string outputPath = "final_evaluation.csv";

//Aesthetics classifier

string a_binary_scores = "../bin/data/SVM/aesthetic_big/aesthetic_big_binary_data.csv";
string a_feature_vector = "../bin/data/SVM/aesthetic_big/aesthetic_big_train_data.csv";
string a_final_evaluation_vector = "../bin/data/SVM/aesthetic_big/aesthetic_big_200_evaluation_samples.csv";
int a_total_items = 700;
int a_evaluation_start = 190; //190 82%
int a_evaluation_items = 50;

//Interestingness classifier
string i_binary_scores = "../bin/data/SVM/interest_big/interest_big_binary_data.csv";
string i_feature_vector = "../bin/data/SVM/interest_big/interest_big_train_data.csv";
string i_final_evaluation_vector = "../bin/data/SVM/interest_big/interest_big_200_evaluation_samples.csv";
int i_total_items = 1200;
int i_evaluation_start = 600; //600 79%
int i_evaluation_items = 100;

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

	svm_a = processSVM(a_binary_scores, a_feature_vector, a_total_items,
		a_evaluation_start, a_evaluation_items, a_final_evaluation_vector, "Aesthetic");

	svm_i = processSVM(i_binary_scores, i_feature_vector, i_total_items,
		i_evaluation_start, i_evaluation_items, i_final_evaluation_vector, "Interestingness");

	auto end = chrono::high_resolution_clock::now();

	cout << "\n [!] Classifiers created in (ms): " << duration_cast<chrono::milliseconds>(end - start).count()
		<< '\n';
};

Ptr<SVM> mlclass::processSVM(string binary_scores, string feature_vector, int total_items,
	int evaluation_start, int evaluation_items,
	string final_evaluation_vector, string classifier_name) {

	int evaluation_end = evaluation_start + evaluation_items;
	int numberToTrain = total_items - evaluation_items; //number of training data items
	const int numberOfFeatures = 19; //number of features

	//allocate the arrays
	auto trainingLabels = new int[numberToTrain];
	auto evalLabels = new int[evaluation_items];
	auto trainingData = new float[numberToTrain][numberOfFeatures];
	auto evaluationData = new float[evaluation_items][numberOfFeatures];

	///Print current classifier stats
	cout << "\n";
	cout << " [M] dataset items: " << total_items << endl;
	//cout << " [M] evaluation items: " << evaluation_items << endl;
	//cout << " [M] start evaluation item: " << evaluation_start << endl;
	cout << " [M] features: " << numberOfFeatures << endl;

	/// Set up label data----------------------------------------------------
	ifstream inputBinaryData(binary_scores);
	std::string current_binary_line;


	int x = 0;
	int t = 0;
	int e = 0;
	while (getline(inputBinaryData, current_binary_line)) {

		int temp = atoi(current_binary_line.c_str());

		if (x < evaluation_start || x >= evaluation_end) {
			trainingLabels[t] = temp;
			t++;
		}
		else {
			evalLabels[e] = temp;
			e++;
		}
		x++;

	}

	/// Set up feature data------------------------------------------------------
	ifstream inputFeatureData(feature_vector);
	string current_training_line;
	int y = 0;
	int tf = 0;
	int ef = 0;
	bool training;

	// Start reading lines as long as there are lines in the file
	while (getline(inputFeatureData, current_training_line)) {
		training = (y < evaluation_start || y >= evaluation_end);

		stringstream temp(current_training_line);
		std::string single_value;
		int z = 0;
		while (getline(temp, single_value, ',')) {

			float tempF = atof(single_value.c_str()); // convert the string element to a float value

			if (training) {

				trainingData[tf][z] = tempF;

			}
			else {

				evaluationData[ef][z] = tempF;

			}
			z++;
		}
		if (training) tf++;
		else ef++;
		y++;

	}

	///Setup the input matrixes for training the svm----------------------------------
	Mat trainingDataMat(numberToTrain, numberOfFeatures, CV_32FC1, trainingData);
	Mat labelsMat(numberToTrain, 1, CV_32SC1, trainingLabels);
	cout << " [M] " << trainingDataMat.size() << " feature vector" << endl;
	cout << " [M] " << labelsMat.size() << " labels vector" << endl;

	/// Create the SVM
	double c = 0.0000001; //just to initialize
	double g = 0.0000001; //just to initialize

	// Set up SVM's parameters
	Ptr<SVM> s = SVM::create();
	s->setType(SVM::C_SVC);
	s->setKernel(SVM::RBF); //(SVM::CHI2, SVM::INTER, SVM::RBF).
	s->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 10000, 1e-6));

	// Pointer
	Ptr<TrainData> td = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);

	// Train the SVM with optimal parameters the second argument is K-fold for cross-validation

	s->trainAuto(td, 10);

	td.release();
	c = s->getC();
	g = s->getGamma();
	//svm->save("svm_classifier.xml"); //save classifier
	// svm->load("svm_2features/classifier_2feature.xml"); //load classifier

	int linhas = 0;
	int ones = 0;
	int zeros = 0;
	int acertos = 0;
	int tp = 0;
	int tn = 0;
	int fn = 0;
	int fp = 0;

	// Start reading lines as long as there are lines in the file

	while (linhas < evaluation_items) {

		Mat sampleDataMat(1, numberOfFeatures, CV_32FC1, evaluationData[linhas]);
		//cout<< sampleDataMat<<endl;

		///Show the decision given by the SVM

		float response = s->predict(sampleDataMat);

		// cout<<"sample tested:"<<endl;
		// cout<<sampleDataMat<<endl;
		// cout<<"SVM classification: "<<response<<" test case: "<<labels50[linhas]<<endl;

		if (response == evalLabels[linhas]) {
			acertos++;
			if (evalLabels[linhas] == 1) {
				tp++;
				ones++;
			}
			else {
				zeros++;
				tn++;
			}

		}
		else {

			if (evalLabels[linhas] == 1) {
				fn++;
				ones++;
			}
			else {
				zeros++;
				fp++;
			}

		}

		linhas++;
	}
	/*
	float acertoF = ((float)acertos / linhas) * 100;
	float precision = (float)tp / (tp + fp);

	if (isnan(precision)) precision = 0.0;
	float recall = (float)tp / (fn + tp);
	//float fnr = (float)fn/(fn+tp);
	float fpr = (float)fp / (tn + fp); //fall-out


	cout << " [M] " << "A: " << acertoF << "%" << " " << c << " " << g << " P: " << precision << " R: " << recall
		<< " fpr " << fpr
		<< " O: " << ones << endl;
	*/

	///For outputing CSV file with additional evaluation data
	/*
	if (binout) {
		float finalEvaluationData[final_evaluation_items][numberOfFeatures];

		/// Set up feature data
		ifstream inputFeatureData2(final_evaluation_vector);
		string current_training_line2;
		int y2 = 0;

		// Start reading lines as long as there are lines in the file
		while (getline(inputFeatureData2, current_training_line2)) {

			stringstream temp(current_training_line2);
			string single_value;
			int z = 0;
			while (getline(temp, single_value, ',')) {
				float temp = atof(single_value.c_str()); // convert the string element to a float value
				finalEvaluationData[y2][z] = temp;

				z++;
			}

			y2++;
		}

		ofstream evalBinData(outputPath.c_str());
		int evalLinhas = 0;
		int ones = 0;

		while (evalLinhas < final_evaluation_items) {

			Mat sampleDataMat(1, numberOfFeatures, CV_32FC1, finalEvaluationData[evalLinhas]);

			///Show the decision given by the SVM

			float response = s->predict(sampleDataMat);

			evalBinData << response << "\n";

			if (response == 1) ones++;

			evalLinhas++;

		}

		// cout <<"ones " << ones << " total " <<evalLinhas <<endl;
		evalBinData.close();
		cout << "final binary data saved at file: " << outputPath << endl;

	}
	*/
	cout << " [M] " << classifier_name << " Classifier generated!" << endl;
	return s;

}

//Predict sample
int mlclass::predictSample(json jSample, int nFeatures) {

	float* sampleData = new float[nFeatures];
	//float sampleData[nFeatures];
	sampleData[0] = jSample["red_ratio"];
	sampleData[1] = jSample["green_ratio"];
	sampleData[2] = jSample["blue_ratio"];
	sampleData[3] = jSample["focus"];
	sampleData[4] = jSample["luminance"];
	sampleData[5] = jSample["luminance_std"];
	sampleData[6] = jSample["dif_hues"];
	sampleData[7] = jSample["faces"];
	sampleData[8] = jSample["faces_area"];
	sampleData[9] = jSample["smiles"];
	sampleData[10] = jSample["rule_of_thirds"];
	sampleData[11] = jSample["static_saliency"];
	sampleData[12] = jSample["shackiness"];
	sampleData[13] = jSample["motion_mag"];
	sampleData[14] = jSample["fg_area"];
	sampleData[15] = jSample["shadow_area"];
	sampleData[16] = jSample["bg_area"];
	sampleData[17] = jSample["camera_move"];
	sampleData[18] = jSample["focus_diff"];

	Mat sampleDataMat(1, nFeatures, CV_32FC1, sampleData);

	///Show the decision given by the SVM

	int finalResponse = 0;
	int aesthetic_response = (int)svm_a->predict(sampleDataMat);
	int interest_response = (int)svm_i->predict(sampleDataMat);

	cout << "\n [M] aesthetics: " << aesthetic_response << " interestingness: " << interest_response << endl;

	if (aesthetic_response >= 1)
		finalResponse += 3;
	if (interest_response >= 1)
		finalResponse += 4;

	delete[] sampleData;
	return finalResponse;

}
//Method for testing random samples
int mlclass::predictTestSample(int nFeatures) {


	float* sampleData = new float[nFeatures];
	// float sampleData[nFeatures];
	sampleData[0] = 0.585126;
	sampleData[1] = 0.281233;
	sampleData[2] = 0.133641;
	sampleData[3] = 1;
	sampleData[4] = 0.110374;
	sampleData[5] = 0.0991967;
	sampleData[6] = 0.0226191;
	sampleData[7] = 0.0113379;
	sampleData[8] = 0.000804487;
	sampleData[9] = 0;
	sampleData[10] = 0.00166289;
	sampleData[11] = 0.0702186;
	sampleData[12] = 0.183673;
	sampleData[13] = 0.0869409;
	sampleData[14] = 0.356178;
	sampleData[15] = 0.000427326;
	sampleData[16] = 0.641127;
	sampleData[17] = 0.104308;
	sampleData[18] = 0.0380477;

	Mat sampleDataMat(1, nFeatures, CV_32FC1, sampleData);

	///Show the decision given by the SVM

	int finalResponse = 0;
	int aesthetic_response = (int)svm_a->predict(sampleDataMat);
	int interest_response = (int)svm_i->predict(sampleDataMat);

	cout << " [M] aesthetic test: " << aesthetic_response << " interest test: " << interest_response << endl;

	if ((aesthetic_response + interest_response) >= 1)
		finalResponse = 1;

	delete[] sampleData;
	return finalResponse;

}