#include "filtersPanel.h"


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

	if (lockSemanticInsert) {
		userInsertKeywords = ofSystemTextBoxDialog("Insert Keywords:", userInsertKeywords);
		cout << "Inserted keywords: " << userInsertKeywords << endl;
	}

	if (lockSemanticRemove) {
		userRemoveKeywords = ofSystemTextBoxDialog("Remove Keywords:", userRemoveKeywords);
		cout << "Removed keywords: " << userRemoveKeywords << endl;
	}

	if (lockTargetVideo)     //if similarity button is clicked on similarity menu
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


	if (normalBP_ON || moreBP_ON)
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


			if ((redRatio >= normalBP_redRatioP) &&		          //Red colour parameter
				(greenRatio >= normalBP_greenRatioP) &&	          //Green colour parameter
				(blueRatio >= normalBP_blueRatioP) &&	          //Blue colour parameter
				(rate >= normalBP_rateP) &&	                      //Blue colour parameter
				(entropy >= normalBP_entropy) &&			          //Simplicity parameter
				(luminance >= normalBP_luminaceP) &&               //Luminance parameter	
				(sharpness >= normalBP_sharpness) &&               //sharpness	
				(abruptness < normalBP_abruptness) &&	          //abruptness
				(motion >= normalBP_motion) &&	                  //motion
				(luminance_std >= normalBP_luminance_std) &&      //luma std
				(dif_hues >= normalBP_dif_hues) &&	              //hues
				(static_saliency >= normalBP_static_saliency) &&  //static saliency
				(ranksum >= normalBP_ranksum) &&	              //ranksum
				(shadow >= normalBP_shadow) &&	                  //shadow area
				(avgFaces >= normalBP_avgFaces) &&	              //ranksum
				(faceArea >= normalBP_faceArea) &&	              //area of faces
				(smiles >= normalBP_smiles) &&	                  //smiles in proper place of faces
				(fgArea >= normalBP_fgArea) &&	                  //foreground area
				(focus_dif >= normalBP_focus_dif) &&              //foreground area
				(shake < normalBP_shake) &&                       //foreground area
				(rule3 >= normalBP_rule3))                        //shakiness



			{
				files[i].setVisible(true);
				//cout << "filtering!"<<endl;
			}
			else
			{
				files[i].setVisible(false);
			}

			//////Human face filtering////////
			if (normalBP_humanFace)								//If human face filtering is set
			{
				if (files[i].getVisible()) {						//And file was visible
					files[i].setVisible(files[i].humanFace);			//Set visible if there is human face
				}
			}
			//////Aesthetic filtering////////
			if (normalBP_predict)
			{
				if (files[i].getVisible()) {
					files[i].setVisible(files[i].predict);
				}
			}
			//////Interestingness filtering////////
			if (normalBP_interest_1)								//If max interest filtering is set
			{
				if (files[i].getVisible()) {						//And file was visible
					files[i].setVisible(files[i].interest_1);			//Set visible if there is max interest
				}
			}

			//////Keyword filtering////////
			if (normalBP_semantic)
			{
				if (files[i].getVisible()) {

					files[i].setVisible(haveKey(files[i], userInsertKeywords, unionIntersect) && haveNotKey(files[i], userRemoveKeywords));
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

			if (sortBP_ON && files[i].getVisible())			//Add all visible files to ranking
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

	if (sortBP_ON) {
		ranking(sortVector);
		hideUnrankedFiles(sortVector, files, length);
	}


	if (resetFilterValues) {

		normalBP_redRatioP = 0;
		normalBP_greenRatioP = 0;
		normalBP_blueRatioP = 0;
		normalBP_entropy = 0;
		normalBP_luminaceP = 0;
		normalBP_sharpness = 0;
		normalBP_rateP = 0;
		normalBP_abruptness = 1;
		normalBP_motion = 0;
		normalBP_shake = 1;
		normalBP_humanFace = false;
		normalBP_rule3 = 0;
		normalBP_avgFaces = 0;
		normalBP_faceArea = 0;
		normalBP_smiles = 0;
		normalBP_fgArea = 0;
		normalBP_focus_dif = 0;
		normalBP_luminance_std = 0;
		normalBP_dif_hues = 0;
		normalBP_static_saliency = 0;
		normalBP_ranksum = 0;
		normalBP_shadow = 0;
		normalBP_predict = false;
		normalBP_interest_1 = false;
		normalBP_semantic = false;
		cout << "Values reseted!" << endl;
		resetFilterValues = false;
		sortBP_ON = false;

		moreBP_v = false;
		moreBP_h = false;
		moreBP_45 = false;
		moreBP_135 = false;

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

		sortFiles(files, numberOfSortedFiles);

	}
}

void filtersPanel::setup()
{

	normalBP_redRatioP = 0;
	normalBP_greenRatioP = 0;
	normalBP_blueRatioP = 0;
	normalBP_entropy = 0;
	normalBP_luminaceP = 0;
	normalBP_sharpness = 0;
	normalBP_rateP = 0;
	normalBP_abruptness = 1;
	normalBP_motion = 0;
	normalBP_shake = 1;

	normalBP_rule3 = 0;
	normalBP_avgFaces = 0;
	normalBP_faceArea = 0;
	normalBP_smiles = 0;
	normalBP_fgArea = 0;
	normalBP_focus_dif = 0;
	normalBP_luminance_std = 0;
	normalBP_dif_hues = 0;
	normalBP_static_saliency = 0;
	normalBP_ranksum = 0;
	normalBP_shadow = 0;
	normalBP_humanFace = false;
	normalBP_predict = false;
	normalBP_interest_1 = false;
	normalBP_semantic = false;

	moreBP_v = false;
	moreBP_h = false;
	moreBP_45 = false;
	moreBP_135 = false;

	resetFilterValues = false;
	sortBP_ON = false;

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

	normalBP_abruptness = 1;
	normalBP_shake = 0.5;

	normalBP_ON = true;
	moreBP_ON = true;

	ofSetVerticalSync(true);

	buttons.setup(); // this sets up the events etc..

	//File groups
	groupBP = buttons.addButtonPanel("FILE GROUPS");
	groupBP->addSelectionItem("      0             ", ugcRateBP_choosenRate, RATE_0);
	groupBP->addSelectionItem("      1             ", ugcRateBP_choosenRate, RATE_1);
	groupBP->addSelectionItem("      2             ", ugcRateBP_choosenRate, RATE_2);
	groupBP->addSelectionItem("      3             ", ugcRateBP_choosenRate, RATE_3);
	groupBP->addSelectionItem("      4             ", ugcRateBP_choosenRate, RATE_4);
	groupBP->addSelectionItem("      5             ", ugcRateBP_choosenRate, RATE_5);
	//ugcRateBP->addToggleItem("testing", moreBP_v);

	//Normal filters Panel
	simpleFilterBP = buttons.addButtonPanel("SIMPLE FILTERS");
	simpleFilterBP->addFlashItem("Clear values", resetFilterValues);
	simpleFilterBP->addSliderItem("Group           ", 0, 5, normalBP_rateP);
	simpleFilterBP->addSliderItem("Obj. index      ", 0, 0.5, normalBP_ranksum);
	simpleFilterBP->addListItem("Color:");
	simpleFilterBP->addSliderItem("Red             ", 0, 1, normalBP_redRatioP);
	simpleFilterBP->addSliderItem("Green           ", 0, 1, normalBP_greenRatioP);
	simpleFilterBP->addSliderItem("Blue            ", 0, 1, normalBP_blueRatioP);
	simpleFilterBP->addListItem("Orientation:");
	simpleFilterBP->addToggleItem("Vertical", moreBP_v);
	simpleFilterBP->addToggleItem("Horizontal", moreBP_h);
	simpleFilterBP->addToggleItem("45 degrees", moreBP_45);
	simpleFilterBP->addToggleItem("135 degrees", moreBP_135);
	simpleFilterBP->addSliderItem("Entropy         ", 0, 1, normalBP_entropy);
	simpleFilterBP->addSliderItem("Luminance       ", 0, 1, normalBP_luminaceP);
	simpleFilterBP->addSliderItem("Sharpness       ", 0, 1, normalBP_sharpness);
	simpleFilterBP->addSliderItem("Hue count       ", 0, 1, normalBP_dif_hues);
	simpleFilterBP->addSliderItem("Saliency        ", 0, 0.3, normalBP_static_saliency);
	simpleFilterBP->addToggleItem("Human face", normalBP_humanFace);
	simpleFilterBP->addSliderItem("Avg faces       ", 0, 1, normalBP_avgFaces);
	simpleFilterBP->addSliderItem("Faces area      ", 0, 0.4, normalBP_faceArea);
	simpleFilterBP->addSliderItem("Rule of thirds  ", 0, 0.3, normalBP_rule3);
	simpleFilterBP->addSliderItem("Smiles          ", 0, 0.5, normalBP_smiles);


	//filter more
	advanceFilterBP = buttons.addButtonPanel("ADVANCED FILTERS");
	advanceFilterBP->addFlashItem("Clear values", resetFilterValues);
	advanceFilterBP->addSliderItem("Lum. std        ", 0, 0.5, normalBP_luminance_std);
	advanceFilterBP->addSliderItem("Foreg. area     ", 0, 0.2, normalBP_fgArea);
	advanceFilterBP->addSliderItem("Shadow          ", 0, 0.3, normalBP_shadow);
	advanceFilterBP->addSliderItem("Focus diff.     ", 0, 1, normalBP_focus_dif);
	advanceFilterBP->addSliderItem("Motion          ", 0, 0.5, normalBP_motion);
	advanceFilterBP->addSliderItem("Abruptness      ", 0, 1, normalBP_abruptness);
	advanceFilterBP->addSliderItem("Shakiness       ", 0, 0.5, normalBP_shake);
	advanceFilterBP->addListItem("Classification:");
	advanceFilterBP->addToggleItem("Aesthetics", normalBP_predict);
	advanceFilterBP->addToggleItem("Interest", normalBP_interest_1);
	advanceFilterBP->addListItem("Semantic analysis:");
	advanceFilterBP->addToggleItem("Keyword filter", normalBP_semantic);
	advanceFilterBP->addToggleItem("Union-Intersect", unionIntersect);


	//Sortpanel
	sortBP = buttons.addButtonPanel("FILE SORT");
	sortBP->addSelectionItem("Group", sortBP_sortType, SORT_0);
	sortBP->addSelectionItem("Obj. index", sortBP_sortType, SORT_1);
	sortBP->addSelectionItem("Red", sortBP_sortType, SORT_2);
	sortBP->addSelectionItem("Green", sortBP_sortType, SORT_3);
	sortBP->addSelectionItem("Blue", sortBP_sortType, SORT_4);
	sortBP->addSelectionItem("Entropy", sortBP_sortType, SORT_5);
	sortBP->addSelectionItem("Luminance", sortBP_sortType, SORT_6);
	sortBP->addSelectionItem("Luminance std", sortBP_sortType, SORT_7);
	sortBP->addSelectionItem("Sharpness", sortBP_sortType, SORT_8);
	sortBP->addSelectionItem("Hue count", sortBP_sortType, SORT_9);
	sortBP->addSelectionItem("Saliency", sortBP_sortType, SORT_10);
	sortBP->addSelectionItem("Avg faces", sortBP_sortType, SORT_11);
	sortBP->addSelectionItem("Faces area", sortBP_sortType, SORT_12);
	sortBP->addSelectionItem("Rule of Thirds      ", sortBP_sortType, SORT_13);
	sortBP->addSelectionItem("Smiles", sortBP_sortType, SORT_14);
	sortBP->addSelectionItem("Foreg. area", sortBP_sortType, SORT_15);
	sortBP->addSelectionItem("Shadow", sortBP_sortType, SORT_16);
	sortBP->addSelectionItem("Focus diff.", sortBP_sortType, SORT_17);
	sortBP->addSelectionItem("Motion", sortBP_sortType, SORT_18);
	sortBP->addSelectionItem("Abruptness", sortBP_sortType, SORT_19);
	sortBP->addSelectionItem("Shakiness", sortBP_sortType, SORT_20);
	sortBP->addSelectionItem("Similarity", sortBP_sortType, SORT_21);
	sortBP->addListItem(" ");
	sortBP->addFlashItem("APPLY SORT", sortBP_ON);
	sortBP_sortType = 0;


	//indexing panel
	otherBP = buttons.addButtonPanel("MODIFY");
	otherBP->addListItem("Similarity:");
	otherBP->addFlashItem("Generate index", lockTargetVideo);
	otherBP->addToggleItem("COLOR", colorSimilarityON);
	otherBP->addToggleItem("ORIENTATION", edgeSimilarityON);
	otherBP->addToggleItem("ENTROPY", entropySimilarityON);
	otherBP->addToggleItem("MOTION", motionSimilarityON);
	otherBP->addListItem("Semantic analysis:");
	otherBP->addFlashItem("Add keywords", lockSemanticInsert);
	otherBP->addFlashItem("Remove keywords", lockSemanticRemove);



}
bool filtersPanel::isOtherClicked(int x, int y)
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

bool filtersPanel::isRateValueClicked(int x, int y)
{
	return groupBP->checkClick(x, y);
}

bool filtersPanel::isToolbarClicked(int x, int y)
{
	ofRectangle r = ofRectangle(0, 0, ofGetWidth(), buttons.toolbarh);		//Takes full toolBar 
	ofPoint mousePoint = ofPoint(x, y);
	return r.inside(mousePoint);
}

int filtersPanel::getRate()
{
	return ugcRateBP_choosenRate;
}

bool filtersPanel::ifFiltersON()
{
	return normalBP_ON;
}
bool filtersPanel::ifFiltersMoreON()
{
	return moreBP_ON;
}

bool filtersPanel::isRankingON()
{
	return sortBP_ON;
}

void filtersPanel::sortFiles(vector<VideoFile> &files, size_t length)
{

	if (length > 1) {

		size_t i, change;
		bool test = false;
		do {
			change = 0;
			i = length - 1;
			do {
				i--;

				switch (sortBP_sortType)
				{
				case 0:
					test = files[i].rate < files[i + 1].rate;
					break;
				case 1:
					test = files[i].ranksum < files[i + 1].ranksum;
					break;
				case 2:
					test = files[i].redRatio < files[i + 1].redRatio;
					break;
				case 3:
					test = files[i].greenRatio < files[i + 1].greenRatio;
					break;
				case 4:
					test = files[i].blueRatio < files[i + 1].blueRatio;
					break;
				case 5:
					test = files[i].entropy < files[i + 1].entropy;
					break;
				case 6:
					test = files[i].luminance < files[i + 1].luminance;
					break;
				case 7:
					test = files[i].luminance_std < files[i + 1].luminance_std;
					break;
				case 8:
					test = files[i].sharpness < files[i + 1].sharpness;
					break;
				case 9:
					test = files[i].dif_hues < files[i + 1].dif_hues;
					break;
				case 10:
					test = files[i].static_saliency < files[i + 1].static_saliency;
					break;
				case 11:
					test = files[i].avgFaces < files[i + 1].avgFaces;
					break;
				case 12:
					test = files[i].faceArea < files[i + 1].faceArea;
					break;
				case 13:
					test = files[i].rule3 < files[i + 1].rule3;
					break;
				case 14:
					test = files[i].smiles < files[i + 1].smiles;
					break;
				case 15:
					test = files[i].fgArea < files[i + 1].fgArea;
					break;
				case 16:
					test = files[i].shadow < files[i + 1].shadow;
					break;
				case 17:
					test = files[i].focus_dif < files[i + 1].focus_dif;
					break;
				case 18:
					test = files[i].motion < files[i + 1].motion;
					break;
				case 19:
					test = files[i].abruptness < files[i + 1].abruptness;
					break;
				case 20:
					test = files[i].shake < files[i + 1].shake;
					break;
				case 21:
					test = files[i].similarityIndex < files[i + 1].similarityIndex;
					break;

				};
				if (test) {
					std::swap(files[i], files[i + 1]);
					change = 1;
				}
			} while (i != 0);

		} while (change != 0);
	}
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