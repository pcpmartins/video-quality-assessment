#include "VideoFile.h"

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


	if (!ofFile::doesFileExist(thumbnailPath)) {			//If there is no thumbnail
		if (video.load(path)) {								//Load video to class			
			generateThumbnail();		//Generate thumbnail for file and returns path to it	
		}
	}
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

string VideoFile::generateThumbnail()
{
	ofImage thumbnail;
	ofImage temp;
	//int upperMargin;

	ofDirectory dir(thumbnailFolderPath);					//Directory with thubmnails

	video.play();
	video.setPosition(0.15);
	video.update();

	if (video.isLoaded())
		thumbnail.setFromPixels(video.getPixelsRef());
	else {
		ofPixels p;
		ofLoadImage(p, "black.jpg");
		thumbnail.setFromPixels(p);
	}
	double tempHeigth;
	if (video.getWidth() > video.getHeight()) {
		tempHeigth = (thumbnailWidth / video.getWidth())*video.getHeight();
	}
	else {
		tempHeigth = thumbnailHeight;
	}
	//upperMargin = (thumbnailHeight - tempHeigth) / 2;

	thumbnail.resize(thumbnailWidth, tempHeigth);

	cout << "thumbnail created for " << name + extension << endl;

	string path = thumbnailFolderPath + "\\" + name + ".jpg";
	video.stop();
	if (!dir.exists())	//If directory with thubmnails doesn't exist
	{
		cout << "Create directory for videos thumbnails" << endl;

		if (dir.create(true)) {
			cout << dir.getAbsolutePath() << " created" << endl;
		}
		else
		{
			cout << "Cannot create directory for videos thumbnails" << endl;
			::ofExit();
		}
	}

	thumbnail.saveImage(path);								//Save it to directories with thumbnails
	return path;
}

bool VideoFile::generateXmlFile()
{
	File::generateXmlFile();
	ofXml xml;
	if (xml.load(xmlPath))
	{
		//xml.setTo("//FILE");
		xml.addValue("ABRUPTNESS", abruptness);
		xml.addValue("SHARPNESS", sharpness);
		xml.addValue("MOTION", motion);
		xml.addChild("RESOLUTION");
		xml.setTo("RESOLUTION");
		xml.addValue("X", resX);
		xml.addValue("Y", resY);
		xml.setTo("//FILE");
		xml.addValue("TYPE", "VIDEO");
		xml.addValue("AVG_FACES", avgFaces);
		xml.addValue("FACE_AREA", faceArea);
		xml.addValue("AVG_HAAR", smiles);
		xml.addValue("RULE_OF_THIRDS", rule3);
		xml.addValue("SHACKINESS", shake);
		xml.addValue("FG_AREA", fgArea);
		xml.addValue("FOCUS_DIFF", focus_dif);
		xml.addValue("LUMA_STD", luminance_std);
		xml.addValue("DIF_HUES", dif_hues);
		xml.addValue("STATIC_SALIENCY", static_saliency);
		xml.addValue("RANKSUM", ranksum);
		xml.addValue("SHADOW", shadow);
		xml.addValue("PREDICT", predict);
		xml.addValue("HUMAN_FACE", humanFace);

		xml.addValue("INTEREST_1", interest_1);
		xml.addValue("INTEREST_2", interest_2);
		xml.addValue("INTEREST_3", interest_3);
		xml.addValue("SIMILARITY", similarityIndex);
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
		sharpness = xml->getValue<double>("//SHARPNESS");
		motion = xml->getValue<double>("//MOTION");
		avgFaces = xml->getValue<double>("//AVG_FACES");
		faceArea = xml->getValue<double>("//FACE_AREA");
		smiles = xml->getValue<double>("//AVG_HAAR");
		rule3 = xml->getValue<double>("//RULE_OF_THIRDS");
		shake = xml->getValue<double>("//SHACKINESS");
		fgArea = xml->getValue<double>("//FG_AREA");
		focus_dif = xml->getValue<double>("//FOCUS_DIFF");
		luminance_std = xml->getValue<double>("//LUMA_STD");
		dif_hues = xml->getValue<double>("//DIF_HUES");
		static_saliency = xml->getValue<double>("//STATIC_SALIENCY");
		ranksum = xml->getValue<double>("//RANKSUM");
		shadow = xml->getValue<double>("//SHADOW");
		predict = xml->getValue<unsigned short>("//PREDICT");
		humanFace = xml->getValue<int>("//HUMAN_FACE");
		interest_1 = xml->getValue<int>("//INTEREST_1");
		interest_2 = xml->getValue<int>("//INTEREST_2");
		interest_3 = xml->getValue<int>("//INTEREST_3");
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

	sharpness = std::stod(csvSingleData[12]);
	abruptness = std::stod(csvSingleData[35]);
	motion = std::stod(csvSingleData[31]);
	rule3 = std::stod(csvSingleData[25]);
	shake = std::stod(csvSingleData[30]);

	avgFaces = std::stod(csvSingleData[22]);
	if (avgFaces > 0.0f)humanFace = 1;
	else humanFace = 0;

	faceArea = std::stod(csvSingleData[23]);
	smiles = std::stod(csvSingleData[24]);
	fgArea = std::stod(csvSingleData[32]);
	focus_dif = std::stod(csvSingleData[36]);
	luminance_std = std::stod(csvSingleData[14]);
	dif_hues = std::stod(csvSingleData[21]);
	static_saliency = std::stod(csvSingleData[26]);
	ranksum = std::stod(csvSingleData[27]);
	shadow = std::stod(csvSingleData[33]);
	predict = std::stoi(csvSingleData[37]);
	interest_1 = std::stoi(csvSingleData[38]);
	interest_2 = std::stoi(csvSingleData[39]);
	interest_3 = std::stoi(csvSingleData[40]);

	similarityIndex = 0;

	return true;
}

string VideoFile::filesFolderPath = "files";
string VideoFile::thumbnailFolderPath = "/thumbnails/videos/";
string VideoFile::thumbnailIconPath = "/icons/video.jpg";
vector <string> VideoFile::availableFormats = { "mp4", "MTS", "mov", "avi" };