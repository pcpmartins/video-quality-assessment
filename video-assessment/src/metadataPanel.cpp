#include "metadataPanel.h"

metadataPanel::metadataPanel()
{
}

metadataPanel::~metadataPanel()
{
}

void metadataPanel::getData(const VideoFile * vid)
{
	metadataString = "\n   File name: " + vid->name + vid->extension + "\n";	
	metadataString += "   Resolution: " + std::to_string(vid->resX) + " x " + std::to_string(vid->resY) + "\n";
	metadataString += "   Objective index: " + toString(vid->ranksum) +"              User Rating: " + std::to_string(vid->rate) +"\n\n";
	metadataString += "       Edge strenght: " + std::to_string(vid->edgeStrenght).substr(0,4) + "                      Color Ratio " + "\n";
	metadataString += "       Edge orientation: " + std::to_string(vid->ehGlobal) + "                          R: " + toString(vid->redRatio) + "\n";
	metadataString += "             " + std::to_string(vid->eh1) + "   " + std::to_string(vid->eh2) + "   " + std::to_string(vid->eh3) + "   " +
		std::to_string(vid->eh4) + "                                 G: " + toString(vid->greenRatio) + "\n";
	metadataString += "             " + std::to_string(vid->eh5) + "   " + std::to_string(vid->eh6) + "   " + std::to_string(vid->eh7) + "   " +
		std::to_string(vid->eh8) + "                                 B: " + toString(vid->blueRatio) + "\n";
	metadataString += "             " + std::to_string(vid->eh9) + "   " + std::to_string(vid->eh10) + "   " + std::to_string(vid->eh11) + "   " +
		std::to_string(vid->eh12)  + "\n";
	metadataString += "             " + std::to_string(vid->eh13) + "   " + std::to_string(vid->eh14) + "   " + std::to_string(vid->eh15) + "   " +
		std::to_string(vid->eh16) +"\n";
	metadataString += +"\n";
	
	metadataString += "    Luminance: " + toString(vid->luminance) +
		"                    Luminance_std: " + toString(vid->luminance_std) + "\n";

	metadataString += "    Hue count: " + toString(vid->dif_hues) +
		"                     Foreground area: " + toString(vid->fgArea) + "\n";
	metadataString += "    Sharpness: " + toString(vid->sharpness) +
		"                    Focus diff: " + toString(vid->focus_dif) + "\n";
	metadataString += "    Entropy: " + std::to_string(vid->entropy).substr(0, 4) +
		"                         Shadows: " + toString(vid->shadow) + "\n";
	metadataString += "                                                  Abruptness: " + toString(vid->abruptness) + "\n";
	                  
	metadataString += "    Avg faces: " + toString(vid->avgFaces) +
		"                     Static saliency: " + toString(vid->static_saliency) + "\n";
	                 
	metadataString += "    Faces area: " + toString(vid->faceArea) +"                   Motion: " + toString(vid->motion) + "\n" +
		"    Rule of thirds: " + toString(vid->rule3) + "                Shakiness: " + toString(vid->shake) + "\n";
	   

	metadataString += "    Smiles: " + toString(vid->smiles) + "\n";
	metadataString += +"\n";
	metadataString += "   Aesthethic: " + toString(vid->predict).substr(0, 1) +
		"                          Similarity: " + toString(vid->similarityIndex) + "\n";
	metadataString += "   Interestingness: " + toString(vid->interest_1).substr(0, 1)
		+ " " + "                  Ref: " + vid->referenceName + "\n\n";

	color1 = getColor1(vid);
	color2 = getColor2(vid);

}

void metadataPanel::setup(int x, int y)
{
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(255, 255, 255, 255);
	font.setup("arial.ttf", //font file, ttf only
		1.0,					//lineheight percent
		1024,					//texture atlas dimension
		true,					//create mipmaps of the font, useful to scale down the font at smaller sizes
		8,					//texture atlas element padding, shouldbe >0 if using mipmaps otherwise
		2.0f					//dpi scaleup, render textures @2x the reso
	);					//lower res mipmaps wil bleed into each other
	fontColor = ofColor(0, 0, 0);

	height = 300;		//Panel height;
	width = 320;
	this->x = x;		//Panel coordinates
	this->y = y;
}

void metadataPanel::draw(int x, int y)
{
	this->x = x;
	this->y = y;

	ofSetColor(ofColor(255, 255, 255));
	font.drawMultiLine(metadataString, fontSize, x, y);
	// multiline bbox /////////////////////////////////////////////////////

	//Border
	bboxMultiline = font.getBBox(metadataString, fontSize, x, y);
	bboxMultiline.setHeight(height);
	bboxMultiline.setWidth(width);
	ofSetLineWidth(0.5);
	ofNoFill();
	ofRect(bboxMultiline);

	//title
	ofPoint p1(x + 4, y);
	title.set(p1, 310, 44);
	ofRect(title);

	//middle
	ofPoint p3(x + 4, y + 48);
	middle.set(p3, 152, 85);
	ofRect(middle);

	//middle right
	ofPoint p4(x + 4 +152 +4, y + 48);
	middleRight.set(p4, 154, 85);
	ofRect(middleRight);

	ofPoint p2(x + 4, y + 264);
	advanced.set(p2, 310, 30);
	ofRect(advanced);

	//Fill
	ofSetLineWidth(3.0);
	ofSetColor(color1);
	ofFill();
	ofRect(bboxMultiline);
	ofSetColor(255, 255, 255);
}

float metadataPanel::getWidth()
{
	return width;
}

float metadataPanel::getHeight()
{
	return height;
}

float metadataPanel::getCordX()
{
	return x;
}

float metadataPanel::getCordY()
{
	return y;
}



string metadataPanel::toString(double x)
{
	stringstream stream;
	stream << fixed << setprecision(2) << x;
	string s = stream.str();
	return s;
}

ofColor metadataPanel::getColor1(const File * f)
{
	int red = f->redMoments.first;
	int green = f->greenMoments.first;
	int blue = f->blueMoments.first;
	ofColor c(red, green, blue, 100);
	return c;
}

ofColor metadataPanel::getColor2(const File * f)
{

	int red = 0;
	int green = 0;
	int blue = 0;
	ofColor c(red, green, blue, 255);
	return c;
}


