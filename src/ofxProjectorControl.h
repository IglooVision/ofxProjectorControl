#pragma once

#include "ofxNetwork.h"
#include "ofxXmlSettings.h"

using namespace std;

class ofxProjectorControl
{
	public:
		ofxProjectorControl();
		~ofxProjectorControl();

		//This sets up the connection with the projectors. 
		//The number of connections set depends on the settings inside ProjectorSettings.xml
		//This is an abstract layer that gets the Communication Mode and calls the relevant function
		void setupConnection();

		//Activates 3D projection
		//Parameters: 0:Off 1:DLP - Link 2 : IR
		//This is for Vivitek DU978-WT. It should get abstracted
		void projector3DActivate(int emitter);
		
		//Sets 3D mode 
		//Parameters: 0:Off 1:DLP - Link 2 : IR
		//This is for Vivitek DU978-WT. It should get abstracted
		void projector3DMode(int mode);

		//Sets 3D Sync Invert 
		//Parameters: 0:Off 1:On
		//This is for Vivitek DU978-WT. It should get abstracted
		void projector3DSyncIvenrt(int activated);

		//Closes the projector
		//This is for Vivitek DU978-WT. It should get abstracted
		void projectorClose();

		//Loads the XML settings from ProjectorSettings.xml
		void loadXmlSettings(string path);
	
	private:
		//Sets up the connections by directly connecting to the projector
		//It uses RC232 protocol through Ethernet
		void setupRC232Conenction();


		ofxXmlSettings			xml;
		vector<ofxTCPClient*>	projectorConnections;
		vector<string>			projectorIPs;
		string					communicationMode;
		int						port;

};