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

	/*
	if (!ofFile::doesFileExist(thumbnailPath)) {			//If there is no thumbnail
		if (video.load(path)) {								//Load video to class			
			generateThumbnail();		//Generate thumbnail for file and returns path to it	
		}
	}
	*/
	loadThumbnail();				//Load thumbnail	
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
/*
string VideoFile::generateThumbnail()
{
	auto start = chrono::high_resolution_clock::now();
	ofImage thumbnail;
	ofImage temp;

	ofDirectory dir(thumbnailFolderPath);					//Directory with thubmnails

	video.play();
	video.setPosition(0.15);
	//video.update();


	if (video.isLoaded())
		thumbnail.setFromPixels(video.getPixelsRef());
	else {
		ofPixels p;
		ofLoadImage(p, "black.jpg");
		thumbnail.setFromPixels(p);
	}
	video.stop();
	double tempHeigth;
	if (video.getWidth() > video.getHeight()) {
		tempHeigth = (thumbnailWidth / video.getWidth())*video.getHeight();
	}
	else {
		tempHeigth = thumbnailHeight;
	}

	thumbnail.resize(thumbnailWidth, tempHeigth);

	cout << "thumbnail created for " << name + extension ;

	string path = thumbnailFolderPath + "\\" + name + ".jpg";
	

	if (!dir.exists())	//If directory with thubmnails doesn't exist
	{
		cout << "Create directory for videos thumbnails" << endl;

		if (dir.create(true)) {
			cout << dir.getAbsolutePath() << " created" << endl;
		}
		else
		{
			cout << "Cannot create directory for videos thumbnails" << endl;
			ofExit();
		}
	}

	thumbnail.saveImage(path);								//Save it to directories with thumbnails
	temp.clear();
	thumbnail.clear();
	video.close();
	video.closeMovie();
	auto end = chrono::high_resolution_clock::now();
	cout << " in: " << std::chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

	return path;
}
*/
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

string VideoFile::filesFolderPath = "files";
string VideoFile::thumbnailFolderPath = "/thumbnails/videos/";
string VideoFile::thumbnailIconPath = "/icons/video.jpg";
vector <string> VideoFile::availableFormats = { "mp4", "MTS", "mov" };