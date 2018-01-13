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

string VideoFile::filesFolderPath = "files";
string VideoFile::thumbnailFolderPath = "/thumbnails/videos/";
string VideoFile::thumbnailIconPath = "/icons/video.jpg";
vector <string> VideoFile::availableFormats = { "mp4", "MTS", "mov" };