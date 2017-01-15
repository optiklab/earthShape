/**
*
* OFDevCon Example Code Sprint
* Quaternion Example for plotting latitude and longitude onto a sphere
*
* Created by James George on 2/23/2012
*/

#include "ofApp.h"

 //----------------------- Constants -------------------------

const string ofApp::DATA_PATH = "geodata\\";
const string ofApp::EMPTY_STRING = "";
const float ofApp::SPHERE_RADIUS = 250.0;

void ofApp::initCities()
{
	// create little objects for each city. 
	// A Lat/Lon like this: 
	// Lewiston, Idaho 	W 46 24' N 117 2'
	// This translate to angles and degrees (1 degree = 1/60. of an angle)
	// West and South are negative values

	// here is a list of big cities and their positions
	// http://www.infoplease.com/ipa/A0001796.html

	City newyork = { "new york", 40+47/60., -73 + 58/60. };
	City tokyo = { "tokyo", 35 + 40./60, 139 + 45/60. };
	City london = { "london", 51 + 32/60., -5./60. };
	City shanghai = { "shanghai", 31 + 10/60., 121 + 28/60. };
	City buenosaires = { "buenos aires", -34 + 35/60., -58 + 22/60. };
	City melbourne = { "melbourne" , -37 + 47/60., 144 + 58/60. };	
	City detroit = { "detroit", 42 + 19/60., -83 + 2 / 60. };

	cities.push_back( newyork );
	cities.push_back( tokyo );
	cities.push_back( london );
	cities.push_back( shanghai );
	cities.push_back( buenosaires );
	cities.push_back( melbourne );
	cities.push_back( detroit );
}

void ofApp::initDemFiles()
{
	fileInfos.push_back({ "E020N40.DEM", "E020N40.HDR", -10, 40, 20, 60, -415, 5778 });
	fileInfos.push_back({ "E020N90.DEM", "E020N90.HDR", 40, 90, 20, 60, -188, 5472 });
	fileInfos.push_back({ "E020S10.DEM", "E020S10.HDR", -60, -10, 20, 60, -1, 3408 });
	fileInfos.push_back({ "E060N40.DEM", "E060N40.HDR", -10, 40, 60, 100, -46, 8685 });
	fileInfos.push_back({ "E060N90.DEM", "E060N90.HDR", 40, 90, 60, 100, -156, 7169 });
	fileInfos.push_back({ "E060S10.DEM", "E060S10.HDR", -60, -10, 60, 100, -4, 2555 });
	fileInfos.push_back({ "E100N40.DEM", "E100N40.HDR", -10, 40, 100, 140, -71, 7213 });
	fileInfos.push_back({ "E100N90.DEM", "E100N90.HDR", 40, 90, 100, 140, -110, 3901 });
	fileInfos.push_back({ "E100S10.DEM", "E100S10.HDR", -60, -10, 100, 140, -20, 1360 });
	fileInfos.push_back({ "E140N40.DEM", "E140N40.HDR", -10, 40, 140, 180, -6, 4650 });
	fileInfos.push_back({ "E140N90.DEM", "E140N90.HDR", 40, 90, 140, 180, -26, 4578 });
	fileInfos.push_back({ "E140S10.DEM", "E140S10.HDR", -60, -10, 140, 180, -43, 3119 });
	fileInfos.push_back({ "W020N40.DEM", "W020N40.HDR", -10, 40, -20, 20, -139, 3958 });
	fileInfos.push_back({ "W020N90.DEM", "W020N90.HDR", 40, 90, -20, 20, -179, 4536 });
	fileInfos.push_back({ "W020S10.DEM", "W020S10.HDR", -60, -10, -20, 20, -12, 2498 });
	fileInfos.push_back({ "W060N40.DEM", "W060N40.HDR", -10, 40, -60, -20, -10, 2503 });
	fileInfos.push_back({ "W060N90.DEM", "W060N90.HDR", 40, 90, -60, -20, -14, 3940 });
	fileInfos.push_back({ "W060S10.DEM", "W060S10.HDR", -60, -10, -60, -20, -61, 2823 });
	fileInfos.push_back({ "W100N40.DEM", "W100N40.HDR", -10, 40, -100, -60, -42, 6543 });
	fileInfos.push_back({ "W100N90.DEM", "W100N90.HDR", 40, 90, -100, -60, -18, 2416 });
	fileInfos.push_back({ "W100S10.DEM", "W100S10.HDR", -60, -10, -100, -60, -206, 6813 });
	fileInfos.push_back({ "W140N40.DEM", "W140N40.HDR", -10, 40, -140, -100, -83, 4228 });
	fileInfos.push_back({ "W140N90.DEM", "W140N90.HDR", 40, 90, -140, -100, -71, 4635 });
	fileInfos.push_back({ "W140S10.DEM", "W140S10.HDR", -60, -10, -140, -100, 0, 910 });
	fileInfos.push_back({ "W180N40.DEM", "W180N40.HDR", -10, 40, -180, -140, -2, 4120 });
	fileInfos.push_back({ "W180N90.DEM", "W180N90.HDR", 40, 90, -180, -140, -6, 6098 });
	fileInfos.push_back({ "W180S10.DEM", "W180S10.HDR", -60, -10, -180, -140, 0, 1784 });
}

void ofApp::initGeoData()
{
	std::ios::sync_with_stdio(false);

	for (int i = 0; i < 27; i++)
	{
		DemFileInfo info = fileInfos[i];

		string dataFilename = DATA_PATH + info.demFileName;
		string headerFilename = DATA_PATH + info.headerFileName;

		DemHeader headerInfo = {};
		readHeader(headerFilename, info, headerInfo);

		readGeoData(dataFilename, info, headerInfo);
	}
}

void ofApp::readGeoData(string& path, DemFileInfo& fileInfo, DemHeader& headerInfo)
{
	std::fstream demf(path, std::ios_base::in | std::ios_base::binary);

	if (!demf.is_open())
	{
		return;
	}

	int16_t *tmp = new int16_t[headerInfo.totalRows * headerInfo.totalCols];
	demf.read((char*)tmp, headerInfo.totalRows * headerInfo.totalCols * 2);
	demf.close();

	if (headerInfo.isBigEndian)
	{
		unsigned char*c = (unsigned char*)tmp;
		for (int j = 0; j < headerInfo.totalRows * headerInfo.totalCols * 2; j += 2)
		{
			swap(c[j], c[j + 1]);
		}
	}

	int counter = 0;

	int rowsCounter = 0, colsCounter = 0;

	int16_t *reading = tmp;

	for (int longitude = fileInfo.longmin; longitude <= fileInfo.longmax; longitude++)
	{
		for (int latitude = fileInfo.latmax; latitude >= fileInfo.latmin; latitude--)
		{
			int x = (longitude - headerInfo.longxmap) / headerInfo.xdim;
			int y = (headerInfo.latymap - latitude) / headerInfo.ydim;
			int filePos = (y * headerInfo.totalCols) + x;

			int16_t height = *(reading + filePos);

			if (height != 0)
			{
				Cover cover = { height, latitude, longitude };

				dots.push_back(cover);
			}
		}
	}

	delete tmp;
}

void ofApp::readHeader(string& path, DemFileInfo& fileInfo, DemHeader& headerInfo)
{
	cout << "###### " << path << " ######" << endl;

	std::fstream hdrf(path, std::ios_base::in);

	if (!hdrf.is_open())
	{
		return;
	}

	std::string config = EMPTY_STRING;
	std::string line = EMPTY_STRING;
	while (std::getline(hdrf, line))
	{
		if (line == EMPTY_STRING)
			continue;

		config += line + "\r\n";
	}

	headerInfo.isBigEndian = (readParameterS(config, "BYTEORDER") == "M");
	headerInfo.totalRows = readParameterI(config, "NROWS");
	headerInfo.totalCols = readParameterI(config, "NCOLS");
	headerInfo.mask = readParameterI(config, "NODATA");
	headerInfo.longxmap = readParameterD(config, "ULXMAP");
	headerInfo.latymap = readParameterD(config, "ULYMAP");
	headerInfo.xdim = readParameterD(config, "XDIM");
	headerInfo.ydim = readParameterD(config, "YDIM");

	hdrf.close();

	if (!headerInfo.isBigEndian)
	{
		cout << "!!!! isBigEndian = false" << endl;
	}

	cout << "###### *************************** ######" << endl;
}

double ofApp::readParameterD(string& source, const string& name)
{
	return std::stod(readParameterS(source, name));
}

int ofApp::readParameterI(string& source, const string& name)
{
	return std::stoi(readParameterS(source, name));
}

string ofApp::readParameterS(string& source, const string& name)
{
	size_t next = source.find(name);
	size_t length = name.length();

	string result = EMPTY_STRING;

	if (next != string::npos)
	{
		size_t nextRowIndex = source.find('\r', next);
		string value = source.substr(next + length, nextRowIndex - (next + length));
		result = ofTrim(value);
	}

	cout << name << " = " << result << endl;

	return result;
}

void ofApp::setup(){
	
	ofSetFrameRate(30);
	ofEnableAlphaBlending();
	ofNoFill();

	initCities();
	initDemFiles();
	initGeoData();
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	
	ofPushMatrix();
	
	//translate so that the center of the screen is 0,0
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	ofSetColor(255, 255, 255, 20);
	
	//draw a translucent wireframe sphere (ofNoFill() is on)
	ofPushMatrix();
	//add an extra spin at the rate of 1 degree per frame
	ofRotate(ofGetFrameNum(), 0, 1, 0);
	ofDrawSphere(0, 0, 0, SPHERE_RADIUS);
	ofPopMatrix();
	
	ofSetColor(255);

	maxHeight = 8685;
	minHeight = -415;

	int heightRange = maxHeight - minHeight;
	int colorStep = heightRange / 255;

	for(unsigned int i = 0; i < dots.size(); i++)
	{
		ofQuaternion latRot, longRot, spinQuat;
		latRot.makeRotate(dots[i].latitude, 1, 0, 0);
		longRot.makeRotate(dots[i].longitude, 0, 1, 0);
		spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);

		int colorSet = dots[i].height / colorStep;
		ofSetColor(colorSet + 100, 255, 255);

		if (dots[i].height > 0)
		{
			ofSetColor(237, 28, 28);
		}

		//float z = SPHERE_RADIUS + (dots[i].height / 21260); // Earth Radius (6378000 m) / Sphere Radius SPHERE_RADIUSpx = 21260
		float z = SPHERE_RADIUS + (dots[i].height / 1000); // Earth Radius (6378000 m) / Sphere Radius SPHERE_RADIUSpx = 21260
		ofVec3f worldPoint = latRot * longRot * spinQuat * ofVec3f(1, 0, SPHERE_RADIUS);
		ofVec3f worldPoint1 = latRot * longRot * spinQuat * ofVec3f(2, 0, z);

		ofDrawLine(worldPoint1, worldPoint);

		ofSetColor(255);
	}

	for (unsigned int i = 0; i < cities.size(); i++)
	{
		ofQuaternion latRot, longRot, spinQuat;
		latRot.makeRotate(cities[i].latitude, 1, 0, 0);
		longRot.makeRotate(cities[i].longitude, 0, 1, 0);
		spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);

		ofVec3f center = ofVec3f(0, 0, SPHERE_RADIUS);
		ofVec3f worldPoint = latRot * longRot * spinQuat * center;
		ofVec3f worldPoint1 = latRot * longRot * spinQuat * ofVec3f(0, 0, 299);

		ofDrawLine(ofVec3f(0, 0, 0), worldPoint);

		ofDrawBitmapString(cities[i].name, worldPoint);
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
