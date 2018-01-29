#include "VideoFile.h"
#include <chrono>


VideoFile::VideoFile()
{
	//cout << "VideoFile created" << endl;
}

VideoFile::VideoFile(const File *f)
{
	//copy values
	name = (*f).name;
	extension = (*f).extension;
	path = (*f).path;
	thumbnailPath = (*f).thumbnailPath;
	xmlPath = (*f).xmlPath;
	rate = (*f).rate;
	similarityIndex = (*f).similarityIndex;
	referenceName = (*f).referenceName;
	video.load(path);

	if (!File::getMetadataFromXml())
		cout << "[error: File.cpp] getMetadataFromXml()" << endl;
	getMetadataFromXml();
}

VideoFile::VideoFile(string name, string path)
{
	this->path = path;
	this->name = name;
	rate = 0;
	separateNameFromExtension();
	setThumbnailPath();
	setXmlPath();

	loadThumbnail();			
}

VideoFile::~VideoFile()
{
}

bool VideoFile::loadThumbnail()
{
	if (!thumbnail.load(thumbnailPath))
	{
		cout << "Thumbnail loading for " << name << " video error" << endl;
		return false;
	}
	return true;
}

bool VideoFile::generateXmlFile()
{
	File::generateXmlFile();
	ofXml xml;
	if (xml.load(xmlPath))
	{

		xml.setTo("//FILE");
		xml.addValue("ABRUPTNESS", abruptness);
		xml.addValue("MOTION", motion);
		xml.addValue("SHACKINESS", shake);
		xml.addValue("FG_AREA", fgArea);
		xml.addValue("FOCUS_DIFF", focus_dif);
		xml.addValue("LUMA_STD", luminance_std);
		xml.addValue("SHADOW", shadow);
		xml.addValue("RANKSUM", ranksum);
		xml.addValue("PREDICT", predict);
		xml.addValue("INTEREST_1", interest_1);
		xml.addValue("TYPE", "VIDEO");
		xml.addChild("SEMANTIC");								
		xml.setTo("SEMANTIC");
		xml.addValue("SID_1", semanticID_1);
		xml.addValue("SVALUE_1", semanticValue_1);
		xml.addValue("SID_2", semanticID_2);
		xml.addValue("SVALUE_2", semanticValue_2);
		xml.addValue("SID_3", semanticID_3);
		xml.addValue("SVALUE_3", semanticValue_3);
		xml.addValue("SID_4", semanticID_4);
		xml.addValue("SVALUE_4", semanticValue_4);
		xml.addValue("SID_5", semanticID_5);
		xml.addValue("SVALUE_5", semanticValue_5);
		xml.setTo("//FILE");
		xml.addChild("AUDIO");
		xml.setTo("AUDIO");
		xml.addValue("A1", a1_average_loudness);
		xml.addValue("A2", a2_dynamic_complexity);
		xml.addValue("A3", a3_bpm);
		xml.addValue("A4", a4_danceability);
		xml.addValue("A5", a5_onset_rate);
		xml.addValue("A6", a6_chords_change_rate);
		xml.addValue("A7", a7_chords_number_rate);
		xml.addValue("A8", a8_key_strength);
		xml.addValue("A9", a9_tuning_diatonic_strength);
		xml.addValue("A10", a10_tuning_equal_tempered_deviation);
		xml.addValue("A11", a11_tuning_nontempered_energy_ratio);
		xml.addValue("MFCC01", mfcc01);
		xml.addValue("MFCC02", mfcc02);
		xml.addValue("MFCC03", mfcc03);
		xml.addValue("MFCC04", mfcc04);
		xml.addValue("MFCC05", mfcc05);
		xml.addValue("MFCC06", mfcc06);
		xml.addValue("MFCC07", mfcc07);
		xml.addValue("MFCC08", mfcc08);
		xml.addValue("MFCC09", mfcc09);
		xml.addValue("MFCC10", mfcc10);
		xml.addValue("MFCC11", mfcc11);
		xml.addValue("MFCC12", mfcc12);
		xml.addValue("MFCC13", mfcc13);

	}
	else {
		return false;
	}
	if (xml.save(xmlPath))
	{
		cout << "xml file for " << name << " generated" << endl;
		return true;
	}
	else
	{
		return false;
	}

}

bool VideoFile::getMetadataFromXml()
{
	File::getMetadataFromXml();
	ofXml* xml = new ofXml(xmlPath);
	if (xml->load(xmlPath)) {
		abruptness = xml->getValue<double>("//ABRUPTNESS");
		motion = xml->getValue<double>("//MOTION");
		shake = xml->getValue<double>("//SHACKINESS");
		fgArea = xml->getValue<double>("//FG_AREA");
		focus_dif = xml->getValue<double>("//FOCUS_DIFF");
		luminance_std = xml->getValue<double>("//LUMA_STD");
		ranksum = xml->getValue<double>("//RANKSUM");
		shadow = xml->getValue<double>("//SHADOW");
		predict = xml->getValue<unsigned short>("//PREDICT");
		interest_1 = xml->getValue<int>("//INTEREST_1");

		semanticID_1 = xml->getValue<int>("//SID_1");
		semanticValue_1 = xml->getValue<double>("//SVALUE_1");
		semanticID_2 = xml->getValue<int>("//SID_2");
		semanticValue_2 = xml->getValue<double>("//SVALUE_2");
		semanticID_3 = xml->getValue<int>("//SID_3");
		semanticValue_3 = xml->getValue<double>("//SVALUE_3");
		semanticID_4 = xml->getValue<int>("//SID_4");
		semanticValue_4 = xml->getValue<double>("//SVALUE_4");
		semanticID_5 = xml->getValue<int>("//SID_5");
		semanticValue_5 = xml->getValue<double>("//SVALUE_5");

		a1_average_loudness = xml->getValue<double>("//A1");
		a2_dynamic_complexity = xml->getValue<double>("//A2");
		a3_bpm = xml->getValue<double>("//A3");
		a4_danceability = xml->getValue<double>("//A4");
		a5_onset_rate = xml->getValue<double>("//A5");
		a6_chords_change_rate = xml->getValue<double>("//A6");
		a7_chords_number_rate = xml->getValue<double>("//A7");
		a8_key_strength = xml->getValue<double>("//A8");
		a9_tuning_diatonic_strength = xml->getValue<double>("//A9");
		a10_tuning_equal_tempered_deviation = xml->getValue<double>("//A10");
		a11_tuning_nontempered_energy_ratio = xml->getValue<double>("//A11");
		mfcc01 = xml->getValue<double>("//MFCC01");
		mfcc02 = xml->getValue<double>("//MFCC02");
		mfcc03 = xml->getValue<double>("//MFCC03");
		mfcc04 = xml->getValue<double>("//MFCC04");
		mfcc05 = xml->getValue<double>("//MFCC05");
		mfcc06 = xml->getValue<double>("//MFCC06");
		mfcc07 = xml->getValue<double>("//MFCC07");
		mfcc08 = xml->getValue<double>("//MFCC08");
		mfcc09 = xml->getValue<double>("//MFCC09");
		mfcc10 = xml->getValue<double>("//MFCC10");
		mfcc11 = xml->getValue<double>("//MFCC11");
		mfcc12 = xml->getValue<double>("//MFCC12");
		mfcc13 = xml->getValue<double>("//MFCC13");

	}
	return false;
}

void VideoFile::setThumbnailPath()
{
	thumbnailPath = thumbnailFolderPath + "\\" + name + ".jpg";
}

void VideoFile::draw(int x, int y, bool play)
{
	cout << name + " clicked" << endl;
}

void VideoFile::stopVideo()
{
	if (video.isPlaying()) {
		video.stop();
	}
}

void VideoFile::draw(ofRectangle space, bool play, ofTrueTypeFont *playerText)
{
	if (!video.getCurrentFrame()) {
		video.play();
		video.update();
		video.setPaused(true);
	}

	if (play) {
		video.play();
		video.update();
	}
	else {
		video.setPaused(!play);
	}
	video.draw(space);
	playerText->drawString(ofToString(video.getCurrentFrame()) + " / " + ofToString(video.getTotalNumFrames()), space.x, space.y - 3);
}
bool VideoFile::getMetadataFromCsv(vector <string> csvSingleData)
{
	File::getMetadataFromCsv(csvSingleData);

	abruptness = std::stod(csvSingleData[35]);
	motion = std::stod(csvSingleData[31]);
	shake = std::stod(csvSingleData[30]);
	fgArea = std::stod(csvSingleData[32]);
	focus_dif = std::stod(csvSingleData[36]);
	luminance_std = std::stod(csvSingleData[14]);
	ranksum = std::stod(csvSingleData[27]);
	shadow = std::stod(csvSingleData[33]);
	predict = std::stoi(csvSingleData[37]);
	interest_1 = std::stoi(csvSingleData[38]);

	return true;
}

void VideoFile::getMetadataFromSemanticSample(vector<pair<double, int> > semanticSample)
{
	semanticID_1 = semanticSample[0].second;
	semanticValue_1 = semanticSample[0].first;
	semanticID_2 = semanticSample[1].second;
	semanticValue_2 = semanticSample[1].first;
    semanticID_3 = semanticSample[2].second;
    semanticValue_3 = semanticSample[2].first;
	semanticID_4 = semanticSample[3].second;
	semanticValue_4 = semanticSample[3].first;
	semanticID_5 = semanticSample[4].second;
	semanticValue_5 = semanticSample[4].first;

}

void VideoFile::getMetadataFromAudioSample(vector<double> audioSample)
{
	if (audioSample.size() > 0)
	{
		a1_average_loudness = audioSample[0];
		a2_dynamic_complexity = audioSample[1] / 30;  //adhoc norm, from feature max
		a3_bpm = audioSample[3] / 200;                //adhoc norm, from feature max
		a4_danceability = audioSample[4] / 4;         //adhoc norm, from feature max
		a5_onset_rate = audioSample[5] / 10;           //adhoc norm, from feature max
		a6_chords_change_rate = audioSample[6];
		a7_chords_number_rate = audioSample[7];
		a8_key_strength = audioSample[8];
		a9_tuning_diatonic_strength = audioSample[9];
		a10_tuning_equal_tempered_deviation = audioSample[10];
		a11_tuning_nontempered_energy_ratio = audioSample[11];
		mfcc01 = audioSample[12];
		mfcc02 = audioSample[13];
		mfcc03 = audioSample[14];
		mfcc04 = audioSample[15];
		mfcc05 = audioSample[16];
		mfcc06 = audioSample[17];
		mfcc07 = audioSample[18];
		mfcc08 = audioSample[19];
		mfcc09 = audioSample[20];
		mfcc10 = audioSample[21];
		mfcc11 = audioSample[22];
		mfcc12 = audioSample[23];
		mfcc13 = audioSample[24];
	}
	else {

		a1_average_loudness = 0;
		a2_dynamic_complexity = 0;  
		a3_bpm = 0;      
		a4_danceability = 0;  
		a5_onset_rate = 0;      
		a6_chords_change_rate = 0;
		a7_chords_number_rate = 0;
		a8_key_strength = 0;
		a9_tuning_diatonic_strength = 0;
		a10_tuning_equal_tempered_deviation = 0;
		a11_tuning_nontempered_energy_ratio = 0;
		mfcc01 = 0;
		mfcc02 = 0;
		mfcc03 = 0;
		mfcc04 = 0;
		mfcc05 = 0;
		mfcc06 = 0;
		mfcc07 = 0;
		mfcc08 = 0;
		mfcc09 = 0;
		mfcc10 = 0;
		mfcc11 = 0;
		mfcc12 = 0;
		mfcc13 = 0;
	
	}

}

string VideoFile::filesFolderPath = "files";
string VideoFile::thumbnailFolderPath = "/thumbnails/videos/";
string VideoFile::thumbnailIconPath = "/icons/video.jpg";
vector <string> VideoFile::availableFormats = { "mp4", "MTS", "mov" };