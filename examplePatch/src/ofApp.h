#pragma once

#include "ofMain.h"

#include "ofxSurfingPatch.h" // -> here is the patching code
#include "ofxPatchbay.h"

#include "ofxSurfingImGui.h"	
#include "ofxGui.h"
#include "ofxSurfing_ofxGui.h"	
#include "CircleBeat.h"

/*
	+ add pins sources/params during runtime
	+ add wires during runtime
	+ persistent data
	+ presets
*/


class ofApp : public ofBaseApp 
{

public:

	void setup();
	void startup();
	void setupCallbacks();
	void update();
	void updateGenerators();
	void draw();
	void exit();
	void keyPressed(int key);
	
	void drawGui();
	void drawImGui();

	ofxSurfingGui ui;

	ofParameter<bool> bGui{ "Gui",true };
	ofParameter<bool> bGui_Patcher{ "thedmd",true };
	ofParameter<bool> bScene{ "Scene",true };
	ofParameter<bool> bGenerators{ "Generators",true };

	void Changed_Params(ofAbstractParameter &e);
	ofParameterGroup params{ "Params" };
	
	void drawScene();
	CircleBeat widget;
	
	NodePatcher patcher;

	void drawPatcher();

	bool initialized = false;

	ofEventListener listener_NewLink;
	ofEventListener listener_RemovedLink;

	//--
	
	void setupPatches();
	void updatePatches();
	void keyPressedPatches(int key);

	// Params Patcher
	ofxPatchbay patchbay;

	// Sources
	ofParameter<float>pSrc0{ "pSrc0", 0, 0, 1 };
	ofParameter<float>pSrc1{ "pSrc1", 0, 0, 1 };
	ofParameter<float>pSrc2{ "pSrc2", 0, 0, 1 };
	ofParameter<float>pSrc3{ "pSrc3", 0, 0, 1 };
	ofParameterGroup gControllers{ "gSources" };

	// Targets
	ofParameter<float>pTar0{ "pTar0", 0, 0, 1 };
	ofParameter<float>pTar1{ "pTar1", 0, 0, 1 };
	ofParameter<float>pTar2{ "pTar2", 0, 0, 1 };
	ofParameter<float>pTar3{ "pTar3", 0, 0, 1 };
	ofParameterGroup gTargets{ "gTargets" };

	// Gui
	ofxPanel guiSources;
	ofxPanel guiTargets;

	// Scene
	ofColor color = ofColor::white;
	ofTrueTypeFont font;

	string strInfo = "\nNO PRESET \n";

	// Serialization
	string path = "patching.json";
	ofJson js;
	void load(string path);
	void save(string path);
	void linkToJson(int i, int o);
	void unlinkFromJson(int i, int o);
};
