#pragma once

#include "ofMain.h"

//using namespace std;

class File
{
public:
	File();
	~File();

	enum fileType
	{
		IMAGE,
		VIDEO
	};

	static int thumbnailWidth;		//Width of the miniature 
	static int thumbnailHeight;		//Height of the miniature 
	int posX;						//Position of the middle size picture
	int posY;
	int resX;					//Resolution X
	int resY;					//Resolution Y
	string name;				//Name of file without extension!
	string extension;			//Extension of file
	string path;				//Full path to file
	string thumbnailPath;		//Path to thumbnail

	ofImage thumbnail;			//Thumbnail
	string xmlPath;				//Path to metadata file
	static string xmlFolderPath;//Path to folder with metadata

	//Metadata
	pair<double, double> redMoments;	//First and second red color moment
	pair<double, double> greenMoments;	//First and second green color moment
	pair<double, double> blueMoments;	//First and second green color moment
	double redRatio;					//red colour to all colour first moments ratio
	double greenRatio;					//green colour to all colour first moments ratio
	double blueRatio;					//Blue colour to all colour first moments ratio
	double edges;						//Edges description
	double edgesHorizontal;				//Horizontals edge
	double edgesVertical;				//Vertical edge
	double edgesDiagonal;
	string edgeDistribution;			//Horizontal or vertical
	double edgesHorizontalRatio;		//Edges horizontal ratio
	double edgesVerticalRatio;			//Edges vertical ratio
	double edgesDiagonalRatio;
	int humanFace;						//If human detected
	double luminance;
	unsigned short rate;					//Rate of file (0..5);
	double simplicity;						// More complex = less aesthetic
	double sharpness;
	double abruptness;
	double rule3;
	double shake;
	double motion;

	double avgFaces;
	double faceArea;
	double smiles;
	double fgArea;
	double focus_dif;

	//novos
	double luminance_std;
	double dif_hues;
	double static_saliency;
	double ranksum;
	double shadow;
	unsigned short predict;
	int interest_1, interest_2, interest_3;

	double similarityIndex;
	string referenceName;
	int eh1, eh2, eh3, eh4, eh5, eh6, eh7, eh8, eh9, eh10, eh11, eh12, eh13, eh14, eh15, eh16, ehGlobal;
	double entropy;

protected:
	ofImage thumbnailIcon;				//Thumbnail icon
private:
	fileType type;				//Image or video
	bool visible;				//Say if file should be visible in panel. It is changed by the filtersPanel class
	bool isCurrentFile;           //currently selected file


//////////////////////////////Methods////////////////////////////////////
public:
	virtual string generateThumbnail();			//Generates thumbnails.	
	virtual bool generateXmlFile();				//Create xml file
	virtual bool getMetadataFromXml();			//Get data from xml
	virtual bool getMetadataFromCsv(vector<string> csvData); // csv parse

	virtual bool loadThumbnail();				//Load thumbnail from disk to ofImage
	virtual void setThumbnailPath();			//Sets path to thumbnail	
	bool separateNameFromExtension();			//Remove extension from fileName
	void setType(fileType type);				//Sets file Type. Required when creating object from name and path
	fileType getType();
	void rateUpdate(int newRate);				//Update rate field. Update xml file

	void indexUpdate(float newIndex, string newReference);

	bool getVisible();
	void setVisible(bool visibility);

	bool getIsCurrentFile();
	void setIsCurrentFile(bool currentFile);


protected:
	void setXmlPath();
};

