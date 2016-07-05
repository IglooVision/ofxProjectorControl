#pragma once

#include "ofxNetwork.h"
#include "ofxXmlSettings.h"
#include "ofxOSC.h"
#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/StreamCopier.h"

#define DEFAULT_KRAMER_IP "192.168.1.38"

using Poco::DigestEngine;
using Poco::MD5Engine;
using Poco::DigestOutputStream;
using Poco::StreamCopier;

using namespace std;

class ofxProjectorControl
{

	public:

		enum EmitterSettings {
			EMITTER_3D_OFF,
			EMITTER_DLP_LINK,
			EMITTER_IR
		};

		enum ModeSettings {
			MODE_FRAME_SEQUENTIAL,
			MODE_TOP_BOTTOM,
			MODE_SIDE_BY_SIDE,
			MODE_FRAME_PACKING
		};

		enum SyncInvert {
			SYNC_INVERT_OFF,
			SYNC_INVERT_ON
		};

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
		//Parameters: 0:Frame Sequential 1:Top/Bottom 2 : Side-By-Side 3: Frame Packing
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

		//A simple interface for Osc Messages
		void handleOSCMessage(ofxOscMessage msg);

		//A authentication function for PJLink in case it is needed
		void authenticatePJLink(string msgRx, ofxTCPClient* tcpClient);

		//A function to switch between channels in the Kramer Matrix
		void switchChannelsKramer();
	
	private:
		//Sets up the connections by directly connecting to the projector
		//It uses RC232 protocol through Ethernet
		void setupRC232Conenction();

		//Sets up the connections by directly connecting to the projector
		//It uses PJLink
		void setupPJLinkConenction();

		//Sets up the connections by directly connecting to the Kramer matrix
		//It uses a direct Ethernet connection with the Kramer matrix
		void setupKramerConnection();

		ofxXmlSettings			xml;
		vector<ofxTCPClient*>	projectorConnections;
		vector<string>			projectorIPs;
		string					communicationMode;
		ofxTCPClient			kramerConnection;
		int						port;


		bool					authenticationNeeded;
		string					password;

		string					kramerIP;
		int						startingChannel;
		int						numberOfInputs;
};