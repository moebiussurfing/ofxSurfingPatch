#pragma once

#include "ofMain.h"
#include "NodePatcher.h"

class ofxSurfingPatch : public ofBaseApp
{

public:

	ofxSurfingPatch();
	~ofxSurfingPatch();

	void setup();
	void startup();
	void update();
	void draw();
	void exit();
};