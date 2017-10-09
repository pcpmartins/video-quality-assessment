#include "metadataPanel.h"

metadataPanel::metadataPanel()
{
}

metadataPanel::~metadataPanel()
{
}

void metadataPanel::getData(const ImageFile * img)
{
	metadataString = createCommonString(img);
	color1 = getColor1(img);
	color2 = getColor2(img);
}

void metadataPanel::getData(const VideoFile * vid)
{
	metadataString = createCommonString(vid);
	metadataString += "                   Foreground area: " + toString(vid->fgArea) + "\n";
	metadataString += "    Avg faces: " + toString(vid->avgFaces) +
		"                     Focus diff: " + toString(vid->focus_dif) + "\n";
	metadataString += "    Faces area: " + toString(vid->faceArea) +
		"                   Shadows: " + toString(vid->shadow) + "\n";
	metadataString += "    Rule of thirds: " + toString(vid->rule3) +
		"                Abruptness: " + toString(vid->abruptness) + "\n";
	metadataString += "    Smiles: " + toString(vid->smiles) +
		"                          Static saliency: " + toString(vid->static_saliency) + "\n";
	metadataString += "                                                  Motion: " + toString(vid->motion) + "\n" +
		"                                                  Shakiness: " + toString(vid->shake) + "\n\n";
	metadataString += "\n";
	metadataString += +"\n";
	metadataString += "   Aesthethic: " + toString(vid->predict).substr(0, 1) +
		"                       Similarity: " + toString(vid->similarityIndex) + "\n";
	metadataString += "   Interestingness: " + toString(vid->interest_1).substr(0, 1)
		+ " " + "             Ref: " + vid->referenceName + "\n\n";

	color1 = getColor1(vid);
	color2 = getColor2(vid);

}

string metadataPanel::createCommonString(const File * f)
{
	string s;
	s = "\n   File name: " + f->name + f->extension + "\n";						//Name of file
	s += "   Resolution: " + std::to_string(f->resX) + " x " + std::to_string(f->resY) + "\n";
	s += "   UGC rating: " + std::to_string(f->rate) + "                      Objective index: " + toString(f->ranksum) + "\n\n";
	s += "   Orientation:" + std::to_string(f->ehGlobal) + "  Color Ratio " + "     Luminance: " + toString(f->luminance) + "\n";
	s += "    " + std::to_string(f->eh1) + "   " + std::to_string(f->eh2) + "   " + std::to_string(f->eh3) + "   " +
		std::to_string(f->eh4) + "       R: " + toString(f->redRatio) + "          Luminance_std: " + toString(f->luminance_std) + "\n";
	s += "    " + std::to_string(f->eh5) + "   " + std::to_string(f->eh6) + "   " + std::to_string(f->eh7) + "   " +
		std::to_string(f->eh8) + "       G: " + toString(f->greenRatio) + "          Sharpness: " + toString(f->sharpness) + "\n";
	s += "    " + std::to_string(f->eh9) + "   " + std::to_string(f->eh10) + "   " + std::to_string(f->eh11) + "   " +
		std::to_string(f->eh12) + "       B: " + toString(f->blueRatio) + "\n";
	s += "    " + std::to_string(f->eh13) + "   " + std::to_string(f->eh14) + "   " + std::to_string(f->eh15) + "   " +
		std::to_string(f->eh16) + "                             Dif. hues: " + toString(f->dif_hues) + "\n";
	s += "                                                  Entropy: " + std::to_string(f->entropy) + "\n";
	s += "\n";
	s += "                               ";

	return s;
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
	middle.set(p3, 152, 75);
	ofRect(middle);

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


