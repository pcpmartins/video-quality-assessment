#include "ImageFile.h"



ImageFile::ImageFile()
{	
	//cout << "ImageFile created" << endl;
}

ImageFile::ImageFile(const File *f)
{
	//copy values

	name = (*f).name;
	extension = (*f).extension;
	path = (*f).path;
	thumbnailPath = (*f).thumbnailPath;
	xmlPath = (*f).xmlPath;
	rate = (*f).rate;
	image.load(path);

	if (!File::getMetadataFromXml())
		cout << "[error: File.cpp] getMetadataFromXml()" << endl;
	getMetadataFromXml();
}

ImageFile::ImageFile(string name, string path)
{
	this->path = path;
	this->name = name;
	rate = 0;
	separateNameFromExtension();
	setThumbnailPath();
	setXmlPath();

			
	if (!ofFile::doesFileExist(thumbnailPath)) {	//If there is no thumbnail
		if (image.load(path)) {						//Load image to class
			//generateThumbnail();					//Generate thumbnail for file and returns path to it
		}	
	}
	loadThumbnail();			//Load thumbnail
}

//ImageFile::ImageFile(string picturePath, int imagePosX, int imagePosY) {
//	image.loadImage(picturePath);
//	posX = imagePosX;
//	posY = imagePosY;
//}

ImageFile::~ImageFile()
{

}


void ImageFile::draw(int x, int y)
{
	ofColor c(ofColor::red);
	
	image.draw(x, y);
	ofSetColor(c);		//Set dark
	ofFill();

}

void ImageFile::draw(ofRectangle space)
{
	image.draw(space);
}
/*
string ImageFile::generateThumbnail()
{
	ofImage thumbnail = ofImage (image);					//Copy of image
	ofDirectory dir(thumbnailFolderPath);					//Directory with thubmnails
	thumbnail.resize(thumbnailWidth, thumbnailHeight);		//Resize 

	//TODO: Add camera icon or something
	string path = thumbnailFolderPath + "\\" + name + extension;
	if (!dir.exists())	//If directory with thubmnails doesn't exist
	{
		cout << "Create directory for images thumbnails" << endl;
		
		if (dir.create(true)) {
			cout << dir.getAbsolutePath() << " created" << endl;
		}
		else
		{
			cout << "Cannot create directory for images thumbnails" << endl;
			::ofExit();
		}
	}

	
	thumbnail.saveImage(path);								//Save it to directories with thumbnails
	return path;
}
*/
bool ImageFile::generateXmlFile()
{
	File::generateXmlFile();
	ofXml xml;
	xml.load(xmlPath);
	xml.setTo("//FILE");
	xml.addChild("RESOLUTION");
	xml.setTo("RESOLUTION");
	xml.addValue("X", resX);
	xml.addValue("Y", resY);
	xml.setTo("//FILE");
	xml.addValue("TYPE", "IMAGE");
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

bool ImageFile::getMetadataFromXml()
{
	ofXml* xml = new ofXml(xmlPath);
	if (xml->load(xmlPath)) {		
		return File::getMetadataFromXml();;
	}
	else {
		return false;
	}	
}
/*
void ImageFile::generateMetadata()
{
	if (!image.isAllocated()) {
		image.load(path);
	}
	metadata = metadata::metadata(image);
	File::generateMetadata();		//Wygeneruj metadane wspolne dla obrazu i wideo. Must be called after initializing metadat object
	resX = image.getWidth();
	resY = image.getHeight();
}
*/
void ImageFile::setThumbnailPath()
{
	thumbnailPath = thumbnailFolderPath + "\\" + name + extension;
}

bool ImageFile::loadThumbnail()
{
	if(!thumbnail.load(thumbnailPath))
	{
		cout << "Thumbnail loading for " << name << " image error" << endl;
		return false;
	}
	return true;
}


string ImageFile::filesFolderPath = "/files/";
//string ImageFile::filesFolderPath = "F:/certhi_numbered/";	
string ImageFile::thumbnailFolderPath = "/thumbnails/images/";
string ImageFile::thumbnailIconPath = "/icons/camera.jpg";
vector <string> ImageFile::avaiableFormats = { "jpg", "png", "jpeg" };

//luminance = pixelsRef.getColor(i,j).getLightness();