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

	// Callbacks

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
					string msg = "New Link \t\t " + ofToString(iController) + " > " + ofToString(iTarget);

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

	guiSources.setPosition(10, 10);
	guiTargets.setPosition(ofGetWidth() - 200 - 10, 10);
}

//--------------------------------------------------------------
void ofApp::updateGenerators()
{
	pSrc0 = ofxSurfingHelpers::Tick(2.0f);
	pSrc1 = ofxSurfingHelpers::Bounce(3.0f);
	pSrc2 = ofxSurfingHelpers::Noise(ofPoint(2, -1));
	pSrc3 = ofxSurfingHelpers::Bounce(0.5f);
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
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		if (ui.bAutoResize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		if (bOpen0)
		{
			ImGui::Begin("Panels", &bOpen0, window_flags);
			{
				ui.AddToggle("PATCHER", bOpen1, OFX_IM_TOGGLE_ROUNDED);
				ui.AddToggle("SCENE", bScene, OFX_IM_TOGGLE_ROUNDED);
				ui.Add(ui.bLog, OFX_IM_TOGGLE_ROUNDED_SMALL);
				ui.AddSpacing();

				if (ui.AddButton("Disconnect All"))
				{
					patchbay.disconnectAll();
					ui.AddToLog("Disconnect All \n");
				}
				ui.AddToggle("Generators", bGenerators, OFX_IM_TOGGLE);
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
void ofApp::keyPressed(int key) 
{
	if (key == ' ') 
	{
		widget.bang();
	}

	keyPressedPatches(key);
}

//--------------------------------------------------------------
void ofApp::setupPatches() {

	// controllers
	gControllers.add(pSrc0);
	gControllers.add(pSrc1);
	gControllers.add(pSrc2);
	gControllers.add(pSrc3);

	// targets
	gTargets.add(pTar0);
	gTargets.add(pTar1);
	gTargets.add(pTar2);
	gTargets.add(pTar3);

	// define controllers
	patchbay.addController(pSrc0);
	patchbay.addController(pSrc1);
	patchbay.addController(pSrc2);
	patchbay.addController(pSrc3);

	// define targets
	patchbay.addTarget(pTar0);
	patchbay.addTarget(pTar1);
	patchbay.addTarget(pTar2);
	patchbay.addTarget(pTar3);

	patchbay.setupParameters();

	//--

	if (0)
	{
		// connect
		patchbay.link(0, 0);

		patchbay.link(1, 1);
		patchbay.link(2, 2);
		patchbay.link(3, 3);

		strInfo = "PRESET\n";
		strInfo += "0 -> 0\n";
		strInfo += "1 -> 1\n";
		strInfo += "2 -> 2\n";
		strInfo += "3 -> 3\n";
	}

	//--

	// gui
	guiSources.setup("SOURCES");
	guiSources.add(gControllers);
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
		widget.setColor(ofColor(color, ofMap(pTar0, 0, 1, 100, 255)));
		float r = ofMap(pTar1, 0, 1, 100, 500, true);
		widget.setRadius(r);
		float x = ofMap(pTar2, 0, 1, 0, ofGetWidth(), true);
		float y = ofMap(pTar3, 0, 1, 0, ofGetHeight(), true);
		int gap = r + 5;
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
	w = ofxSurfingHelpers::getWidthBBtextBoxed(font, strInfo);
	h = ofxSurfingHelpers::getHeightBBtextBoxed(font, strInfo);
	ofxSurfingHelpers::drawTextBoxed(font, strInfo, ofGetWidth() - w - 15, ofGetHeight() - h - 30);
}

//--------------------------------------------------------------
void ofApp::keyPressedPatches(int key) {

	// set configurations

	if (key == OF_KEY_RETURN) {
		patchbay.printConnections();
		
		ui.AddToLog(patchbay.getConnections());
	}

	if (key == OF_KEY_BACKSPACE)
	{
		color = ofColor::white;
		
		patchbay.disconnectAll();

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

		strInfo = "\nPRESET 4 \n";
		strInfo += "0 -> 2\n";
		strInfo += "1 -> 3\n";
		strInfo += "2 -> 0\n";
		strInfo += "3 -> 1\n";

		ui.AddToLog(strInfo);
	}
}