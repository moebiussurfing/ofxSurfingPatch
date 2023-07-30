#pragma once

#include "ofMain.h"

# define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
# include <imgui_node_editor.h>
# include <imgui_internal.h>

namespace ed = ax::NodeEditor;

//--

// Struct to hold basic information about connection between
// pins. Note that connection (aka. link) has its own ID.
// This is useful later with dealing with selections, deletion
// or other operations.
struct LinkInfo
{
	ed::LinkId Id;
	ed::PinId  InputId;
	ed::PinId  OutputId;
};

//--

static ed::EditorContext* g_Context = nullptr;
// Editor context, required to trace a editor state.

static bool g_FirstFrame = true;
// Flag set for first frame only, some action need to be executed once.

static ImVector<LinkInfo> g_Links;
// List of live links. It is dynamic unless you want to create read-only view over nodes.

static int g_NextLinkId = 100;
// Counter to help generate link ids. In real application this will probably based on pointer to user data structure.

//--

//--------------------------------------------------------------
class NodePatcher
{
public:

	//--

	//TODO:
	// patch by code
	/*
	ed::PinId getPin(int i) {
		return ed::LinkId(i);
	}
	*/

	//--

	//--------------------------------------------------------------
	void link(int i, int o)
	{
		// Since we accepted new link, lets add one to our list of links.
		g_Links.push_back({
			ed::LinkId(g_NextLinkId++),
			ed::PinId(sourcesIds[i]),
			ed::PinId(targetsIds[o])
			});
	}

	//--

	/*
	//--------------------------------------------------------------
	void link(int i, int o)
	{
		ax::NodeEditor::QueryNewLink(PinId * startId, PinId * endId);



		ed::PinId inputPinId, outputPinId;

		// Since we accepted new link, lets add one to our list of links.
		g_Links.push_back({ ed::LinkId(g_NextLinkId++), i, o });

		// Draw new link.
		ed::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
	}
	*/

	//--

	//--------------------------------------------------------------
	void clearLinks()
	{
		g_Links.clear();

		/*
		// Then remove link from your data.
		for (auto& link : g_Links)
		{
			//if (link.Id == deletedLinkId)//all
			{
				g_Links.erase(&link);
				//break;
			}
		}
		*/
	}

	//--

	//TODO: workaround..

	// Callbacks for
	// parent engine
	ofParameter<bool>bNewLink{ "bNewLink", false };
	ofParameter<bool>bRemovedLink{ "bNewLink", false };

	int lastId = -1;
	int lastPinFrom = -1;
	int lastPinTo = -1;

	//TODO>
	vector<int> sourcesIds;
	vector<int> targetsIds;

	//--

	//--------------------------------------------------------------
	void OnSetup()
	{
		ed::Config config;
		config.SettingsFile = "NodePatcher.json";
		g_Context = ed::CreateEditor(&config);

		//TODO: predefined maps

		sourcesIds.clear();
		// id 1 is sources node 
		sourcesIds.push_back(2);
		sourcesIds.push_back(3);
		sourcesIds.push_back(4);
		sourcesIds.push_back(5);

		targetsIds.clear();
		// id 6 is targets node 
		targetsIds.push_back(7);
		targetsIds.push_back(8);
		targetsIds.push_back(9);
		targetsIds.push_back(10);
	}

	//--

	//--------------------------------------------------------------
	void OnExit()
	{
		ed::DestroyEditor(g_Context);
	}

	string getListLinks()
	{
		string s = "";
		s += "LIST LINKS";
		s += "\n";

		for (auto& link : g_Links)
		{
			int id = link.Id.Get();
			int in = link.InputId.Get();
			int out = link.OutputId.Get();
			s += ofToString("id:") + ofToString(id) + ofToString(" ");
			s += ofToString("in:") + ofToString(in) + ofToString(" ");
			s += ofToString("out:") + ofToString(out) + ofToString(" ");
			s += "\n";
		}
		s += "\n";

		return s;
	}

	//--

	//--------------------------------------------------------------
	void OnDraw()
	{
		ed::SetCurrentEditor(g_Context);

		// Start interaction with editor.
		ed::Begin("My Editor", ImVec2(0.0, 0.0f));

		int uniqueId = 1;
		//int uniqueIdNodes = 1;
		//int uniqueIdWire = 1;

		//
		// 1) Commit known data to editor
		//

		//--

		// Output
		// Submit Node PatchBay
		//ed::NodeId nodeI_OutputId = uniqueIdNodes++;
		ed::NodeId nodeI_OutputId = uniqueId++;

		ed::PinId  nodeI_OutputPinId1 = uniqueId++;
		ed::PinId  nodeI_OutputPinId2 = uniqueId++;
		ed::PinId  nodeI_OutputPinId3 = uniqueId++;
		ed::PinId  nodeI_OutputPinId4 = uniqueId++;

		if (g_FirstFrame)
			ed::SetNodePosition(nodeI_OutputId, ImVec2(10, 10));

		// Sources

		ed::BeginNode(nodeI_OutputId);
		{
			ImGui::Text("SOURCES");
			ImGui::Spacing();

			float w = 20;

			ImGui::Dummy(ImVec2(w, 0));
			ImGui::SameLine();

			ed::BeginPin(nodeI_OutputPinId1, ed::PinKind::Output);
			ImGui::Text("Out0 ->");
			ed::EndPin();

			ImGui::Dummy(ImVec2(w, 0));
			ImGui::SameLine();

			ed::BeginPin(nodeI_OutputPinId2, ed::PinKind::Output);
			ImGui::Text("Out1 ->");
			ed::EndPin();

			ImGui::Dummy(ImVec2(w, 0));
			ImGui::SameLine();

			ed::BeginPin(nodeI_OutputPinId3, ed::PinKind::Output);
			ImGui::Text("Out2 ->");
			ed::EndPin();

			ImGui::Dummy(ImVec2(w, 0));
			ImGui::SameLine();

			ed::BeginPin(nodeI_OutputPinId4, ed::PinKind::Output);
			ImGui::Text("Out3 ->");
			ed::EndPin();
		}
		ed::EndNode();

		//----

		// Input
		// Submit Node PatchBay
		//ed::NodeId nodeI_InputId = uniqueIdNodes++;
		ed::NodeId nodeI_InputId = uniqueId++;

		ed::PinId  nodeI_InputPinId1 = uniqueId++;
		ed::PinId  nodeI_InputPinId2 = uniqueId++;
		ed::PinId  nodeI_InputPinId3 = uniqueId++;
		ed::PinId  nodeI_InputPinId4 = uniqueId++;

		if (g_FirstFrame) ed::SetNodePosition(nodeI_InputId, ImVec2(300, 10));

		// Targets

		ed::BeginNode(nodeI_InputId);
		{
			ImGui::Text("TARGETS");
			ImGui::Spacing();

			ed::BeginPin(nodeI_InputPinId1, ed::PinKind::Input);
			ImGui::Text("<- In0");
			ed::EndPin();

			ed::BeginPin(nodeI_InputPinId2, ed::PinKind::Input);
			ImGui::Text("<- In1");
			ed::EndPin();

			ed::BeginPin(nodeI_InputPinId3, ed::PinKind::Input);
			ImGui::Text("<- In2");
			ed::EndPin();

			ed::BeginPin(nodeI_InputPinId4, ed::PinKind::Input);
			ImGui::Text("<- In3");
			ed::EndPin();
		}
		ed::EndNode();

		//--

		// Submit Links
		for (auto& linkInfo : g_Links)
			ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

		//
		// 2) Handle interactions
		//

		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ed::BeginCreate())
		{
			ed::PinId inputPinId, outputPinId;
			if (ed::QueryNewLink(&inputPinId, &outputPinId))
			{
				// QueryNewLink returns true if editor want to create new link between pins.
				//
				// Link can be created only for two valid pins, it is up to you to
				// validate if connection make sense. Editor is happy to make any.
				//
				// Link always goes from input to output. User may choose to drag
				// link from output pin or input pin. This determine which pin ids
				// are valid and which are not:
				//   * input valid, output invalid - user started to drag new ling from input pin
				//   * input invalid, output valid - user started to drag new ling from output pin
				//   * input valid, output valid   - user dragged link over other pin, can be validated

				if (inputPinId && outputPinId) // both are valid, let's accept link
				{
					// ed::AcceptNewItem() return true when user release mouse button.
					if (ed::AcceptNewItem())
					{
						// Since we accepted new link, lets add one to our list of links.
						g_Links.push_back({ ed::LinkId(g_NextLinkId++), inputPinId, outputPinId });

						// Draw new link.
						ed::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);

						//--

						//TODO: workaround

						//lastPinFrom = inputPinId.AsPointer();
						//lastPinFrom = inputPinId.cast<int>();
						//lastPinFrom = inputPinId.AsPointer();

						int i = (int)inputPinId.Get();
						int o = (int)outputPinId.Get();

						lastId = g_NextLinkId;
						lastPinFrom = i;
						lastPinTo = o;
						bNewLink = true;
					}

					// You may choose to reject connection between these nodes
					// by calling ed::RejectNewItem(). This will allow editor to give
					// visual feedback by changing link thickness and color.
				}
			}
		}
		ed::EndCreate(); // Wraps up object creation action handling.


		// Handle deletion action
		if (ed::BeginDelete())
		{
			// There may be many links marked for deletion, let's loop over them.
			ed::LinkId deletedLinkId;
			while (ed::QueryDeletedLink(&deletedLinkId))
			{
				// If you agree that link can be deleted, accept deletion.
				if (ed::AcceptDeletedItem())
				{
					// Then remove link from your data.
					for (auto& link : g_Links)
					{
						if (link.Id == deletedLinkId)
						{
							g_Links.erase(&link);

							//-

							//TODO:
							int i = (int)link.InputId.Get();
							int o = (int)link.OutputId.Get();
							lastPinFrom = i;
							lastPinTo = o;
							bRemovedLink = true;

							break;
						}
					}
				}

				// You may reject link deletion by calling:
				// ed::RejectDeletedItem();
			}
		}
		ed::EndDelete(); // Wrap up deletion action


		// End of interaction with editor.
		ed::End();

		if (g_FirstFrame) ed::NavigateToContent(0.0f);

		ed::SetCurrentEditor(nullptr);

		//ImGui::ShowMetricsWindow();

		//--

		g_FirstFrame = false;//flag that first frame done!
	}

};
