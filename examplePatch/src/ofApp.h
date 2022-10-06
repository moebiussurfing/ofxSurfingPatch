#pragma once

#include "ofMain.h"

#include "ofxSurfingImGui.h"	
#include "ofxSurfing_ofxGui.h"	

#include "ofxSurfingPatch.h" // -> here is the patching code

#include "ofxPatchbay.h"
#include "ofxGui.h"
#include "CircleBeat.h"

/*

	+	add plots
	+	add pins during runtime

*/


class ofApp : public ofBaseApp 
{

public:

	void setup();
	void update();
	void updateGenerators();
	void draw();
	void exit();
	void keyPressed(int key);
	
	void drawGui();
	void drawImGui();

	ofxSurfingGui ui;
	
	Example example;

	void drawWidgets();
	
	CircleBeat widget;

	bool bOpen0 = true;
	bool bOpen1 = true;
	bool bScene = true;
	bool bGenerators = true;

	bool initialized = false;

	ofEventListener listener_NewLink;
	ofEventListener listener_RemovedLink;

	void Changed_Params(ofAbstractParameter &e);
	ofParameterGroup params{ "Params" };

	//--
	
	void setupPatches();
	void updatePatches();
	void drawScene();
	void keyPressedPatches(int key);

	ofxPatchbay patchbay;

	// controllers
	ofParameter<float>pSrc0{ "pSrc0", 0, 0, 1 };
	ofParameter<float>pSrc1{ "pSrc1", 0, 0, 1 };
	ofParameter<float>pSrc2{ "pSrc2", 0, 0, 1 };
	ofParameter<float>pSrc3{ "pSrc3", 0, 0, 1 };
	ofParameterGroup gControllers{ "gSources" };

	// targets
	ofParameter<float>pTar0{ "pTar0", 0, 0, 1 };
	ofParameter<float>pTar1{ "pTar1", 0, 0, 1 };
	ofParameter<float>pTar2{ "pTar2", 0, 0, 1 };
	ofParameter<float>pTar3{ "pTar3", 0, 0, 1 };
	ofParameterGroup gTargets{ "gTargets" };

	// gui
	ofxPanel guiSources;
	ofxPanel guiTargets;

	string strInfo = "\nNO PRESET \n";

	// scene
	ofColor color = ofColor::white;
	ofTrueTypeFont font;
};
