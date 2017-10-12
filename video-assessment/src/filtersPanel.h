#pragma once
#include "File.h"
#include "VideoFile.h"
#include "ofxButtons.h"

#define NUMBER_OF_RANKED_FILES 1000

class filtersPanel
{
public:
	filtersPanel();
	~filtersPanel();

	void draw();							//Draw gui window
	void filter(VideoFile files[], int length,int choosenFileIndex);	//Filters array 
	void setup();					//Setup gui in coordinates. Length is for hashList

	bool isSimilarityClicked(int x, int y); //Check if similarity title or value was clicked
	int isSimilarityExtracted; //Check if similarity data was already extracted

	bool isFiltersClicked(int x, int y);	//Check if filters title or value clicked
	bool isFiltersMoreClicked(int x, int y);	//Check if filters title or value clicked
	bool isRankingClicked(int x, int y);	//Check if ranking title or value clicked
	bool isRateClicked(int x, int y);		//Check if rate title or value clicked
	bool isRateValueClicked(int x, int y);	//Check if rate clicked
	bool isToolbarClicked(int x, int y);	//Check if toolbar clicked

	int getRate();
	bool ifFiltersON();						//Getter of filtersFlag
	bool ifFiltersMoreON();						//Getter of filtersFlag
	bool isRankingON();						//Getter of rankingsFlag



private:
	ButtonManager buttons;		//Toolbar

	ButtonPanel * ugcRateBP;		//UGC Rate panel
	int ugcRateBP_choosenRate;			//Choosen rate
	enum {
		RATE_0,
		RATE_1,
		RATE_2,
		RATE_3,
		RATE_4,
		RATE_5
	};

	ButtonPanel * moreBP;		//more panel
	bool moreBP_ON;              //Flag if aply filters or not

	bool moreBP_v;
	bool moreBP_h;
	bool moreBP_45;
	bool moreBP_135;

	ButtonPanel * normalBP;		//Normal panel	
	bool normalBP_ON;              //Flag if aply filters or not

	float normalBP_redRatioP;		//Red ratio colour param
	float normalBP_greenRatioP;	//Green ratio colour param					
	float normalBP_blueRatioP;		//Blue ratio colour param
	float normalBP_luminaceP;		//Luminance param
	float normalBP_sharpness;        //Shapness param
	float normalBP_rateP;			//Rate param
	float normalBP_abruptness;			//Abruptness param
	float normalBP_motion;			//motion param
	float normalBP_entropy;			//motion param
	float normalBP_dif_hues;
	float normalBP_static_saliency;

	bool normalBP_humanFace;		//Flag if look for faces
	float normalBP_rule3;			//shake param
	float normalBP_avgFaces;		//Luminance param
	float normalBP_faceArea;        //Shapness param
	float normalBP_smiles;			//Rate param

	float normalBP_fgArea;			//Abruptness param
	float normalBP_focus_dif;			//motion param
	float normalBP_shake;			//shake param
	float normalBP_luminance_std;
	float normalBP_ranksum;
	float normalBP_shadow;

	bool normalBP_predict;
	bool normalBP_interest_1;

	ButtonPanel * sortBP;		//Sort panel

	bool sortBP_ON;				//Flag if ranking on

	int sortBP_sortType;			//Choosen rate
	enum {
		SORT_0,
		SORT_1,
		SORT_2,
		SORT_3,
		SORT_4,
		SORT_5,
		SORT_6,
		SORT_7,
		SORT_8,
		SORT_9,
		SORT_10,
		SORT_11,
	    SORT_12,
		SORT_13,
		SORT_14,
		SORT_15,
		SORT_16,
		SORT_17,
		SORT_18,
		SORT_19,
		SORT_20,
		SORT_21
	};

	bool resetFilterValues;				//reset values

	ButtonPanel * similarityBP;		//Similarity index panel
	bool lockTargetVideo;				//reset values
	bool colorSimilarityON;
	bool edgeSimilarityON;
	bool entropySimilarityON;
	bool motionSimilarityON;
	

	vector<VideoFile> sortVector;		//Vector of ranked files
	int numberOfSortedFiles;		//Number of ranked files

	void ranking(vector<VideoFile> &files);		//Rank array (sort)
	void sortFiles(vector<VideoFile> &files, int length);						//Sort 
	void hideUnrankedFiles(vector<VideoFile> rankedFiles, VideoFile allFiles[], int allLenght);	//Show only top ranked files
};

