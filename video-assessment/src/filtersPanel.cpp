#include "filtersPanel.h"
#include "ofxDatGui.h"


using namespace std;


void printRankedNames(vector<VideoFile> rankedFiles, string name = "") {
	cout << "\n///////" << name << ": " << rankedFiles.size() << "\\\\\\\\\\\\" << endl;
	for (File f : rankedFiles) {
		cout << f.name << endl;
	}
}

filtersPanel::filtersPanel()
{

}

filtersPanel::~filtersPanel()
{

}

void filtersPanel::draw()
{

}

void filtersPanel::filter(VideoFile files[], int length, int choosenFileIndex)
{
	float colorSimilarity = 0.0, edgeSimilarity = 0.0, entropySimilarity = 0.0, motionSimilarity = 0.0;
	int nsim = 0;
	float  red1 = 0.0, green1 = 0.0, blue1 = 0.0, red2 = 0.0, green2 = 0.0, blue2 = 0.0;
	int eh1 = 0, eh2 = 0, eh3 = 0, eh4 = 0, eh5 = 0, eh6, eh7 = 0, eh8 = 0, eh9 = 0, eh10 = 0,
		eh11 = 0, eh12 = 0, eh13 = 0, eh14 = 0, eh15 = 0, eh16 = 0, ehGlobal = 0;
	double entropy, motion = 0.0;
	float f1_average_loudness = 0.0, f2_dynamic_complexity = 0.0, f3_bpm = 0.0, f4_danceability = 0.0,
		f5_onset_rate = 0.0, f6_chords_change_rate = 0.0, f7_chords_number_rate = 0.0, f8_key_strength = 0.0,
		f9_tuning_diatonic_strength = 0.0, f10_tuning_equal_tempered_deviation = 0.0,
		f11_tuning_nontempered_energy_ratio = 0.0;

	if (saveXML) {
	
		saveToXML("buttons.xml");
		cout << "Saved to XML" << endl;
	}
	if (loadXML) {
		loadFromXML("buttons.xml");
		cout << "Loaded from XML" << endl;
	}

	if (lockSemanticInsert) {
		userInsertKeywords = ofSystemTextBoxDialog("Insert Keywords:", userInsertKeywords);
		cout << "Inserted keywords: " << userInsertKeywords << endl;
	}

	if (lockSemanticRemove) {
		userRemoveKeywords = ofSystemTextBoxDialog("Remove Keywords:", userRemoveKeywords);
		cout << "Removed keywords: " << userRemoveKeywords << endl;
	}

	if (lockTargetVideo)     //if similarity button is clicked on modify menu
	{

		if (choosenFileIndex < 0) { cout << "no current selection!" << endl; }
		else {
			if (colorSimilarityON)
			{
				nsim++;
				red1 = files[choosenFileIndex].redMoments.first;		//red color value of file
				green1 = files[choosenFileIndex].greenMoments.first;	//green color value of file
				blue1 = files[choosenFileIndex].blueMoments.first;		//blue color value of file
				red2 = files[choosenFileIndex].redMoments.second;		//red color value of file
				green2 = files[choosenFileIndex].greenMoments.second;	//green color value of file
				blue2 = files[choosenFileIndex].blueMoments.second;		//blue color value of file		
			}
			if (edgeSimilarityON)
			{
				nsim++;
				eh1 = files[choosenFileIndex].eh1;
				eh2 = files[choosenFileIndex].eh2;
				eh3 = files[choosenFileIndex].eh3;
				eh4 = files[choosenFileIndex].eh4;
				eh5 = files[choosenFileIndex].eh5;
				eh6 = files[choosenFileIndex].eh6;
				eh7 = files[choosenFileIndex].eh7;
				eh8 = files[choosenFileIndex].eh8;
				eh9 = files[choosenFileIndex].eh9;
				eh10 = files[choosenFileIndex].eh10;
				eh11 = files[choosenFileIndex].eh11;
				eh12 = files[choosenFileIndex].eh12;
				eh13 = files[choosenFileIndex].eh13;
				eh14 = files[choosenFileIndex].eh14;
				eh15 = files[choosenFileIndex].eh15;
				eh16 = files[choosenFileIndex].eh16;
				ehGlobal = files[choosenFileIndex].ehGlobal;
			}
			if (entropySimilarityON)
			{
				nsim++;
				entropy = files[choosenFileIndex].entropy;
			}
			if (motionSimilarityON)
			{
				nsim++;
				motion = files[choosenFileIndex].motion;
			}

			for (int i = 0; i < length; ++i) {

				if (colorSimilarityON)
				{

					colorSimilarity = sqrt(		        //Calculate color similarity 
						pow(red1 - files[i].redMoments.first, 2) +
						pow(green1 - files[i].greenMoments.first, 2) +
						pow(blue1 - files[i].blueMoments.first, 2) +
						pow(red2 - files[i].redMoments.second, 2) +
						pow(green2 - files[i].greenMoments.second, 2) +
						pow(blue2 - files[i].blueMoments.second, 2));

					colorSimilarity = (10.0 - colorSimilarity) / 10.0;
					//cout << red1<<" " << files[i].redMoments.first << endl;
					//cout << colorSimilarity << endl;
				}

				if (edgeSimilarityON)
				{
					int equality = 0;

					if (eh1 == files[i].eh1) equality++;
					if (eh2 == files[i].eh2) equality++;
					if (eh3 == files[i].eh3) equality++;
					if (eh4 == files[i].eh4) equality++;
					if (eh5 == files[i].eh5) equality++;
					if (eh6 == files[i].eh6) equality++;
					if (eh7 == files[i].eh7) equality++;
					if (eh8 == files[i].eh8) equality++;
					if (eh9 == files[i].eh9) equality++;
					if (eh10 == files[i].eh10) equality++;
					if (eh11 == files[i].eh11) equality++;
					if (eh12 == files[i].eh12) equality++;
					if (eh13 == files[i].eh13) equality++;
					if (eh14 == files[i].eh14) equality++;
					if (eh15 == files[i].eh15) equality++;
					if (eh16 == files[i].eh16) equality++;

					if (ehGlobal == files[i].ehGlobal) equality += 8;

					edgeSimilarity = (double)equality / 24;
					//cout << "same" << edgeSimilarity;

				}
				if (entropySimilarityON)
				{
					entropySimilarity = 1 - sqrt(		  //Calculate color similarity 
						pow(entropy - files[i].entropy, 2));
				}
				if (motionSimilarityON)
				{
					motionSimilarity = 1 - sqrt(		   //Calculate color similarity 
						pow(motion - files[i].motion, 2));
				}

				files[i].indexUpdate(((colorSimilarity + edgeSimilarity + entropySimilarity +
					motionSimilarity) / nsim), files[choosenFileIndex].name);
				//cout << entropySimilarity <<" " << motionSimilarity << endl;
			}	//end of for cycle  					

		}

	}

	if (f_ON || fadv_ON)
	{

		for (int i = 0; i < length; ++i) {				//For each file take values 
			double red = files[i].redMoments.first;		//red color value of file
			double green = files[i].greenMoments.first;	//green color value of file
			double blue = files[i].blueMoments.first;		//blue color value of file
			int rate = files[i].rate;					//rate of file

			double redRatio = files[i].redRatio;
			double greenRatio = files[i].greenRatio;
			double blueRatio = files[i].blueRatio;
			double entropy = files[i].entropy;
			double luminance = files[i].luminance;
			double sharpness = files[i].sharpness;
			double abruptness = files[i].abruptness;
			double motion = files[i].motion;

			double rule3 = files[i].rule3;

			double luminance_std = files[i].luminance_std;
			double dif_hues = files[i].dif_hues;
			double static_saliency = files[i].static_saliency;
			double ranksum = files[i].ranksum;
			double shadow = files[i].shadow;

			double avgFaces = files[i].avgFaces;
			double faceArea = files[i].faceArea;
			double smiles = files[i].smiles;
			double fgArea = files[i].faceArea;
			double focus_dif = files[i].focus_dif;
			double shake = files[i].shake;

			double average_loudness = files[i].a1_average_loudness;
			double dynamic_complexity = files[i].a2_dynamic_complexity;
			double bpm = files[i].a3_bpm;
			double danceability = files[i].a4_danceability;
			double onset_rate = files[i].a5_onset_rate;
			double chords_change_rate = files[i].a6_chords_change_rate;
			double colourfulness_mean = files[i].cf1;
			double colourfulness_std = files[i].cf2;


			if ((redRatio >= f_redRatioP) &&		  
				(greenRatio >= f_greenRatioP) &&	    
				(blueRatio >= f_blueRatioP) &&	     
				(rate >= f_rateP) &&	            
				(entropy >= f_entropy) &&			    
				(luminance >= f_luminaceP) &&      
				(sharpness >= f_sharpness) &&           
				(abruptness < f_abruptness) &&	       
				(motion >= f_motion) &&	                
				(luminance_std >= f_luminance_std) &&    
				(dif_hues >= f_dif_hues) &&	           
				(static_saliency >= f_static_saliency) && 
				(colourfulness_mean >= f_cf1) &&
				(colourfulness_std >= f_cf2) &&
				(ranksum >= f_ranksum) &&	           
				(shadow >= f_shadow) &&	                
				(avgFaces >= f_avgFaces) &&	             
				(faceArea >= f_faceArea) &&	             
				(smiles >= f_smiles) &&	                
				(fgArea >= f_fgArea) &&	                
				(focus_dif >= f_focus_dif) &&
				(shake < f_shake) &&
				(rule3 >= f_rule3) &&
				(average_loudness >= fa1_average_loudness) &&
				(dynamic_complexity >= fa2_dynamic_complexity) &&
				(bpm >= fa3_bpm) &&
				(danceability >= fa4_danceability) &&
				(onset_rate >= fa5_onset_rate) &&
				(chords_change_rate >= fa6_chords_change_rate))

			{
				files[i].setVisible(true);
				//cout << "filtering!"<<endl;
			}
			else
			{
				files[i].setVisible(false);
			}

			//////Human face filtering////////
			if (f_humanFace)								   //If human face filtering is set
			{
				if (files[i].getVisible()) {				   //And file was visible
					files[i].setVisible(files[i].humanFace);   //Set visible if there is human face
				}
			}
			//////Aesthetic filtering////////
			if (f_predict)
			{
				if (files[i].getVisible()) {
					files[i].setVisible(files[i].predict);
				}
			}
			//////Interestingness filtering////////
			if (f_interest)								      //If max interest filtering is set
			{
				if (files[i].getVisible()) {				  //And file was visible
					files[i].setVisible(files[i].interest_1); //Set visible if there is interest
				}
			}

			//////Keyword filtering////////
			if (f_semantic)
			{
				if (files[i].getVisible()) {

					files[i].setVisible(haveKey(files[i], userInsertKeywords, unionIntersect) && haveNotKey(files[i], userRemoveKeywords));
				}
			}

			if (moreBP_gclear)
			{
				files[i].rate = 0.0;				
			}

		    if (moreBP_g1)
			{

				if (files[i].getVisible()) {
					files[i].setVisible(files[i].rate == 1);
				}
			}
			if (moreBP_g2)
			{

				if (files[i].getVisible()) {
					files[i].setVisible(files[i].rate == 2);
				}
			}
			 if (moreBP_g3)
			{

				if (files[i].getVisible()) {
					files[i].setVisible(files[i].rate == 3);
				}
			}				

			if (moreBP_v)
			{
				if (files[i].getVisible()) {
					files[i].setVisible(files[i].ehGlobal == 1);
				}

			}
			else if (moreBP_h)
			{
				if (files[i].getVisible()) {
					files[i].setVisible(files[i].ehGlobal == 2);
				}

			}
			else if (moreBP_45)
			{
				if (files[i].getVisible()) {
					files[i].setVisible(files[i].ehGlobal == 3);
				}

			}
			else if (moreBP_135)
			{
				if (files[i].getVisible()) {
					files[i].setVisible(files[i].ehGlobal == 4);
				}

			}

			if (s_ON && files[i].getVisible())			//Add all visible files to ranking
			{
				sortVector.push_back(files[i]);
				numberOfSortedFiles = sortVector.size() < NUMBER_OF_RANKED_FILES ? sortVector.size() : NUMBER_OF_RANKED_FILES;
			}
		}
	}
	else												//No filters
	{
		for (int i = 0; i < length; ++i) { 				//All files visible
			files[i].setVisible(true);
		}
	}

	//Rank files

	if (s_ON) {
		ranking(sortVector);
		hideUnrankedFiles(sortVector, files, length);
	}

	if (resetFilterValues) {

		f_redRatioP = 0;
		f_greenRatioP = 0;
		f_blueRatioP = 0;
		f_entropy = 0;
		f_luminaceP = 0;
		f_sharpness = 0;
		f_rateP = 0;
		f_abruptness = 1;
		f_motion = 0;
		f_shake = 1;
		f_humanFace = false;
		f_rule3 = 0;
		f_avgFaces = 0;
		f_faceArea = 0;
		f_smiles = 0;
		f_fgArea = 0;
		f_focus_dif = 0;
		f_luminance_std = 0;
		f_dif_hues = 0;
		f_static_saliency = 0;

		f_cf1 = 0;
		f_cf2 = 0;

		f_ranksum = 0;
		f_shadow = 0;
		f_predict = false;
		f_interest = false;
		f_semantic = false;
		f_audio = false;
		cout << "Values reseted!" << endl;
		resetFilterValues = false;
		s_ON = false;

		fa1_average_loudness = 0;
		fa2_dynamic_complexity = 0,
		fa3_bpm = 0,
		fa4_danceability = 0,
		fa5_onset_rate = 0,
		fa6_chords_change_rate = 0;

		moreBP_v = false;
		moreBP_h = false;
		moreBP_45 = false;
		moreBP_135 = false;

		moreBP_g1 = false;
		moreBP_g2 = false;
		moreBP_g3 = false;
		moreBP_gclear = false;

		for (int i = 0; i < length; ++i) { 				//All files visible
			files[i].setVisible(true);
		}
	}
	sortVector.clear();
}

void filtersPanel::ranking(vector<VideoFile> &files)
{
	size_t length = files.size();
	if (length > 0) {

		sortFiles(files);

	}
}

void filtersPanel::setup()
{

	f_redRatioP = 0;
	f_greenRatioP = 0;
	f_blueRatioP = 0;
	f_entropy = 0;
	f_luminaceP = 0;
	f_sharpness = 0;
	f_rateP = 0;
	f_abruptness = 1;
	f_motion = 0;
	f_shake = 1;

	f_rule3 = 0;
	f_avgFaces = 0;
	f_faceArea = 0;
	f_smiles = 0;
	f_fgArea = 0;
	f_focus_dif = 0;
	f_luminance_std = 0;
	f_dif_hues = 0;
	f_static_saliency = 0;

	f_cf1 = 0;
	f_cf2 = 0;

	f_ranksum = 0;
	f_shadow = 0;
	f_humanFace = false;
	f_predict = false;
	f_interest = false;
	f_semantic = false;
	f_audio = false;

	moreBP_v = false;
	moreBP_h = false;
	moreBP_45 = false;
	moreBP_135 = false;

	moreBP_g1 = false;
	moreBP_g2 = false;
	moreBP_g3 = false;
	moreBP_gclear = false;

	resetFilterValues = false;
	s_ON = false;

	lockTargetVideo = false;
	colorSimilarityON = false;
	edgeSimilarityON = false;
	entropySimilarityON = false;
	motionSimilarityON = false;

	userInsertKeywords = "";
	userRemoveKeywords = "";
	lockSemanticInsert = false;
	lockSemanticRemove = false;
	unionIntersect = false;

	saveXML = false;
	loadXML = false;

	f_abruptness = 1;
	f_shake = 0.5;

	fa1_average_loudness = 0;
	fa2_dynamic_complexity = 0,
	fa3_bpm = 0,
	fa4_danceability = 0,
	fa5_onset_rate = 0,
	fa6_chords_change_rate = 0;

	f_ON = true;
	fadv_ON = true;

	ofSetVerticalSync(true);

	buttons.setup(); // this sets up the events etc..

	//File groups
	groupBP = buttons.addButtonPanel("FILE GROUPS");
	//groupBP->addListItem("--- Add to group ---  ");
	groupBP->addSelectionItem("------ 0 -------", file_group, GROUP_0);
	groupBP->addSelectionItem("------ 1 -------", file_group, GROUP_1);
	groupBP->addSelectionItem("------ 2 -------", file_group, GROUP_2);
	groupBP->addSelectionItem("------ 3 ------- ", file_group, GROUP_3);
	groupBP->addListItem("--------------------");
	groupBP->addFlashItem("Clear groups     ", moreBP_gclear);

	//Simple filters Panel
	simpleFilterBP = buttons.addButtonPanel("SIMPLE FILTERS");
	simpleFilterBP->addFlashItem("Clear values", resetFilterValues);
	simpleFilterBP->addSliderItem("Obj. index      ", 0, 0.75, f_ranksum);
	simpleFilterBP->addListItem("------- Groups ------");
	simpleFilterBP->addToggleItem("Group 1", moreBP_g1);
	simpleFilterBP->addToggleItem("Group_2", moreBP_g2);
	simpleFilterBP->addToggleItem("Group 3", moreBP_g3);
	
	simpleFilterBP->addListItem("------ Colour -------");
	simpleFilterBP->addSliderItem("Red             ", 0, 0.5, f_redRatioP);
	simpleFilterBP->addSliderItem("Green           ", 0, 0.5, f_greenRatioP);
	simpleFilterBP->addSliderItem("Blue            ", 0, 0.5, f_blueRatioP);
	simpleFilterBP->addListItem("---- Orientation ----");
	simpleFilterBP->addToggleItem("Vertical", moreBP_v);
	simpleFilterBP->addToggleItem("Horizontal", moreBP_h);
	simpleFilterBP->addToggleItem("45 degrees", moreBP_45);
	simpleFilterBP->addToggleItem("135 degrees", moreBP_135);
	simpleFilterBP->addSliderItem("Entropy         ", 0, 0.75, f_entropy);
	simpleFilterBP->addSliderItem("Luminance       ", 0, 0.75, f_luminaceP);
	simpleFilterBP->addSliderItem("Sharpness       ", 0, 1, f_sharpness);
	simpleFilterBP->addSliderItem("Hue count       ", 0, 0.5, f_dif_hues);
	simpleFilterBP->addSliderItem("Saliency        ", 0, 0.5, f_static_saliency);

	simpleFilterBP->addSliderItem("CF mean         ", 0, 1, f_cf1);
	simpleFilterBP->addSliderItem("CF std.         ", 0, 1, f_cf2);

	simpleFilterBP->addToggleItem("Human face", f_humanFace);
	simpleFilterBP->addSliderItem("Avg faces       ", 0, 1, f_avgFaces);
	simpleFilterBP->addSliderItem("Faces area      ", 0, 0.5, f_faceArea);
	simpleFilterBP->addSliderItem("Rule of thirds  ", 0, 0.5, f_rule3);
	simpleFilterBP->addSliderItem("Smiles          ", 0, 0.5, f_smiles);


	//advanced filter
	advanceFilterBP = buttons.addButtonPanel("ADVANCED FILTERS");
	advanceFilterBP->addFlashItem("Clear values", resetFilterValues);
	advanceFilterBP->addSliderItem("Lum. std        ", 0, 0.3, f_luminance_std);
	advanceFilterBP->addSliderItem("Foreg. area     ", 0, 0.3, f_fgArea);
	advanceFilterBP->addSliderItem("Shadow          ", 0, 0.4, f_shadow);
	advanceFilterBP->addSliderItem("Focus diff.     ", 0, 0.5, f_focus_dif);
	advanceFilterBP->addSliderItem("Motion          ", 0, 0.75, f_motion);
	advanceFilterBP->addSliderItem("Abruptness      ", 0, 1, f_abruptness);
	advanceFilterBP->addSliderItem("Shakiness       ", 0, 0.5, f_shake);
	advanceFilterBP->addListItem("-------- SVM --------");
	advanceFilterBP->addToggleItem("Aesthetics", f_predict);
	advanceFilterBP->addToggleItem("Interest", f_interest);
	advanceFilterBP->addListItem("----- Semantics -----");
	advanceFilterBP->addToggleItem("Keyword filter", f_semantic);
	advanceFilterBP->addToggleItem("Intersect", unionIntersect);
	advanceFilterBP->addListItem("------- Audio -------");
	advanceFilterBP->addSliderItem("Loudness        ", 0, 1, fa1_average_loudness);
	advanceFilterBP->addSliderItem("Complexity      ", 0, 1, fa2_dynamic_complexity);
	advanceFilterBP->addSliderItem("Bpm             ", 0, 1, fa3_bpm);
	advanceFilterBP->addSliderItem("Danceability    ", 0, 1, fa4_danceability);
	advanceFilterBP->addSliderItem("Onset           ", 0, 1, fa5_onset_rate);
	advanceFilterBP->addSliderItem("Chords change   ", 0, 1, fa6_chords_change_rate);

	//Sortpanel
	sortBP = buttons.addButtonPanel("FILE SORT");
	sortBP->addSelectionItem("File ID", sortType, SORT_30);
	sortBP->addSelectionItem("Similarity", sortType, SORT_21);
	sortBP->addSelectionItem("Group", sortType, SORT_0);
	sortBP->addSelectionItem("Obj. index", sortType, SORT_1);
	sortBP->addListItem("------- Visual ------");
	sortBP->addSelectionItem("Red", sortType, SORT_2);
	sortBP->addSelectionItem("Green", sortType, SORT_3);
	sortBP->addSelectionItem("Blue", sortType, SORT_4);
	sortBP->addSelectionItem("Entropy", sortType, SORT_5);
	sortBP->addSelectionItem("Luminance", sortType, SORT_6);
	sortBP->addSelectionItem("Luminance std", sortType, SORT_7);
	sortBP->addSelectionItem("Sharpness", sortType, SORT_8);
	sortBP->addSelectionItem("Hue count", sortType, SORT_9);
	sortBP->addSelectionItem("Saliency", sortType, SORT_10);
	sortBP->addSelectionItem("Colourfulness m", sortType, SORT_28);
	sortBP->addSelectionItem("Colourfulness s", sortType, SORT_29);
	sortBP->addSelectionItem("Avg faces", sortType, SORT_11);
	sortBP->addSelectionItem("Faces area", sortType, SORT_12);
	sortBP->addSelectionItem("Rule of Thirds      ", sortType, SORT_13);
	sortBP->addSelectionItem("Smiles", sortType, SORT_14);
	sortBP->addSelectionItem("Foreg. area", sortType, SORT_15);
	sortBP->addSelectionItem("Shadow", sortType, SORT_16);
	sortBP->addSelectionItem("Focus diff.", sortType, SORT_17);
	sortBP->addSelectionItem("Motion", sortType, SORT_18);
	sortBP->addSelectionItem("Abruptness", sortType, SORT_19);
	sortBP->addSelectionItem("Shakiness", sortType, SORT_20);
	sortBP->addListItem("------- Audio -------");
	sortBP->addSelectionItem("Loudness", sortType, SORT_22);
	sortBP->addSelectionItem("Complexity", sortType, SORT_23);
	sortBP->addSelectionItem("Bpm", sortType, SORT_24);
	sortBP->addSelectionItem("Danceability", sortType, SORT_25);
	sortBP->addSelectionItem("Onset", sortType, SORT_26);
	sortBP->addSelectionItem("Chords change", sortType, SORT_27);
	sortBP->addListItem("---------------------");
	
	sortBP->addFlashItem("APPLY SORT", s_ON);
	sortType = 30;

	//extended panel
	otherBP = buttons.addButtonPanel("EXTENDED");
	otherBP->addListItem("---- Similarity -----");
	otherBP->addFlashItem("Generate index", lockTargetVideo);
	otherBP->addToggleItem("Colour", colorSimilarityON);
	otherBP->addToggleItem("Orientation", edgeSimilarityON);
	otherBP->addToggleItem("Entropy", entropySimilarityON);
	otherBP->addToggleItem("Motion", motionSimilarityON);
	otherBP->addListItem("----- Semantics -----");
	otherBP->addFlashItem("Add keywords", lockSemanticInsert);
	otherBP->addFlashItem("Remove keywords", lockSemanticRemove);
	otherBP->addListItem("---------------------");
	otherBP->addFlashItem("Save to XML", saveXML);
	otherBP->addFlashItem("Load from XML", loadXML);

}
bool filtersPanel::isModifyClicked(int x, int y)
{
	if (otherBP->checkTitleClick(x, y) || otherBP->checkClick(x, y))		//If value or title clicked
	{
		otherBP->checkClick(x, y);	//Number of checkClick calls have to be odd, so toggle button change
		return true;
	}
	else
	{
		return false;
	}
}

bool filtersPanel::isSimpleFiltersClicked(int x, int y)
{
	if (simpleFilterBP->checkTitleClick(x, y) || simpleFilterBP->checkClick(x, y))		//If value or title clicked
	{
		simpleFilterBP->checkClick(x, y);	//Number of checkClick calls have to be odd, so toggle button change
		return true;
	}
	else
	{
		return false;
	}
}
bool filtersPanel::isAdvancedFiltersClicked(int x, int y)
{
	if (advanceFilterBP->checkTitleClick(x, y) || advanceFilterBP->checkClick(x, y))		//If value or title clicked
	{
		advanceFilterBP->checkClick(x, y);	//Number of checkClick calls have to be odd, so toggle button change
		return true;
	}
	else
	{
		return false;
	}
}

bool filtersPanel::isSortClicked(int x, int y)
{
	if (sortBP->checkTitleClick(x, y) || sortBP->checkClick(x, y))		//If value or title clicked
	{
		sortBP->checkClick(x, y);	//Number of checkClick calls have to be odd, so toggle button change
		return true;
	}
	else
	{
		return false;
	}
}

bool filtersPanel::isGroupClicked(int x, int y)
{
	if (groupBP->checkTitleClick(x, y) || groupBP->checkClick(x, y))		//If value or title clicked
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool filtersPanel::isGroupValueClicked(int x, int y)
{
	return groupBP->checkClick(x, y);
}

bool filtersPanel::isToolbarClicked(int x, int y)
{
	ofRectangle r = ofRectangle(0, 0, ofGetWidth(), buttons.toolbarh);		//Takes full toolBar 
	ofPoint mousePoint = ofPoint(x, y);
	return r.inside(mousePoint);
}

int filtersPanel::getGroup()
{
	return file_group;
}

bool filtersPanel::ifFiltersON()
{
	return f_ON;
}
bool filtersPanel::ifFiltersAdvON()
{
	return fadv_ON;
}

bool filtersPanel::isGroupON()
{
	return s_ON;
}

bool filtersPanel::ifClearGroupON()
{
	return moreBP_gclear;
}

void filtersPanel::sortFiles(vector<VideoFile> &files)
{
			switch (sortType)
			{
			case 0:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.rate > rhs.rate; });
				break;
			case 1:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.ranksum > rhs.ranksum; });
				break;
			case 2:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.redRatio > rhs.redRatio; });
				break;
			case 3:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.greenRatio > rhs.greenRatio; });
				break;
			case 4:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.blueRatio > rhs.blueRatio; });
				break;
			case 5:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.entropy > rhs.entropy; });
				break;
			case 6:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.luminance > rhs.luminance; });
				break;
			case 7:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.luminance_std > rhs.luminance_std; });
				break;
			case 8:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.sharpness > rhs.sharpness; });
				break;
			case 9:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.dif_hues > rhs.dif_hues; });
				break;
			case 10:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.static_saliency > rhs.static_saliency; });
				break;
			case 11:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.avgFaces > rhs.avgFaces; });
				break;
			case 12:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.faceArea > rhs.faceArea; });
				break;
			case 13:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.rule3 > rhs.rule3; });
				break;
			case 14:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.smiles > rhs.smiles; });
				break;
			case 15:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.fgArea > rhs.fgArea; });
				break;
			case 16:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.shadow > rhs.shadow; });
				break;
			case 17:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.focus_dif > rhs.focus_dif; });
				break;
			case 18:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.motion > rhs.motion; });
				break;
			case 19:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.abruptness > rhs.abruptness; });
				break;
			case 20:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.shake > rhs.shake; });
				break;
			case 21:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.similarityIndex > rhs.similarityIndex; });
				break;
			case 22:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.a1_average_loudness > rhs.a1_average_loudness; });
				break;
			case 23:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.a2_dynamic_complexity > rhs.a2_dynamic_complexity; });
				break;
			case 24:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.a3_bpm > rhs.a3_bpm; });
				break;
			case 25:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.a4_danceability > rhs.a4_danceability; });
				break;
			case 26:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.a5_onset_rate > rhs.a5_onset_rate; });
				break;
			case 27:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.a6_chords_change_rate > rhs.a6_chords_change_rate; });
				break;
			case 28:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.cf1 > rhs.cf1; });
				break;
			case 29:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.cf2 > rhs.cf2; });
				break;
			case 30:
				sort(files.begin(), files.end(), [](const VideoFile& lhs, const VideoFile& rhs)
				{ return lhs.fileID < rhs.fileID; });
				break;

			};

}

void filtersPanel::hideUnrankedFiles(vector<VideoFile> rankedFiles, VideoFile allFiles[], int allLength)
{
	//All files visibiliy is set to false. Now we can look only for ranked files and set them on true
	for (int i = 0; i < allLength; ++i) {
		allFiles[i].setVisible(false);
	}

	//Set each ranked file visible with order of rankedFiles
	for (int j = 0; j < rankedFiles.size(); ++j) {

		for (int i = 0; i < allLength; ++i) {
			if (rankedFiles[j].path == allFiles[i].path) {
				allFiles[i].setVisible(true);
				std::swap(allFiles[j], allFiles[i]);
				break;
			}
		}
	}
}

bool filtersPanel::haveKey(VideoFile file, string keywords, bool ui)
{
	bool retval = false;
	int numberOfKeys = 0;
	int detectedConcepts = 0;

	if (keywords == "") {
		retval = true;
	}
	else {
		vector<int> v = split(keywords, ',');
		numberOfKeys = v.size();

		if (!ui) {

			for (int i = 0; i < v.size(); i++)
			{
				if (file.semanticID_1 == v[i] || file.semanticID_2 == v[i] || file.semanticID_3 == v[i]
					|| file.semanticID_4 == v[i] || file.semanticID_5 == v[i])
				{
					retval = true;
				}
			}
		}
		else {

			for (int i = 0; i < v.size(); i++)
			{
				if (file.semanticID_1 == v[i]) { detectedConcepts++; }
				if (file.semanticID_2 == v[i]) { detectedConcepts++; }
				if (file.semanticID_3 == v[i]) { detectedConcepts++; }
				if (file.semanticID_4 == v[i]) { detectedConcepts++; }
				if (file.semanticID_5 == v[i]) { detectedConcepts++; }

				if (detectedConcepts == numberOfKeys)
				{
					retval = true;
				}
			}
		}
	}
	return retval;
}

bool filtersPanel::haveNotKey(VideoFile file, string keywords)
{
	bool retval = true;

	if (keywords == "") {
		retval = true;
	}
	else {
		vector<int> v = split(keywords, ',');

		for (int i = 0; i < v.size(); i++) {

			if (file.semanticID_1 == v[i] || file.semanticID_2 == v[i] || file.semanticID_3 == v[i]
				|| file.semanticID_4 == v[i] || file.semanticID_5 == v[i])
			{
				retval = false;
			}
		}
	}
	return retval;
}

vector<int> filtersPanel::split(const string &s, char delim) {
	vector<int> result;
	stringstream ss(s);
	string item;

	while (getline(ss, item, delim)) {

		result.push_back(stoi(item));
	}

	return result;
}

void filtersPanel::saveToXML(string xmlfilePath)
{
	//similarityIndex = (double)newIndex;		//Rate update

	ofXml xml; //xmlFile

	xml.addChild("TRESHOLDS");
	xml.setTo("//TRESHOLDS");

	xml.addValue("RED_RATIO", f_redRatioP);	
	xml.addValue("GREEN_RATIO", f_greenRatioP);
	xml.addValue("BLUE_RATIO", f_blueRatioP);
	xml.addValue("ENTROPY", f_entropy);
	xml.addValue("LUMINANCE", f_luminaceP);
	xml.addValue("SHARPNESS", f_sharpness);
	xml.addValue("ABRUPTNESS", f_abruptness);
	xml.addValue("MOTION", f_motion);

	xml.addValue("SHAKE", f_shake);
	xml.addValue("FACES", f_humanFace);
	xml.addValue("ROF3", f_rule3);
	xml.addValue("AVGFACES", f_avgFaces);
	xml.addValue("FACEAREA", f_faceArea);
	xml.addValue("SMILES", f_smiles);
	xml.addValue("FGAREA", f_fgArea);
	xml.addValue("FOCUSDIFF", f_focus_dif);
	xml.addValue("LUMSTD", f_luminance_std);
	xml.addValue("DIFFHUES", f_dif_hues);
	xml.addValue("SALIENCY", f_static_saliency);
	xml.addValue("CLRFM", f_cf1);
	xml.addValue("CLRFS", f_cf2);
	xml.addValue("OBJINDEX", f_ranksum);
	xml.addValue("SHADOWS", f_shadow);
	xml.addValue("AESTHETIC", f_predict);
	xml.addValue("INTEREST", f_interest);

	xml.addValue("LOUDNESS", fa1_average_loudness);
	xml.addValue("COMPLEXITY", fa2_dynamic_complexity);
	xml.addValue("BPM", fa3_bpm);
	xml.addValue("DANCEABILITY", fa4_danceability);
	xml.addValue("ONSET", fa5_onset_rate);
	xml.addValue("CCR", fa6_chords_change_rate);

	xml.addValue("VERTICAL", moreBP_v);
	xml.addValue("HORIZONTAL", moreBP_h);
	xml.addValue("RIGTH", moreBP_45);
	xml.addValue("LEFT", moreBP_135);

	if (!xml.save(xmlfilePath))
	{
		cout << " Error saving to xml: " << endl;
	}

}


void filtersPanel::loadFromXML(string xmlfilePath)
{
	ofXml* xml = new ofXml(xmlfilePath);
	if (xml->load(xmlfilePath)) {
		f_redRatioP = xml->getValue<double>("//RED_RATIO");
		f_greenRatioP = xml->getValue<double>("//GREEN_RATIO");
		f_blueRatioP = xml->getValue<double>("//BLUE_RATIO");
		f_entropy = xml->getValue<double>("//ENTROPY");
		f_luminaceP = xml->getValue<double>("//LUMINANCE");
		f_sharpness = xml->getValue<double>("//SHARPNESS");
		f_abruptness = xml->getValue<double>("//ABRUPTNESS");
		f_motion = xml->getValue<double>("//MOTION");

		f_shake = xml->getValue<double>("//SHAKE");
		f_humanFace = xml->getValue<bool>("//FACES");
		f_rule3 = xml->getValue<double>("//ROF3");
		f_avgFaces = xml->getValue<double>("//AVGFACES");
		f_faceArea = xml->getValue<double>("//FACEAREA");
		f_smiles = xml->getValue<double>("//SMILES");
		f_fgArea = xml->getValue<double>("//FGAREA");
		f_focus_dif = xml->getValue<double>("//FOCUSDIFF");
		f_luminance_std = xml->getValue<double>("//LUMSTD");
		f_dif_hues = xml->getValue<double>("//DIFFHUES");
		f_static_saliency = xml->getValue<double>("//SALIENCY");

		f_cf1 = xml->getValue<double>("//CLRFM");
		f_cf2 = xml->getValue<double>("//CLRFS");
		f_ranksum = xml->getValue<double>("//OBJINDEX");
		f_shadow = xml->getValue<double>("//SHADOWS");
		f_predict = xml->getValue<bool>("//AESTHETIC");
		f_interest = xml->getValue<bool>("//INTEREST");

		fa1_average_loudness = xml->getValue<double>("//LOUDNESS");
		fa2_dynamic_complexity = xml->getValue<double>("//COMPLEXITY");
		fa3_bpm = xml->getValue<double>("//BPM");
		fa4_danceability = xml->getValue<double>("//DANCEABILITY");
		fa5_onset_rate = xml->getValue<double>("//ONSET");
		fa6_chords_change_rate = xml->getValue<double>("//CCR");

		moreBP_v = xml->getValue<bool>("//VERTICAL");
		moreBP_h = xml->getValue<bool>("//HORIZONTAL");
		moreBP_45 = xml->getValue<bool>("//RIGTH");
		moreBP_135 = xml->getValue<bool>("//LEFT");

	}
	else { cout << "error" << endl; }

}

