#pragma once

#include "ofMain.h"

typedef struct
{
	string name; 
	double latitude;
	double longitude;
} City;

typedef struct
{
	float height;
	double latitude;
	double longitude;
} Cover;

typedef struct
{
	bool isBigEndian = true;
	int totalRows;
	int totalCols;
	int mask;
	double longxmap;
	double latymap;
	double xdim;
	double ydim;
} DemHeader;

typedef struct
{
	string demFileName;
	string headerFileName;
	int latmin;
	int latmax;
	int longmin;
	int longmax;
	int elevationmin;
	int elevationmax;
} DemFileInfo;

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
	void initCities();
	void initGeoData();
	void initDemFiles();

	void readHeader(string& path, DemFileInfo& fileInfo, DemHeader& headerInfo);
	double readParameterD(string& source, const string& name);
	string readParameterS(string& source, const string& name);
	int readParameterI(string& source, const string& name);

	void readGeoData(string& path, DemFileInfo& fileInfo, DemHeader& headerInfo);

private:
	vector<Cover> dots;
	vector<City> cities;
	vector<DemFileInfo> fileInfos;

	const static string DATA_PATH;
	const static string EMPTY_STRING;
	const static float SPHERE_RADIUS;

	float minHeight;
	float maxHeight;
};
