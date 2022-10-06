#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetWindowPosition(-1920, 25);

	font.loadFont(FONT_PATH_DEFAULT_LEGACY, 10);

	ofxSurfingHelpers::setThemeDark_ofxGui();

	ui.setup();

	setupPatches();

	//--

	listener_NewLink = example.bNewLink.newListener([this](bool b)
		{
			if (b) {
				b = false;

				string msg = "New Link \t " + ofToString(example.lastPinFrom) + " > " + ofToString(example.lastPinTo);

				int iController = -1;
				int iTarget = -1;

				// workaround hardcoded for 4 to 4 elements
				if (example.lastPinFrom >= 2 && example.lastPinFrom <= 5)
				{
					iController = example.lastPinFrom - 2;
				}
				if (example.lastPinTo >= 7 && example.lastPinTo <= 10)
				{
					iTarget = example.lastPinTo - 7;
				}

				if (iController != -1 && iTarget != -1) {
					string msg = "New Link \t " + ofToString(iController) + " > " + ofToString(iTarget);

					patchbay.link(iController, iTarget);

					ui.AddToLog(msg);
					ofLogNotice(__FUNCTION__) << msg;
				}
				else {
					ui.AddToLog("Out of range. Link invalid");
					ofLogError(__FUNCTION__) << "Out of range. Link invalid";
				}
			}
		});

	//--

	listener_RemovedLink = example.bRemovedLink.newListener([this](bool b)
		{
			if (b) {
				b = false;

				string msg = "Removed Link \t " + ofToString(example.lastPinFrom) + " > " + ofToString(example.lastPinTo);

				int iController = -1;
				int iTarget = -1;

				// workaround hardcoded for 4 to 4 elements
				if (example.lastPinFrom >= 2 && example.lastPinFrom <= 5)
				{
					iController = example.lastPinFrom - 2;
				}
				if (example.lastPinTo >= 7 && example.lastPinTo <= 10)
				{
					iTarget = example.lastPinTo - 7;
				}

				if (iController != -1 && iTarget != -1) {
					string msg = "Removed Link \t " + ofToString(iController) + " > " + ofToString(iTarget);

					patchbay.unlink(iController, iTarget);

					ui.AddToLog(msg);
					ofLogNotice(__FUNCTION__) << msg;
				}
				else {
					ui.AddToLog("Out of range. Link invalid");
					ofLogError(__FUNCTION__) << "Out of range. Link invalid";
				}
			}
		});

	//--

	ofAddListener(params.parameterChangedE(), this, &ofApp::Changed_Params); // setup()

}

//--------------------------------------------------------------
void ofApp::update()
{
	updatePatches();
	if (bGenerators) updateGenerators();
}

//--------------------------------------------------------------
void ofApp::updatePatches()
{
	patchbay.update();

	guiControllers.setPosition(10, 10);
	guiTargets.setPosition(ofGetWidth() - 200 - 10, 10);
}

//--------------------------------------------------------------
void ofApp::updateGenerators()
{
	pController0 = ofxSurfingHelpers::Tick(2.0f);
	pController1 = ofxSurfingHelpers::Bounce(3.0f);
	pController2 = ofxSurfingHelpers::Noise(ofPoint(2, -1));
	pController3 = ofxSurfingHelpers::Bounce(0.5f);
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	drawImGui();

	// gui
	guiControllers.draw();
	guiTargets.draw();
}

//--------------------------------------------------------------
void ofApp::drawImGui()
{
	ui.Begin();
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		if (ui.bAutoResize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		if (bOpen0)
		{
			ImGui::Begin("Panels", &bOpen0, window_flags);
			{
				ui.AddToggle("PATCHER", bOpen1, OFX_IM_TOGGLE_ROUNDED_SMALL);
				ui.AddToggle("SCENE", bScene, OFX_IM_TOGGLE_ROUNDED_SMALL);
				ui.AddSpacing();

				if (ui.AddButton("Disconnect All"))
				{
					patchbay.disconnectAll();
				}
				ui.AddToggle("Generators", bGenerators, OFX_IM_TOGGLE_ROUNDED_SMALL);
				ui.Add(ui.bLog, OFX_IM_TOGGLE_ROUNDED);
			}
			ImGui::End();
		}

		//--

		if (bOpen1) drawWidgets();

		/*
		if (ui.bLog)
		{
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
		}
		*/
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	//--

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawWidgets()
{
	if (!initialized) {
		initialized = true;

		//// simple-example
		//g_Context = ed::CreateEditor();

		// basic-interaction-example
		example.Application_Initialize();
	}

	ImGui::Begin("thedmd/imgui-node-editor");
	{
		//// simple-example
		//{
		//	ed::SetCurrentEditor(g_Context);
		//	ed::Begin("My Editor");
		//	int uniqueId = 1;
		//	// Start drawing nodes.
		//	ed::BeginNode(uniqueId++);
		//	ImGui::Text("Node A");
		//	ed::BeginPin(uniqueId++, ed::PinKind::Input);
		//	ImGui::Text("-> In");
		//	ed::EndPin();
		//	ImGui::SameLine();
		//	ed::BeginPin(uniqueId++, ed::PinKind::Output);
		//	ImGui::Text("Out ->");
		//	ed::EndPin();
		//	ed::EndNode();
		//	ed::End();
		//}

		// basic-interaction-example
		example.Application_Frame();
	}
	ImGui::End();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	ofRemoveListener(params.parameterChangedE(), this, &ofApp::Changed_Params); // exit()

	//// simple-example
	//ed::DestroyEditor(g_Context);

	// basic-interaction-example
	example.Application_Finalize();
}

// callback for a parameter group  
//--------------------------------------------------------------
void ofApp::Changed_Params(ofAbstractParameter& e)
{
	string name = e.getName();
	ofLogVerbose() << "Changed parameter named: " << name << " : with value " << e;

	string msg = ofToString(name) + " : " + ofToString(e);

	//ui.AddToLog(msg);

	//if (name == SHOW_gui.getName())
	//{
	//}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		widget.bang();
	}

	keyPressedPatches(key);
}

//--------------------------------------------------------------
void ofApp::setupPatches() {

	// controllers
	gControllers.add(pController0);
	gControllers.add(pController1);
	gControllers.add(pController2);
	gControllers.add(pController3);

	// targets
	gTargets.add(pTarget0);
	gTargets.add(pTarget1);
	gTargets.add(pTarget2);
	gTargets.add(pTarget3);

	// define controllers
	patchbay.addController(pController0);
	patchbay.addController(pController1);
	patchbay.addController(pController2);
	patchbay.addController(pController3);

	// define targets
	patchbay.addTarget(pTarget0);
	patchbay.addTarget(pTarget1);
	patchbay.addTarget(pTarget2);
	patchbay.addTarget(pTarget3);

	patchbay.setupParameters();

	//--

	if (0)
	{
		// connect
		patchbay.link(0, 0);

		patchbay.link(1, 1);
		patchbay.link(2, 2);
		patchbay.link(3, 3);

		str2 = "PRESET\n";
		str2 += "0 -> 0\n";
		str2 += "1 -> 1\n";
		str2 += "2 -> 2\n";
		str2 += "3 -> 3\n";
	}

	//--

	// gui
	guiControllers.setup("CONTROLLERS");
	guiControllers.add(gControllers);
	guiTargets.setup("TARGETS");
	guiTargets.add(gTargets);

	//rect = ofRectangle(0, 0, 100, 100);

	//--

	params.add(gControllers);
	//params.add(gTargets);
	//--

	// widget
	//widget.setName("Patcher");
	widget.setEnableBorder(false);
	widget.setDraggable(false);
	widget.setToggleMode(true);
	widget.toggle(true);
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	ofBackground(128);

	// draw scene
	if (bScene)
	{
		widget.setColor(ofColor(ofColor::red, ofMap(pTarget0, 0, 1, 100, 255)));
		widget.setRadius(ofMap(pTarget1, 0, 1, 100, 500, true));
		float x = ofMap(pTarget2, 0, 1, 0, ofGetWidth(), true);
		float y = ofMap(pTarget3, 0, 1, 0, ofGetWidth(), true);
		int gap = 300;
		x = ofClamp(x, gap, ofGetWidth() - gap);
		y = ofClamp(y, gap, ofGetHeight() - gap);
		widget.setPosition(x, y);
		widget.draw();
	}

	//--

	// help info
	string str1 = "";
	int w, h;
	str1 += "RETURN       : PRINT CONNECTIONS\n";
	str1 += "BACKSPACE    : DISCONNECT ALL\n";
	str1 += "KEYS 1-2-3-4 : PATCHBAY LINKS PRESETS";
	h = ofxSurfingHelpers::getHeightBBtextBoxed(font, str1);
	ofxSurfingHelpers::drawTextBoxed(font, str1, 20, ofGetHeight() - h - 15);

	// patching preset
	w = ofxSurfingHelpers::getWidthBBtextBoxed(font, str2);
	h = ofxSurfingHelpers::getHeightBBtextBoxed(font, str2);
	ofxSurfingHelpers::drawTextBoxed(font, str2, ofGetWidth() - w - 15, ofGetHeight() - h - 15);
}

//--------------------------------------------------------------
void ofApp::keyPressedPatches(int key) {

	// set configurations

	if (key == OF_KEY_RETURN) patchbay.printConnections();

	if (key == OF_KEY_BACKSPACE)
	{
		patchbay.disconnectAll();
		str2 = "PRESET\n";
		str2 += "0    0\n";
		str2 += "1    1\n";
		str2 += "2    2\n";
		str2 += "3    3";

		ui.AddToLog(str2);
	}

	if (key == '1')
	{
		patchbay.disconnectAll();
		patchbay.link(0, 0);
		patchbay.link(1, 1);
		patchbay.link(2, 2);
		patchbay.link(3, 3);

		str2 = "PRESET\n";
		str2 += "0 -> 0\n";
		str2 += "1 -> 1\n";
		str2 += "2 -> 2\n";
		str2 += "3 -> 3";

		ui.AddToLog(str2);
	}
	if (key == '2')
	{
		patchbay.disconnectAll();
		patchbay.link(0, 3);
		patchbay.link(1, 2);
		patchbay.link(2, 1);
		patchbay.link(3, 0);

		str2 = "PRESET\n";
		str2 += "0 -> 3\n";
		str2 += "1 -> 2\n";
		str2 += "2 -> 1\n";
		str2 += "3 -> 0";

		ui.AddToLog(str2);
	}
	if (key == '3')
	{
		patchbay.disconnectAll();
		patchbay.link(0, 3);
		patchbay.link(1, 1);
		patchbay.link(2, 2);
		patchbay.link(3, 0);

		str2 = "PRESET\n";
		str2 += "0 -> 3\n";
		str2 += "1 -> 1\n";
		str2 += "2 -> 2\n";
		str2 += "3 -> 0";

		ui.AddToLog(str2);
	}
	if (key == '4')
	{
		patchbay.disconnectAll();
		patchbay.link(0, 2);
		patchbay.link(1, 3);
		patchbay.link(2, 0);
		patchbay.link(3, 1);

		str2 = "PRESET\n";
		str2 += "0 -> 2\n";
		str2 += "1 -> 3\n";
		str2 += "2 -> 0\n";
		str2 += "3 -> 1";

		ui.AddToLog(str2);
	}
}