#pragma once
#include "File.h"
#include "ofMain.h"

class ImageFile :
	public File
{
public:
	ImageFile();
	ImageFile(const File *f);
	ImageFile(string name, string path);
	~ImageFile();

	ofImage image;								//Image
	static string filesFolderPath;				//Path to files
	static string thumbnailFolderPath;			//Path to thumbnails
	static string thumbnailIconPath;			//Path to camera icon to add to thumbnail
	static vector <string> avaiableFormats;		//Required to distinguish images from videos and other files

	//Image metadata:

private:


///////////////////////////////////////Methods/////////////////////////////////////////////
public:

	void draw(int x, int y);					//Call when clicked. Show metadata 
	void draw(ofRectangle space);
	//string generateThumbnail() override;
	bool generateXmlFile() override;
	bool getMetadataFromXml()  override;
	//void generateMetadata();					//Generate metadata
	void setThumbnailPath() override;
	bool loadThumbnail() override;				//Load thumbnail from a disk and save it to ofImage thumbnail
	void open();								//Call when double-clicked. OPen full screen?

private:

};

