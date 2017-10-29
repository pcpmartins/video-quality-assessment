#include "File.h"

File::File()
{
	visible = true;		//At start, all files should be visible
	isCurrentFile = false;  //At start, there is no current file
}


File::~File()
{
}

/*string File::generateThumbnail()
{
	cout << "generateThumbnail()" << endl;
	return NULL;
}
*/
bool File::generateXmlFile()
{
	ofXml xml;
	xml.addChild("FILE");
	xml.setTo("//FILE");

	xml.addValue("THUMBNAIL_PATH", thumbnailPath);		//Thumbnail path field
	xml.addValue("FILE_PATH", path);					//File path field

	xml.addChild("RESOLUTION");
	xml.setTo("RESOLUTION");
	xml.addValue("X", resX);
	xml.addValue("Y", resY);

	xml.setTo("//FILE");
	xml.addValue("RATE", rate);
	xml.addValue("LUMINANCE", luminance);				//Luminance value
	xml.addValue("SHARPNESS", sharpness);
	xml.addValue("DIF_HUES", dif_hues);
	xml.addValue("SIMILARITY", similarityIndex);
	xml.addValue("REFERENCE", referenceName);
	xml.addValue("HUMAN_FACE", humanFace);
	xml.addValue("AVG_FACES", avgFaces);
	xml.addValue("FACE_AREA", faceArea);
	xml.addValue("AVG_HAAR", smiles);
	xml.addValue("RULE_OF_THIRDS", rule3);
	xml.addValue("STATIC_SALIENCY", static_saliency);
	xml.addValue("ENTROPY", entropy);
	xml.addValue("EHSTRENGHT", edgeStrenght);

	xml.addChild("COLOUR");								//Colours values
	xml.setTo("COLOUR");
	xml.addValue("RED1", redMoments.first);
	xml.addValue("GREEN1", greenMoments.first);
	xml.addValue("BLUE1", blueMoments.first);
	xml.addValue("RED2", redMoments.second);
	xml.addValue("GREEN2", greenMoments.second);
	xml.addValue("BLUE2", blueMoments.second);
	xml.addValue("RED_RATIO", redRatio);
	xml.addValue("GREEN_RATIO", greenRatio);
	xml.addValue("BLUE_RATIO", blueRatio);

	xml.setTo("//FILE");
	xml.addChild("EH");								//edge histograms
	xml.setTo("EH");
	xml.addValue("EH1", eh1);
	xml.addValue("EH2", eh2);
	xml.addValue("EH3", eh3);
	xml.addValue("EH4", eh4);
	xml.addValue("EH5", eh5);
	xml.addValue("EH6", eh6);
	xml.addValue("EH7", eh7);
	xml.addValue("EH8", eh8);
	xml.addValue("EH9", eh9);
	xml.addValue("EH10", eh10);
	xml.addValue("EH11", eh11);
	xml.addValue("EH12", eh12);
	xml.addValue("EH13", eh13);
	xml.addValue("EH14", eh14);
	xml.addValue("EH15", eh15);
	xml.addValue("EH16", eh16);
	xml.addValue("EHGLOBAL", ehGlobal);
	xml.setTo("//FILE");
	

	if (xml.save(xmlPath))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool File::getMetadataFromXml()
{
	ofXml* xml = new ofXml(xmlPath);
	if (xml->load(xmlPath)) {
		thumbnailPath = xml->getValue<string>("//THUMBNAIL_PATH");
		path = xml->getValue<string>("//FILE_PATH");
		rate = xml->getValue<int>("//RATE");
		resX = xml->getValue<int>("//X");
		resY = xml->getValue<int>("//Y");

		luminance = xml->getValue<double>("//LUMINANCE");
		sharpness = xml->getValue<double>("//SHARPNESS");
		dif_hues = xml->getValue<double>("//DIF_HUES");
		static_saliency = xml->getValue<double>("//STATIC_SALIENCY");
		entropy = xml->getValue<double>("//ENTROPY");
		edgeStrenght = xml->getValue<double>("//EHSTRENGHT");

		redMoments.first = xml->getValue<double>("//RED1");
		greenMoments.first = xml->getValue<double>("//GREEN1");
		blueMoments.first = xml->getValue<double>("//BLUE1");
		redMoments.second = xml->getValue<double>("//RED2");
		greenMoments.second = xml->getValue<double>("//GREEN2");
		blueMoments.second = xml->getValue<double>("//BLUE2");
		redRatio = xml->getValue<double>("//RED_RATIO");
		greenRatio = xml->getValue<double>("//GREEN_RATIO");
		blueRatio = xml->getValue<double>("//BLUE_RATIO");

		humanFace = xml->getValue<int>("//HUMAN_FACE");		//0 if no face, 1 if there is face. Save boolean
		rule3 = xml->getValue<double>("//RULE_OF_THIRDS");
		avgFaces = xml->getValue<double>("//AVG_FACES");
		faceArea = xml->getValue<double>("//FACE_AREA");
		smiles = xml->getValue<double>("//AVG_HAAR");
		similarityIndex = xml->getValue<double>("//SIMILARITY");
		referenceName = xml->getValue<string>("//REFERENCE");

		eh1 = xml->getValue<int>("//EH1");
		eh2 = xml->getValue<int>("//EH2");
		eh3 = xml->getValue<int>("//EH3");
		eh4 = xml->getValue<int>("//EH4");
		eh5 = xml->getValue<int>("//EH5");
		eh6 = xml->getValue<int>("//EH6");
		eh7 = xml->getValue<int>("//EH7");
		eh8 = xml->getValue<int>("//EH8");
		eh9 = xml->getValue<int>("//EH9");
		eh10 = xml->getValue<int>("//EH10");
		eh11 = xml->getValue<int>("//EH11");
		eh12 = xml->getValue<int>("//EH12");
		eh13 = xml->getValue<int>("//EH13");
		eh14 = xml->getValue<int>("//EH14");
		eh15 = xml->getValue<int>("//EH15");
		eh16 = xml->getValue<int>("//EH16");
		ehGlobal = xml->getValue<int>("//EHGLOBAL");
		

		return true;
	}
	else
	{
		return false;
	}
}

bool File::getMetadataFromCsv(vector <string> csvSingleData)
{
	if (std::stoi(csvSingleData[0]) != 0) {
	
		resX = std::stoi(csvSingleData[1]);
		resY = std::stoi(csvSingleData[2]);

		luminance = std::stod(csvSingleData[13]);
		sharpness = std::stod(csvSingleData[12]);
		dif_hues = std::stod(csvSingleData[21]);
		static_saliency = std::stod(csvSingleData[26]);
		entropy = std::stod(csvSingleData[58]);
		edgeStrenght = std::stod(csvSingleData[59]);

		redMoments.first = std::stod(csvSingleData[4]);
		greenMoments.first = std::stod(csvSingleData[7]);
		blueMoments.first = std::stod(csvSingleData[10]);
		redMoments.second = std::stod(csvSingleData[5]);
		greenMoments.second = std::stod(csvSingleData[8]);
		blueMoments.second = std::stod(csvSingleData[11]);
		redRatio = std::stod(csvSingleData[3]);
		greenRatio = std::stod(csvSingleData[6]);
		blueRatio = std::stod(csvSingleData[9]);

		avgFaces = std::stod(csvSingleData[22]);
		rule3 = std::stod(csvSingleData[25]);

		if (avgFaces > 0) humanFace = 1;
		else humanFace = 0;

		faceArea = std::stod(csvSingleData[23]);
		smiles = std::stod(csvSingleData[24]);

		similarityIndex = 0;
		referenceName = "none";

		eh1 = std::stoi(csvSingleData[41]);
		eh2 = std::stoi(csvSingleData[42]);
		eh3 = std::stoi(csvSingleData[43]);
		eh4 = std::stoi(csvSingleData[44]);
		eh5 = std::stoi(csvSingleData[45]);
		eh6 = std::stoi(csvSingleData[46]);
		eh7 = std::stoi(csvSingleData[47]);
		eh8 = std::stoi(csvSingleData[48]);
		eh9 = std::stoi(csvSingleData[49]);
		eh10 = std::stoi(csvSingleData[50]);
		eh11 = std::stoi(csvSingleData[51]);
		eh12 = std::stoi(csvSingleData[52]);
		eh13 = std::stoi(csvSingleData[53]);
		eh14 = std::stoi(csvSingleData[54]);
		eh15 = std::stoi(csvSingleData[55]);
		eh16 = std::stoi(csvSingleData[56]);
		ehGlobal = std::stoi(csvSingleData[57]);	

		return true;
	}
	else {
		return false;
	}
}

bool File::loadThumbnail()
{
	cout << "loadThumbnail()" << endl;
	return false;
}

bool File::separateNameFromExtension()
{
	size_t lastindex = name.find_last_of(".");
	if (lastindex == string::npos) {		//No "." in file name. 
		return false;
		cout << "File without extension?" << endl;
	}
	else {
		extension = name.substr(lastindex, name.length());
		name = name.substr(0, lastindex);
		return true;
	}

}

void File::setXmlPath()
{
	xmlPath = xmlFolderPath + name + extension + ".xml";
}

void File::rateUpdate(int newRate)
{
	if (newRate > 5 || newRate < 0) {
		cout << "[error: ]New rate must be beetwen 0..5" << endl;
	}
	else {
		rate = newRate;		//Rate update

							//xmlFile update
		ofXml* xml = new ofXml(xmlPath);
		if (xml->load(xmlPath))
		{
			xml->setValue("//RATE", std::to_string(newRate));
		}

		if (xml->save(xmlPath))
		{
			cout << name << " rate updated to: " << rate << endl;
		}
	}
}

void File::indexUpdate(float newIndex, string newReference)
{
	similarityIndex = (double)newIndex;		//Rate update

	ofXml* xml = new ofXml(xmlPath); //xmlFile update
	if (xml->load(xmlPath))
	{
		xml->setValue("//SIMILARITY", std::to_string(newIndex));
		xml->setValue("//REFERENCE", (newReference));
	}

	if (!xml->save(xmlPath))
	{
		cout << " Error saving similarity to xml: " << endl;
	}
}

void File::setThumbnailPath()
{
	cout << "setThumbnailPath" << endl;
}

void File::setType(fileType type)
{
	this->type = type;
}

File::fileType File::getType()
{
	return type;
}

bool File::getVisible()
{
	return visible;
}

void File::setVisible(bool visibility)
{
	visible = visibility;
}

bool File::getIsCurrentFile()
{
	return isCurrentFile;
}

void File::setIsCurrentFile(bool currentFile)
{
	isCurrentFile = currentFile;
}

string File::xmlFolderPath = "/xml/";
int File::thumbnailHeight = 100;
int File::thumbnailWidth = 120;