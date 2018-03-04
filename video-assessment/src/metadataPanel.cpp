#include "metadataPanel.h"

metadataPanel::metadataPanel()
{
}

metadataPanel::~metadataPanel()
{
}

void metadataPanel::getData(const VideoFile * vid)
{
	metadataString = "\n   R: " + toString(vid->redRatio) + "       File name: "
		+ vid->name.substr(0, 19) + vid->extension + "\n";
	metadataString += "   G: " + toString(vid->greenRatio) + "       Size: "
		+ std::to_string(vid->resX) + " x " + std::to_string(vid->resY)
		+ "            File ID: " + std::to_string(vid->fileID) + "\n";
	metadataString += "   B: " + toString(vid->blueRatio) + "       Objective index: "
		+ toString(vid->ranksum) + "        Group: " + std::to_string(vid->rate) + "\n\n";
	metadataString += "    Edge strenght: " + std::to_string(vid->edgeStrenght).substr(0, 5) +
		"            Loudness:          " + std::to_string(vid->a1_average_loudness).substr(0, 6) + "\n";
	metadataString += "     Global orientation: " + edgeOrientIntToString(vid->ehGlobal).substr(0, 6) +
		"             Complexity:        " + std::to_string(vid->a2_dynamic_complexity).substr(0, 6) + "\n";
	metadataString += "               " + edgeOrientIntToString(vid->eh1) + "   " + edgeOrientIntToString(vid->eh2) +
		"   " + edgeOrientIntToString(vid->eh3) + "   " + edgeOrientIntToString(vid->eh4) 
		+ "                     Bpm:                  " + std::to_string(vid->a3_bpm * 200).substr(0, 6) + "\n";
	metadataString += "               " + edgeOrientIntToString(vid->eh5) + "   " + edgeOrientIntToString(vid->eh6) +
		"   " + edgeOrientIntToString(vid->eh7) + "   " +
		edgeOrientIntToString(vid->eh8) + "                     Danceability:      " +
		std::to_string(vid->a4_danceability).substr(0, 6) + "\n";
	metadataString += "               " + edgeOrientIntToString(vid->eh9) + "   " + edgeOrientIntToString(vid->eh10) +
		"   " + edgeOrientIntToString(vid->eh11) + "   " +
		edgeOrientIntToString(vid->eh12) + "                     Onset:                " +
		std::to_string(vid->a5_onset_rate).substr(0, 6) + "\n";
	metadataString += "               " + edgeOrientIntToString(vid->eh13) + "   " + edgeOrientIntToString(vid->eh14) +
		"   " + edgeOrientIntToString(vid->eh15) + "   " +
		edgeOrientIntToString(vid->eh16) + "                     CCR:                  " +
		std::to_string(vid->a6_chords_change_rate).substr(0, 6) + "\n";
	metadataString += +"\n";

	metadataString += "    Luminance:        " + std::to_string(vid->luminance).substr(0, 6) +
		"       Luminance_std:   " + std::to_string(vid->luminance_std).substr(0, 6) + "\n";

	metadataString += "    Hue count:         " + std::to_string(vid->dif_hues).substr(0, 6) +
		"       Foreground area: " + std::to_string(vid->fgArea).substr(0, 6) + "\n";
	metadataString += "    Sharpness:        " + std::to_string(vid->sharpness).substr(0, 6) +
		"       Focus diff:            " + std::to_string(vid->focus_dif).substr(0, 6) + "\n";
	metadataString += "    Entropy:             " + std::to_string(vid->entropy).substr(0, 6) +
		"       Shadows:             " + std::to_string(vid->shadow).substr(0, 6) + "\n";
	metadataString += "    Static saliency:  " + std::to_string(vid->static_saliency).substr(0, 6) +
		"       Abruptness:          " + std::to_string(vid->abruptness).substr(0, 6) + "\n";
	metadataString += "    CFness mean:   " + std::to_string(vid->cf1).substr(0, 6)
		+ "       Motion:                 " + std::to_string(vid->motion).substr(0, 6) + "\n";
	metadataString += "    CFness std:       " + std::to_string(vid->cf2).substr(0, 6)
		+ "       Shakiness:           " + std::to_string(vid->shake).substr(0, 6) + "\n";
	metadataString += "\n";
	metadataString += "\n";
	//metadataString += std::to_string(vid->semanticValue_1).substr(0, 20)+" : "+ std::to_string(vid->semanticValue_1).substr(0, 6) + "\n";
	metadataString += "    Avg faces:         " + std::to_string(vid->avgFaces).substr(0, 6) + "\n";

	metadataString += "    Faces area:       " + std::to_string(vid->faceArea).substr(0, 6) + "\n";
	metadataString += "    Rule of thirds:    " + std::to_string(vid->rule3).substr(0, 6) + "\n";


	metadataString += "    Smiles:              " + std::to_string(vid->smiles).substr(0, 6) + "\n";
	metadataString += +"\n";

	metadataString += "   Aesthethic:          " + std::to_string(vid->predict).substr(0, 1) +
		"                    Similarity:        " + std::to_string(vid->similarityIndex).substr(0, 6) + "\n";
	metadataString += "   Interestingness:  " + std::to_string(vid->interest_1).substr(0, 1)
		+ " " + "    Ref: " + vid->referenceName.substr(0, 25) + "\n\n";

	color1 = getColor1(vid);
	color2 = getColor2(vid);

}

void metadataPanel::setup(int x, int y)
{
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	//ofBackground(255, 255, 255, 255);
	font.setup("arial.ttf", //font file, ttf only
		1.0,					//lineheight percent
		1024,					//texture atlas dimension
		true,					//create mipmaps of the font, useful to scale down the font at smaller sizes
		8,					//texture atlas element padding, shouldbe >0 if using mipmaps otherwise
		2.0f					//dpi scaleup, render textures @2x the reso
	);					//lower res mipmaps wil bleed into each other
	//fontColor = ofColor(ofColor::black);

	height = 355;		//Panel height;
	width = 320;
	this->x = x;		//Panel coordinates
	this->y = y;
}

void metadataPanel::draw(int x, int y)
{
	this->x = x;
	this->y = y;

	//ofSetColor(ofColor(255, 255, 255));

	// multiline bbox /////////////////////////////////////////////////////

	//Border
	bboxMultiline = font.getBBox(metadataString, fontSize, x, y);
	bboxMultiline.setHeight(height);
	bboxMultiline.setWidth(width);
	ofSetLineWidth(1);
	ofNoFill();
	ofRect(bboxMultiline);

	//RGB squares
	//ofRectangle redRect, greenRect, blueRect;	//Frame
	ofPoint pr(x + 4, y);
	ofFill();
	ofSetColor(255, 0, 0);
	redRect.set(pr, 55, 15);
	ofRect(redRect);
	ofNoFill();
	ofSetColor(255, 255, 255, 255);

	ofPoint pg(x + 4, y + 15);
	ofFill();
	ofSetColor(0, 255, 0);
	redRect.set(pg, 55, 12);
	ofRect(redRect);
	ofNoFill();
	ofSetColor(255, 255, 255, 255);

	ofPoint pb(x + 4, y + 30);
	ofFill();
	ofSetColor(0, 0, 255);
	redRect.set(pb, 55, 16);
	ofRect(redRect);
	ofNoFill();
	ofSetColor(255, 255, 255, 255);


	//title
	ofPoint p1(x + 4, y);
	title.set(p1, 310, 44);
	ofRect(title);

	//middle
	ofPoint p3(x + 4, y + 48);
	middle.set(p3, 152, 85);
	ofRect(middle);

	//middle right
	ofPoint p4(x + 4 + 152 + 4, y + 48);
	middleRight.set(p4, 154, 85);
	ofRect(middleRight);

	//middle2
	ofPoint p5(x + 4, y + 136);
	middle2.set(p5, 152, 110);
	ofRect(middle2);

	//middle2 right
	ofPoint p6(x + 4 + 152 + 4, y + 136);
	middleRight2.set(p6, 154, 96);
	ofRect(middleRight2);

	//middle3
	ofPoint p7(x + 4, y + 250);
	middle3.set(p7, 152, 61);
	ofRect(middle3);

	ofPoint p9(x + 160, y + 236);
	middle3Right.set(p9, 154, 75);
	ofRect(middle3Right);

	//bottom
	ofPoint p2(x + 4, y + 315);
	advanced.set(p2, 310, 32);
	ofRect(advanced);

	//Fill
	ofSetLineWidth(3.0);
	ofSetColor(color1);
	ofFill();
	ofRect(bboxMultiline);
	ofSetColor(255, 255, 255);

	font.drawMultiLine(metadataString, fontSize, x, y);
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

ofColor metadataPanel::getColor1(const VideoFile * f)
{
	int red = f->redRatio * 255;
	int green = f->greenRatio * 255;
	int blue = f->blueRatio * 255;
	ofColor c(red, green, blue, 150);
	return c;
}

ofColor metadataPanel::getColor2(const VideoFile * f)
{

	int red = 0;
	int green = 0;
	int blue = 0;
	ofColor c(red, green, blue, 255);
	return c;
}
string metadataPanel::edgeOrientIntToString(int orientation) {

	if (orientation == 0) return "0";
	else if (orientation == 1) return "|";
	else if (orientation == 2) return "-";
	else if (orientation == 3) return "/";
	else if (orientation == 4) return "\\";
	else if (orientation == 5) return "*";
}


