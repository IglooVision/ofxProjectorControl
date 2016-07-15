#pragma once

#include "ofxNetwork.h"
#include "ofxXmlSettings.h"
#include "ofxOSC.h"
#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/StreamCopier.h"

#define DEFAULT_KRAMER_IP "192.168.1.38"
#define OPTOMA_FRAME_SEQUENTIAL "3"
#define OPTOMA_TOP_BOTTOM "2"
#define OPTOMA_SIDE_BY_SIDE "1"
#define OPTOMA_SYNC_INVERT_ON "0"
#define OPTOMA_SYNC_INVERT_OFF "1"
#define OPTOMA_3D_OFF "~00405 0\r\n"
#define OPTOMA_DLP_LINK "1"
#define OPTOMA_IR "3"

using Poco::DigestEngine;
using Poco::MD5Engine;
using Poco::DigestOutputStream;
using Poco::StreamCopier;

using namespace std;

class ofxProjectorControl
{

	public:

		enum EmitterSettings {
			FORMAT_3D_OFF,
			FORMAT_DLP_LINK,
			FORMAT_IR
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

		//This activates the 3D projection and sets the projector in IR - SBS mode.
		//It is part of the abstract layer and works for different brands as long as the correct brand is specified in Projector Settings xml
		void projector3DOn();

		//This deactivates the 3D projection.
		//It is part of the abstract layer and works for different brands as long as the correct brand is specified in Projector Settings xml
		void projector3DOff();

		//Activates 3D projection
		//Parameters: 0:Off 1:DLP - Link 2 : IR
		void projector3DActivate(int format);
		
		//Sets 3D mode 
		//Parameters: 0:Frame Sequential 1:Top/Bottom 2 : Side-By-Side 3: Frame Packing
		void projector3DMode(int mode);

		//Sets 3D Sync Invert 
		//Parameters: 0:Off 1:On
		void projector3DSyncInvert(int activated);

		//Closes the projector
		void projectorClose();

		//Open the projector
		//Only works for Optoma
		void projectorOpen();

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

		//Activates 3D projection
		//Parameters: 0:Off 1:DLP - Link 2 : IR
		//This is for Vivitek DU978-WT. 
		void projector3DActivateVivitek(int format);

		//Sets 3D mode 
		//Parameters: 0:Frame Sequential 1:Top/Bottom 2 : Side-By-Side 3: Frame Packing
		//This is for Vivitek DU978-WT.
		void projector3DModeVivitek(int mode);

		//Sets 3D Sync Invert 
		//Parameters: 0:Off 1:On
		//This is for Vivitek DU978-WT. 
		void projector3DSyncInvertVivitek(int activated);

		//Closes the projector
		//This is for Vivitek DU978-WT. 
		void projectorCloseVivitek();

		//Activates 3D projection
		//Parameters: 0:Off 1:DLP - Link 2 : IR
		//This is for Optoma 415. 
		void projector3DActivateOptoma(int format);

		//Sets 3D mode 
		//Parameters: 0:Frame Sequential 1:Top/Bottom 2 : Side-By-Side 3: Frame Packing
		//This is for Optoma 415-WT. 
		void projector3DModeOptoma(int mode);

		//Sets 3D Sync Invert 
		//Parameters: 0:Off 1:On
		//This is for Optoma 415. 
		void projector3DSyncInvertOptoma(int activated);

		//Closes the projector
		//This is for Optoma 415. 
		void projectorCloseOptoma();

		//Opens the projector
		//This is for Optoma 415. 
		void projectorOpenOptoma();

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
		string					projectorBrand;
		int						default3DFormat;
		int						default3DMode;
};