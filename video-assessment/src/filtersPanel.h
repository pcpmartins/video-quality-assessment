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

	void draw();							                            //Draw gui window
	void filter(VideoFile files[], int length, int choosenFileIndex);	//Filters array 
	void setup();
	//Check if similarity title or value was clicked
	bool isModifyClicked(int x, int y);
	//Check if similarity data was already extracted
	int isSimilarityExtracted;

	bool isSimpleFiltersClicked(int x, int y);	    //Check if filters title or value clicked
	bool isAdvancedFiltersClicked(int x, int y);	//Check if filters title or value clicked
	bool isSortClicked(int x, int y);     	//Check if ranking title or value clicked
	bool isGroupClicked(int x, int y);	    	//Check if rate title or value clicked
	bool isGroupValueClicked(int x, int y);   	//Check if rate clicked
	bool isToolbarClicked(int x, int y);	    //Check if toolbar clicked

	int getGroup();
	bool ifFiltersON();						    //Getter of filtersFlag
	bool ifFiltersAdvON();						//Getter of filtersFlag
	bool isGroupON();						    //Getter of rankingsFlag

private:
	ButtonManager buttons;		        //Toolbar

	ButtonPanel * groupBP;		    
	int file_group;			//file group
	enum {
		GROUP_0,
		GROUP_1,
		GROUP_2,
		GROUP_3
	};

	ButtonPanel * advanceFilterBP;		   //advanced panel
	bool fadv_ON;                //Flag if advanced filters are ON

	bool moreBP_v;
	bool moreBP_h;
	bool moreBP_45;
	bool moreBP_135;

	bool moreBP_g1;
	bool moreBP_g2;
	bool moreBP_g3;

	ButtonPanel * simpleFilterBP;		   //Normal panel	
	bool f_ON;              //Flag if simple filters are ON

	float f_redRatioP;		//Red ratio colour param
	float f_greenRatioP;  	//Green ratio colour param					
	float f_blueRatioP;		//Blue ratio colour param
	float f_luminaceP;		//Luminance param
	float f_sharpness;       //Shapness param
	float f_rateP;			//Rate param
	float f_abruptness;		//Abruptness param
	float f_motion;			//motion param
	float f_entropy;			//motion param
	float f_dif_hues;
	float f_static_saliency;

	bool f_humanFace;		//Flag if look for faces
	float f_rule3;			//shake param
	float f_avgFaces;		//Luminance param
	float f_faceArea;        //Shapness param
	float f_smiles;			//Rate param

	float f_fgArea;			//Abruptness param
	float f_focus_dif;		//motion param
	float f_shake;			//shake param
	float f_luminance_std;
	float f_ranksum;
	float f_shadow;

	bool f_predict;
	bool f_interest;
	bool f_semantic;
	bool f_audio;

	float fa1_average_loudness, fa2_dynamic_complexity, fa3_bpm, fa4_danceability, fa5_onset_rate,
	fa6_chords_change_rate, fa7_chords_number_rate, fa8_key_strength, fa9_tuning_diatonic_strength,
	fa10_tuning_equal_tempered_deviation, fa11_tuning_nontempered_energy_ratio;

	ButtonPanel * sortBP;		//Sort panel

	bool s_ON;				//Flag if ranking on

	int sortType;		//Choosen rate
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
		SORT_21,
		SORT_22,
		SORT_23,
		SORT_24,
		SORT_25,
		SORT_26,
		SORT_27
	};

	bool resetFilterValues;			//reset values

	ButtonPanel * otherBP;		//Similarity index panel
	bool lockTargetVideo;
	bool colorSimilarityON;
	bool edgeSimilarityON;
	bool entropySimilarityON;
	bool motionSimilarityON;

	string userInsertKeywords;
	bool lockSemanticInsert;
	string userRemoveKeywords;
	bool lockSemanticRemove;
	bool unionIntersect;


	bool haveKey(VideoFile file, string keywords, bool ui);
	bool haveNotKey(VideoFile file, string keywords);
	vector<int> split(const string &s, char delim);
	vector<VideoFile> sortVector;		//Vector of ranked files
	size_t numberOfSortedFiles;		//Number of ranked files

	void ranking(vector<VideoFile> &files);		//Rank array (sort)
	void sortFiles(vector<VideoFile> &files, size_t length);	//Sort 
	//Show only n top ranked files
	void hideUnrankedFiles(vector<VideoFile> rankedFiles, VideoFile allFiles[], int allLenght);
};

