#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetWindowPosition(-1920, 25);

	setupParams();

	setupPatches();

	//TODO: workaround
	setupCallbacks();
}

//--------------------------------------------------------------
void ofApp::setupParams() {

	font.loadFont(FONT_PATH_DEFAULT_LEGACY, 10);

	ofxSurfingHelpers::setThemeDark_ofxGui();

	ui.setup();

	// Controllers
	gControllers.add(pSrc0);
	gControllers.add(pSrc1);
	gControllers.add(pSrc2);
	gControllers.add(pSrc3);
	gControllers.add(pSrc4b);
	gControllers.add(pSrc5b);

	// Targets
	gTargets.add(pTar0);
	gTargets.add(pTar1);
	gTargets.add(pTar2);
	gTargets.add(pTar3);
	gTargets.add(pTar4b);
	gTargets.add(pTar5b);

	//--

	// Gui
	guiSources.setup("SOURCES");
	guiSources.add(gControllers);
	guiTargets.setup("TARGETS");
	guiTargets.add(gTargets);

	//--

	// Scene Widget
	widget.setEnableBorder(false);
	widget.setDraggable(false);
	widget.setToggleMode(true);
	widget.toggle(true);
	widget.setPosition(ofGetWidth() / 2, ofGetHeight() / 2);
}

//--------------------------------------------------------------
void ofApp::setupPatches() {

	//--

	// Define controllers
	patchbay.addController(pSrc0);
	patchbay.addController(pSrc1);
	patchbay.addController(pSrc2);
	patchbay.addController(pSrc3);
	patchbay.addController(pSrc4b);
	patchbay.addController(pSrc5b);

	// Define targets
	patchbay.addTarget(pTar0);
	patchbay.addTarget(pTar1);
	patchbay.addTarget(pTar2);
	patchbay.addTarget(pTar3);
	patchbay.addTarget(pTar4b);
	patchbay.addTarget(pTar5b);

	// Setup to register
	patchbay.setupRegister();

	//--

	// Make some patching
	// using the previously added 
	// controllers and/to targets
	/*
	if (0)
	{
		// connect
		patchbay.link(0, 0);
		patchbay.link(1, 1);
		patchbay.link(2, 2);
		patchbay.link(3, 3);

		strInfo = "PRESET \n";
		strInfo += "0 -> 0\n";
		strInfo += "1 -> 1\n";
		strInfo += "2 -> 2\n";
		strInfo += "3 -> 3\n";
	}
	*/
}

//--------------------------------------------------------------
void ofApp::startup()
{
	load(path);
}

//--------------------------------------------------------------
void ofApp::setupCallbacks()
{
	//TODO: workaround
	// modified patch links into the ImGui node editor,
	// are reflected here!

	//--
	// 
	// CREATE LINK

	listener_NewLink = patcher.bNewLink.newListener([this](bool b)
		{
			if (b) {
				b = false;

				//string msg = "New Link \t " + ofToString(patcher.lastPinFrom) + " > " + ofToString(patcher.lastPinTo);

				int iSrc = -1;
				int iTar = -1;

				//TODO: WIP
				// workaround hardcoded for 4 to 4 elements
				if (patcher.lastPinFrom >= 2 && patcher.lastPinFrom <= 5)
				{
					iSrc = patcher.lastPinFrom - 2;
				}
				if (patcher.lastPinTo >= 7 && patcher.lastPinTo <= 10)
				{
					iTar = patcher.lastPinTo - 7;
				}

				if (iSrc != -1 && iTar != -1)
				{
					patchbay.link(iSrc, iTar);
					linkToJson(iSrc, iTar);

					string msg = "New Link \t\t " + ofToString(iSrc) + " > " + ofToString(iTar);
					ui.AddToLog(msg);
					ofLogNotice(__FUNCTION__) << msg;
				}
				else
				{
					ui.AddToLog("Out of range. Link invalid");
					ofLogError(__FUNCTION__) << "Out of range. Link invalid";
				}
			}
		});

	//--

	//TODO: WIP notice that only one link can be deleted at time,
	//or jsno will be broken...

	// REMOVE LINK

	listener_RemovedLink = patcher.bRemovedLink.newListener([this](bool b)
		{
			if (b) {
				b = false;

				//string msg = "Removed Link \t " + ofToString(patcher.lastPinFrom) + " > " + ofToString(patcher.lastPinTo);

				int iSrc = -1;
				int iTar = -1;

				// workaround hardcoded for 4 to 4 elements
				if (patcher.lastPinFrom >= 2 && patcher.lastPinFrom <= 5)
				{
					iSrc = patcher.lastPinFrom - 2;
				}

				if (patcher.lastPinTo >= 7 && patcher.lastPinTo <= 10)
				{
					iTar = patcher.lastPinTo - 7;
				}

				if (iSrc != -1 && iTar != -1)
				{
					patchbay.unlink(iSrc, iTar);
					unlinkFromJson(iSrc, iTar);

					string msg = "Removed Link \t " + ofToString(iSrc) + " > " + ofToString(iTar);
					ui.AddToLog(msg);
					ofLogNotice(__FUNCTION__) << msg;
				}
				else
				{
					ui.AddToLog("Out of range. Link invalid");
					ofLogError(__FUNCTION__) << "Out of range. Link invalid";
				}
			}
		});
}

//--------------------------------------------------------------
void ofApp::unlinkFromJson(int i, int o)
{
	int n = -1;
	int _n = 0;
	for (auto& j : js)
	{
		if (!j.empty())
		{
			int _i = j["source"];
			int _o = j["target"];
			if (i == _i && o == _o) //same link
			{
				n = _n;
			}
			_n++;
		}
	}

	if (n != -1) {
		js.erase(n);
	}
}

//--------------------------------------------------------------
void ofApp::linkToJson(int i, int o)
{
	ofJson j;
	j["source"] = i;
	j["target"] = o;
	js.push_back(j);
}

//--------------------------------------------------------------
void ofApp::load(string path)
{
	ofFile file(path);
	if (file.exists())
	{
		file >> js;

		patchbay.disconnectAll();
		patcher.clearLinks();

		for (auto& j : js) {
			if (!j.empty())
			{
				int i = j["source"];
				int o = j["target"];

				patchbay.link(i, o);
				patcher.link(i, o);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::save(string path)
{
	ofSavePrettyJson(path, js);
}

//--

//--------------------------------------------------------------
void ofApp::update()
{
	if (ofGetFrameNum() == 10) startup();

	if (bGenerators) updateGenerators();

	updatePatches();
}

//--------------------------------------------------------------
void ofApp::updatePatches()
{
	patchbay.update();

	int pad = 15;
	int h;
	int w;
	h = guiSources.getHeight();
	guiSources.setPosition(pad, ofGetHeight() / 2 - h / 2);
	h = guiTargets.getHeight();
	w = guiTargets.getWidth();
	guiTargets.setPosition(ofGetWidth() - w - pad, ofGetHeight() / 2 - h / 2);
}

//--------------------------------------------------------------
void ofApp::updateGenerators()
{
	pSrc0 = ofxSurfingHelpers::Tick(2.0f);
	pSrc1 = ofxSurfingHelpers::Bounce(3.0f);
	pSrc2 = ofxSurfingHelpers::Noise(ofPoint(2, -1));
	pSrc3 = ofxSurfingHelpers::Bounce(0.5f);
}

//--

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	drawImGui();

	// gui
	guiSources.draw();
	guiTargets.draw();
}

//--------------------------------------------------------------
void ofApp::drawImGui()
{
	ui.Begin();
	{
		if (bGui)
		{
			if (ui.BeginWindow(bGui))
			{
				auto& io = ImGui::GetIO();
				ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
				ImGui::Separator();

				ui.Add(bGui_Patcher, OFX_IM_TOGGLE_ROUNDED);
				ui.Add(bScene, OFX_IM_TOGGLE_ROUNDED);
				ui.Add(ui.bLog, OFX_IM_TOGGLE_ROUNDED);
				ui.AddSpacing();
				if (ui.AddButton("Disconnect All"))
				{
					patchbay.disconnectAll();
					patcher.clearLinks();
					js.clear();

					ui.AddToLog("Disconnect All \n");
				}
				ui.Add(bGenerators, OFX_IM_TOGGLE_BORDER_BLINK);
				ui.EndWindow();
			}
		}

		drawPatches();
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::drawPatches()
{
	if (!bGui_Patcher) return;

	if (!initialized) {
		initialized = true;
		patcher.OnSetup();
	}

	/*
	if (bGui_Patcher) {
		ImVec2 size_min(400, 400);
		ImVec2 size_max(1000, 1000);
		ImGui::SetNextWindowSizeConstraints(size_min, size_max);
	}
	*/

	if (bGui_Patcher) {
		ImGuiCond cond = ImGuiCond_FirstUseEver;
		ImVec2 sz(600, 300);
		ImVec2 pos(ofGetWidth() / 2 - sz.x / 2, 10);
		ImGui::SetNextWindowPos(pos, cond);
		ImGui::SetNextWindowSize(sz, cond);
	}

	ImGuiWindowFlags window_flags;
	window_flags = ImGuiWindowFlags_None;
	if (ui.BeginWindow(bGui_Patcher, window_flags))
	{
		patcher.OnDraw();

		ui.EndWindow();
	}
}

//--

//--------------------------------------------------------------
void ofApp::exit()
{
	patcher.OnExit();

	save(path);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == OF_KEY_F1)
	{
		ui.AddToLog(patcher.getListLinks());
	}

	keyPressedPatches(key);
}

//--------------------------------------------------------------
void ofApp::keyPressedPatches(int key) {

	// set configurations

	if (key == OF_KEY_RETURN) {
		patchbay.printConnections();

		//ui.AddToLog(patchbay.getConnections());
	}

	// Disconnect all
	if (key == OF_KEY_BACKSPACE)
	{
		color = ofColor::white;

		patchbay.disconnectAll();
		patcher.clearLinks();
		js.clear();

		strInfo = "\nNO PRESET \n";
		strInfo += "0    0\n";
		strInfo += "1    1\n";
		strInfo += "2    2\n";
		strInfo += "3    3\n";

		ui.AddToLog(strInfo);
	}

	if (key == '1')
	{
		color = ofColor::red;

		patchbay.disconnectAll();
		patchbay.link(0, 0);
		patchbay.link(1, 1);
		patchbay.link(2, 2);
		patchbay.link(3, 3);

		patcher.clearLinks();
		patcher.link(0, 0);
		patcher.link(1, 1);
		patcher.link(2, 2);
		patcher.link(3, 3);

		js.clear();
		linkToJson(0, 0);
		linkToJson(1, 1);
		linkToJson(2, 2);
		linkToJson(3, 3);

		strInfo = "\nPRESET 1 \n";
		strInfo += "0 -> 0\n";
		strInfo += "1 -> 1\n";
		strInfo += "2 -> 2\n";
		strInfo += "3 -> 3\n";

		ui.AddToLog(strInfo);
	}

	if (key == '2')
	{
		color = ofColor::green;

		patchbay.disconnectAll();
		patchbay.link(0, 3);
		patchbay.link(1, 2);
		patchbay.link(2, 1);
		patchbay.link(3, 0);

		patcher.clearLinks();
		patcher.link(0, 3);
		patcher.link(1, 2);
		patcher.link(2, 1);
		patcher.link(3, 0);

		js.clear();
		linkToJson(0, 3);
		linkToJson(1, 2);
		linkToJson(2, 1);
		linkToJson(3, 0);

		strInfo = "\nPRESET 2 \n";
		strInfo += "0 -> 3\n";
		strInfo += "1 -> 2\n";
		strInfo += "2 -> 1\n";
		strInfo += "3 -> 0\n";

		ui.AddToLog(strInfo);
	}

	if (key == '3')
	{
		color = ofColor::blue;

		patchbay.disconnectAll();
		patchbay.link(0, 3);
		patchbay.link(1, 1);
		patchbay.link(2, 2);
		patchbay.link(3, 0);

		patcher.clearLinks();
		patcher.link(0, 3);
		patcher.link(1, 1);
		patcher.link(2, 2);
		patcher.link(3, 0);

		js.clear();
		linkToJson(0, 3);
		linkToJson(1, 1);
		linkToJson(2, 2);
		linkToJson(3, 0);

		strInfo = "\nPRESET 3 \n";
		strInfo += "0 -> 3\n";
		strInfo += "1 -> 1\n";
		strInfo += "2 -> 2\n";
		strInfo += "3 -> 0\n";

		ui.AddToLog(strInfo);
	}

	if (key == '4')
	{
		color = ofColor::yellow;

		patchbay.disconnectAll();
		patchbay.link(0, 2);
		patchbay.link(1, 3);
		patchbay.link(2, 0);
		patchbay.link(3, 1);

		patcher.clearLinks();
		patcher.link(0, 2);
		patcher.link(1, 3);
		patcher.link(2, 0);
		patcher.link(3, 1);

		js.clear();
		linkToJson(0, 2);
		linkToJson(1, 3);
		linkToJson(2, 0);
		linkToJson(3, 1);

		strInfo = "\nPRESET 4 \n";
		strInfo += "0 -> 2\n";
		strInfo += "1 -> 3\n";
		strInfo += "2 -> 0\n";
		strInfo += "3 -> 1\n";

		ui.AddToLog(strInfo);
	}
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	ofBackground(128);

	// Scene Widget
	if (bScene)
	{
		int g = 300;
		float r = ofMap(pTar1, 0, 1, 100, 500, true);
		widget.setRadius(r);
		widget.setColor(ofColor(color, ofMap(pTar0, 0, 1, 100, 255)));
		float x = ofMap(pTar2, 0, 1, -g, g, true);
		float y = ofMap(pTar3, 0, 1, -g, g, true);
		ofPushMatrix();
		ofTranslate(x, y);
		widget.draw();
		ofPopMatrix();
	}

	//--

	// Help info
	string str1 = "";
	int w, h;
	str1 += "RETURN       : PRINT CONNECTIONS\n";
	str1 += "BACKSPACE    : DISCONNECT ALL\n";
	str1 += "KEYS 1-2-3-4 : PRESETS";
	h = ofxSurfingHelpers::getHeightBBtextBoxed(font, str1);
	ofxSurfingHelpers::drawTextBoxed(font, str1, 20, ofGetHeight() - h - 15);

	// Patching Preset
	w = ofxSurfingHelpers::getWidthBBtextBoxed(font, strInfo);
	h = ofxSurfingHelpers::getHeightBBtextBoxed(font, strInfo);
	ofxSurfingHelpers::drawTextBoxed(font, strInfo, ofGetWidth() - w - 15, ofGetHeight() - h - 30);
}