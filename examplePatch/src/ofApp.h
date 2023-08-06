#pragma once

#include "ofMain.h"

#include "ofxSurfingPatch.h"
#include "ofxPatchbay.h"

#include "ofxSurfingImGui.h"	
#include "ofxGui.h"
#include "ofxSurfing_ofxGui.h"	
#include "CircleBeat.h"
#include "ofxSurfing_Widgets.h"

class ofApp : public ofBaseApp
{

public:

	void setup();
	void startup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);

	void setupParams();
	void setupCallbacks();
	void updateGenerators();
	void drawGui();
	void drawImGui();

	ofxSurfingGui ui;

	ofParameter<bool> bGui{ "Gui",true };
	ofParameter<bool> bGui_Patcher{ "SURFING PATCH",true };
	ofParameter<bool> bScene{ "Scene",true };
	ofParameter<bool> bGenerators{ "Generators",true };

	void drawScene();
	CircleBeat widget;

	//--

	NodePatcher patcher;

	void setupPatches();
	void updatePatches();
	void drawPatches();
	void keyPressedPatches(int key);

	bool initialized = false;

	ofEventListener listener_NewLink;
	ofEventListener listener_RemovedLink;

	// Params Patcher
	ofxPatchbay patchbay;

	//--

	// Patching Serialization
	string path = "patching.json";
	ofJson js;
	void load(string path);
	void save(string path);
	void linkToJson(int i, int o);
	void unlinkFromJson(int i, int o);

	//--

	// Sources
	ofParameter<float>pSrc0{ "pSrc0", 0, 0, 1 };
	ofParameter<float>pSrc1{ "pSrc1", 0, 0, 1 };
	ofParameter<float>pSrc2{ "pSrc2", 0, 0, 1 };
	ofParameter<float>pSrc3{ "pSrc3", 0, 0, 1 };
	ofParameter<bool>pSrc4b{ "pSrc4", false };
	ofParameter<bool>pSrc5b{ "pSrc5", false };
	ofParameterGroup gControllers{ "gSources" };

	// Targets
	ofParameter<float>pTar0{ "pTar0", 0, 0, 1 };
	ofParameter<float>pTar1{ "pTar1", 0, 0, 1 };
	ofParameter<float>pTar2{ "pTar2", 0, 0, 1 };
	ofParameter<float>pTar3{ "pTar3", 0, 0, 1 };
	ofParameter<bool>pTar4b{ "pTar4", false };
	ofParameter<bool>pTar5b{ "pTar5", false };
	ofParameterGroup gTargets{ "gTargets" };

	// Gui
	ofxPanel guiSources;
	ofxPanel guiTargets;

	// Scene
	ofColor color = ofColor::white;
	ofTrueTypeFont font;
	string strInfo = "\nNO PRESET \n";
};
