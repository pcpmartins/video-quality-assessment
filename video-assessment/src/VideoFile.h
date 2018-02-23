#pragma once
#include "File.h"

class VideoFile :
	public File
{
public:
	VideoFile();
	VideoFile(const File *f);
	VideoFile(string name, string path);
	~VideoFile();

	ofVideoPlayer video;							//Image
	static string filesFolderPath;					//Path to files
	static string thumbnailFolderPath;				//Path to thumbnails
	static string thumbnailIconPath;				//Path to camera icon to add to thumbnail
	static vector <string> availableFormats;			//Required to distinguish images from videos and other files

	double abruptness;
	double motion;
	double shake;
	double fgArea;
	double focus_dif;
	double luminance_std;
	double ranksum;
	double shadow;
	unsigned short predict;
	int interest_1;

	int semanticID_1, semanticID_2, semanticID_3, semanticID_4, semanticID_5;
	double semanticValue_1, semanticValue_2, semanticValue_3, semanticValue_4, semanticValue_5;
	double a1_average_loudness, a2_dynamic_complexity, a3_bpm, a4_danceability, a5_onset_rate,
		a6_chords_change_rate, a7_chords_number_rate, a8_key_strength, a9_tuning_diatonic_strength,
		a10_tuning_equal_tempered_deviation, a11_tuning_nontempered_energy_ratio, mfcc01, mfcc02, 
		mfcc03, mfcc04, mfcc05, mfcc06, mfcc07, mfcc08, mfcc09, mfcc10, mfcc11, mfcc12, mfcc13, cf1, cf2;

	///////////////////Methods////////////////////////
public:

	//void show();					//Call when clicked. Show metadata 
	//void open();					//Call when double-clicked. OPen full screen?
	//virtual void load(string path) override;
	//virtual void draw() override;
	//string generateThumbnail() override;
	bool generateXmlFile() override;
	bool getMetadataFromXml()  override;
	bool getMetadataFromCsv(vector<string> csvData) override; // csv parse
	void getMetadataFromSemanticSample(vector< pair <double, int > > semanticSample); // csv parse semantic
	void getMetadataFromAudioSample(vector< double > audioSample); // csv parse audio

	void setThumbnailPath() override;
	bool loadThumbnail() override;
	void draw(ofRectangle space, bool play, ofTrueTypeFont *playerText);
	void draw(int x, int y, bool play);
	void stopVideo();

private:

};

