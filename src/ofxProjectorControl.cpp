#include "ofxProjectorControl.h"

void ofxProjectorControl::logEverywhere(string logMessage)
{
	cout << logMessage << endl;

	ofLogNotice() << logMessage << endl;
}

ofxProjectorControl::ofxProjectorControl()
{
	logEverywhere("PROJECTOR CONTROL: class created");
}

//--------------------------------------------------------------
ofxProjectorControl::~ofxProjectorControl()
{
	//A vector of ofxTCPClient pointers is used that should be deleted here so we avoid memory leaks 
	for (int i = 0; i < projectorConnections.size(); i++)
	{
		delete (projectorConnections[i]);
		projectorConnections[i] = NULL;
	}
	projectorConnections.clear();
}

//--------------------------------------------------------------
void ofxProjectorControl::setupConnection()
{
		
	loadXmlSettings("ProjectorSettings.xml");
	logEverywhere("PROJECTOR CONTROL: Settings loaded");

	if (communicationMode == "RC232OverEthernet")
	{
		setupRC232Conenction();
	}
	else if (communicationMode == "PJLink")
	{
		setupPJLinkConenction();
	}
	else if (communicationMode == "KramerSwitch")
	{
		setupKramerConnection();
	}
		
}

//--------------------------------------------------------------
void ofxProjectorControl::setupRC232Conenction()
{
	logEverywhere("PROJECTOR CONTROL: setupRC232Conenction");
	//This is were the vector of connections is created 
	for (int i = 0; i < projectorIPs.size(); i++)
	{
		logEverywhere("PROJECTOR CONTROL: connecting to " + projectorIPs[i]);
		ofxTCPClient* _tcpClient = new ofxTCPClient();
		
		bool connected = _tcpClient->setup(projectorIPs[i], port);
		
		//if the connections is not possible then it is not pushed in the vector
		//When this for loop finishes inside projectorConnections we have all the active connections
		if (!connected)
		{
			logEverywhere("Projector couldn't connect: " + ofToString(i));
		}
		else
		{
			projectorConnections.push_back(_tcpClient);
		}		
	}
}

//--------------------------------------------------------------
void ofxProjectorControl::setupKramerConnection()
{
	cout << "PROJECTOR CONTROL: setupKramerConnection" << endl;

	//This is were the vector of connections is created 
	cout << "PROJECTOR CONTROL: connecting to " << kramerIP << endl;
		
	bool connected = kramerConnection.setup(kramerIP, port);

	//if the connections is not possible then it is not pushed in the vector
	//When this for loop finishes inside projectorConnections we have all the active connections
	if (!connected)
	{
		ofLogNotice() << "Kramer couldn't connect" << endl;
	}

}

//--------------------------------------------------------------
void ofxProjectorControl::setupPJLinkConenction()
{
//	cout << "Setting up PJLink connection" << endl;
	logEverywhere("Setting up PJLink connection");
	//This is were the vector of connections is created 
	for (int i = 0; i < projectorIPs.size(); i++)
	{
		string msgRx = "";
		ofxTCPClient* _tcpClient = new ofxTCPClient();

		bool connected = _tcpClient->setup(projectorIPs[i], port, true);

		//if the connections is not possible then it is not pushed in the vector
		//When this for loop finishes inside projectorConnections we have all the active connections
		if (!connected)
		{
			logEverywhere("Projector couldn't connect" + i);
		}
		else
		{
			logEverywhere("Connection established: " + projectorIPs[i]);
			logEverywhere("Number " + ofToString(i));
			string response = "";
			while (msgRx.length() < 8) {
				cout << "msgRx: " << msgRx << endl;
				msgRx = _tcpClient->receiveRaw();
			}
			ofLogNotice() << "received response: " << msgRx << endl;
			logEverywhere("Received response: " + msgRx);
			projectorConnections.push_back(_tcpClient);

			string authToken = "";

			//eg. PJLINK 1 604cc14d
			//if (msgRx[10] == '1') {

			//if (msgRx[10] == '1') {
			//	ofLogNotice() << "with authentication" << endl;
			//	MD5Engine md5;
			//	md5.reset();
			//	string hash = msgRx.substr(12, 8);
			//	ofLogNotice() << hash << endl;
			//	md5.update("admin1:igloo:" + hash );
			//	authToken = DigestEngine::digestToHex(md5.digest());
			//}
			//
		
		//	string msgSend = "%1POWR 0\r";
		//	_tcpClient->sendRaw(authToken+msgSend);

			string msgCommand ="%1POWR 0\r";
			string msgSend="";

			msgSend = authToken + msgCommand;

			cout << msgSend << endl;
			_tcpClient->sendRaw(msgSend);

			msgRx = "";
			while (msgRx.length() < 4) {
				msgRx = _tcpClient->receiveRaw();
			}

			logEverywhere("received response: " + msgRx);

			_tcpClient->close();
		}

		if (authenticationNeeded)
		{
			authenticatePJLink(msgRx, _tcpClient);
		}
		
	}
}

//Abstract layer function that triggers 3D
//--------------------------------------------------------------
void ofxProjectorControl::projector3DOn()
{
	projector3DActivate(default3DFormat);
	projector3DMode(default3DMode);
}

//Abstract layer function that turns 3D off
//--------------------------------------------------------------
void ofxProjectorControl::projector3DOff()
{
	projector3DActivate(FORMAT_3D_OFF);
}

//Projector Control of 3D convienience method using XML values
//--------------------------------------------------------------
void ofxProjectorControl::setProjector3D(bool state) {
	if (state) projector3DOn();
	else projector3DOff();
}

//Abstract layer functions for different projector brands using RS232 over RJ45 commmunication
//--------------------------------------------------------------
void ofxProjectorControl::projector3DActivate(int emitter)
{
	if(projectorBrand == "Vivitek")
	{
		projector3DActivateVivitek(emitter);
	}
	else if (projectorBrand == "Optoma")
	{
		projector3DActivateOptoma(emitter);
	}
}

void ofxProjectorControl::projector3DMode(int mode)
{
	if (projectorBrand == "Vivitek")
	{
		projector3DModeVivitek(mode);
	}
	else if (projectorBrand == "Optoma")
	{
		projector3DModeOptoma(mode);
	}
}

void ofxProjectorControl::projector3DSyncInvert(int activated)
{
	if (projectorBrand == "Vivitek")
	{
		projector3DSyncInvertVivitek(activated);
	}
	else if (projectorBrand == "Optoma")
	{
		projector3DSyncInvertOptoma(activated);
	}
}

void ofxProjectorControl::projectorOpen()
{
	projectorOpenOptoma();
}

void ofxProjectorControl::projectorClose()
{
	if (projectorBrand == "Vivitek")
	{
		projectorCloseVivitek();
	}
	else if (projectorBrand == "Optoma")
	{
		projectorCloseOptoma();
	}
}

//Implementation functions 3D activate for Vivitek and Optoma. In this the type of emitter is defined (DLP-Link or IR)
//--------------------------------------------------------------
void ofxProjectorControl::projector3DActivateVivitek(int emitter)
{
	string emmitterString = std::to_string(emitter);
	string message = "V99S0315"+ emmitterString +"\r\n";
	for (int i=0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

//Optoma doesn't seem to have an off command for the type of 3D emitter
//So in order to disable 3D we turn the 3D mode to auto 
//--------------------------------------------------------------
void ofxProjectorControl::projector3DActivateOptoma(int format)
{
	string formatString = std::to_string(format);
	if (format == FORMAT_DLP_LINK)
	{
		formatString = OPTOMA_DLP_LINK;
	}
	else if (format == FORMAT_IR)
	{
		formatString = OPTOMA_IR;
	}
	
	string message = "~00230 " + formatString + "\r\n";

	//Optoma doens't have a 3D off option so this is a hack that sets 3d mode to auto which disables 3d 
	if (format == FORMAT_3D_OFF)
	{
		message = OPTOMA_3D_OFF;
	}
	logEverywhere(message);
	for (int i = 0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}

}

//Implementation functions 3D mode for Vivitek and Optoma. 
//--------------------------------------------------------------
void ofxProjectorControl::projector3DModeVivitek(int mode)
{
	string modeString = std::to_string(mode);

	string message = "V99S0317" + modeString + "\r\n";
	for (int i = 0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

void ofxProjectorControl::projector3DModeOptoma(int mode)
{
	string modeString = std::to_string(mode);

	if (mode == MODE_FRAME_SEQUENTIAL)
	{
		modeString = OPTOMA_FRAME_SEQUENTIAL;
	}
	else if (mode == MODE_TOP_BOTTOM)
	{
		modeString = OPTOMA_TOP_BOTTOM;
	}
	else if (mode == MODE_SIDE_BY_SIDE)
	{
		modeString = OPTOMA_SIDE_BY_SIDE;
	}

	string message = "~00405 " + modeString + "\r\n";
	for (int i = 0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

//Implementation functions 3D Invert for Vivitek and Optoma. 
//--------------------------------------------------------------
void ofxProjectorControl::projector3DSyncInvertVivitek(int activated)
{
	string modeString = std::to_string(activated);
	string message = "V99S0316" + modeString + "\r\n";
	for (int i=0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

void ofxProjectorControl::projector3DSyncInvertOptoma(int activated)
{
	string activateString = std::to_string(activated);
	if (activated == SYNC_INVERT_OFF)
	{
		activateString = OPTOMA_SYNC_INVERT_OFF;
	}
	else if (activated == SYNC_INVERT_ON)
	{
		activateString = OPTOMA_SYNC_INVERT_ON;
	}

	string message = "~00231 " + activateString + "\r\n";

	logEverywhere(message);
	for (int i = 0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

//Implementation functions to close projectors for Vivitek and Optoma.
//--------------------------------------------------------------
void ofxProjectorControl::projectorCloseVivitek()
{

	string message = "V99S0002\r\n";
	for (int i=0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

void ofxProjectorControl::projectorCloseOptoma()
{
	string message = "~0000 0\r\n";
	for (int i = 0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

//Implementation function to open projectors for Optoma.
//--------------------------------------------------------------
void ofxProjectorControl::projectorOpenOptoma()
{
	string message = "~0000 1\r\n";
	for (int i = 0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

//--------------------------------------------------------------
void ofxProjectorControl::switchChannelsKramer()
{

	int fromChannelHex = startingChannel + 128;
	int toChannelHex   = startingChannel +  128;

	int command[4] = { 0x01, 0x81, 0x81, 0x81 };

//	bool send = kramerConnection.sendRawBytes((char*)command, sizeof(command));

//	bool send = kramerConnection.sendRaw("01818181");

	bool send = kramerConnection.sendRaw("#PRST-RCL 2\r\n");

	bool breakpoint = false;
}

//--------------------------------------------------------------
void ofxProjectorControl::loadXmlSettings(string path)
{
	bool _isLoaded = xml.load(path);
	logEverywhere("Loading " + path);
	if (_isLoaded)
	{
		// load the mode type in which the application will communicate with the projector
		communicationMode = xml.getValue("Settings:communicationMode", "");
		logEverywhere("Mode: " + communicationMode);

		//load the brand of the projector
		projectorBrand = xml.getValue("Settings:projectorBrand", "");

		// load the communication port it should be the same for all projectors
		port = xml.getValue("Settings::port", 23);
		logEverywhere("Port: " + ofToString(port));
		kramerIP = xml.getValue("Settings::kramerIP", "0");
		startingChannel = ofToInt(xml.getValue("Settings::startingChannel", "0"));
		numberOfInputs = ofToInt(xml.getValue("Settings::numberOfInputs", "0"));
		string tempDefault3DFormat = xml.getValue("Settings::default3DFormat", "IR");
		convertToEnumFormatSettings(tempDefault3DFormat);
		string tempDefault3DMode = xml.getValue("Settings::default3DMode", "SIDE_BY_SIDE");
		convertToEnumModeSettings(tempDefault3DMode);

		authenticationNeeded = ofToBool(xml.getValue("Settings::authenticationNeeded", "false"));

		if (authenticationNeeded)
		{
			password = xml.getValue("Settings::password", "igloo");
		}

		// load the IPs
		xml.pushTag("Settings");
		xml.pushTag("projectors");

		int numProjectors = xml.getNumTags("projector");
		for (int i = 0; i< numProjectors; i++) 
		{
			xml.pushTag("projector", i);
			string _IP = xml.getValue("IP", "fail");
			projectorIPs.push_back(_IP);
			xml.popTag(); // pop projector i
		}
		xml.popTag(); // pop projectors
		xml.popTag(); // pop settings
	}
	else
	{
		string errMsg = "[ERROR] Projector Control - cannot load settings xml";
		logEverywhere(errMsg);
	}
		
}

//--------------------------------------------------------------
void ofxProjectorControl::authenticatePJLink(string msgRx, ofxTCPClient* tcpClient)
{
	//string authToken = "";

	////eg. PJLINK 1 604cc14d
	//if (msgRx[7] == '1') {
	//	ofLogNotice() << "with authentication" << endl;
	//	MD5Engine md5;
	//	md5.reset();
	//	string hash = msgRx.substr(9, 8);
	//	ofLogNotice() << hash << endl;
	//	md5.update(hash + password);
	//	authToken = DigestEngine::digestToHex(md5.digest());
	//}
	//ofLogNotice() << "sending command: " << authToken + command << endl;
	//tcpClient->sendRaw(authToken + command);
	//msgRx = "";
	//while (msgRx.length() < 8) {
	//	msgRx = tcpClient->receiveRaw();
	//}
	//ofLogNotice() << "received response: " << msgRx << endl;
}

//--------------------------------------------------------------
void ofxProjectorControl::handleOSCMessage(ofxOscMessage msg)
{
	string oscMsgAddress = msg.getAddress();
	cout << "PROJECTOR CONTROL: handling osc message " << oscMsgAddress << endl;

	if (oscMsgAddress == "/projector/3DOff")				{ projector3DActivate(ofxProjectorControl::FORMAT_3D_OFF); }
	else if (oscMsgAddress == "/projector/3DOn")			{ projector3DOn(); }
	else if (oscMsgAddress == "/projector/DLP")				{ projector3DActivate(ofxProjectorControl::FORMAT_DLP_LINK); }
	else if (oscMsgAddress == "/projector/IR")				{ projector3DActivate(ofxProjectorControl::FORMAT_IR); }
	else if (oscMsgAddress == "/projector/FrameSequential")	{ projector3DMode(ofxProjectorControl::MODE_FRAME_SEQUENTIAL); }
	else if (oscMsgAddress == "/projector/TopBottom")		{ projector3DMode(ofxProjectorControl::MODE_TOP_BOTTOM); }
	else if (oscMsgAddress == "/projector/SBS")				{ projector3DMode(ofxProjectorControl::MODE_SIDE_BY_SIDE); }
	else if (oscMsgAddress == "/projector/FramePacking")	{ projector3DMode(ofxProjectorControl::MODE_FRAME_PACKING); }
	else if (oscMsgAddress == "/projector/InvertOff")		{ projector3DSyncInvert(ofxProjectorControl::SYNC_INVERT_OFF); }
	else if (oscMsgAddress == "/projector/InvertOn")		{ projector3DSyncInvert(ofxProjectorControl::SYNC_INVERT_ON); }
	else if (oscMsgAddress == "/projector/Close")			{ projectorClose(); }
	else if (oscMsgAddress == "/projector/Open")			{ projectorOpen(); }
}

void ofxProjectorControl::convertToEnumFormatSettings(string formatSetting)
{
	if (formatSetting == "3D_OFF")
	{
		default3DFormat = FORMAT_3D_OFF;
	}
	if (formatSetting == "DLP_LINK")
	{
		default3DFormat = FORMAT_DLP_LINK;
	}
	if (formatSetting == "IR")
	{
		default3DFormat = FORMAT_IR;
	}
}

void ofxProjectorControl::convertToEnumModeSettings(string modeSetting)
{
	if (modeSetting == "FRAME_SEQUENTIAL")
	{
		default3DMode = MODE_FRAME_SEQUENTIAL;
	}
	if (modeSetting == "TOP_BOTTOM")
	{
		default3DMode = MODE_TOP_BOTTOM;
	}
	if (modeSetting == "SIDE_BY_SIDE")
	{
		default3DMode = MODE_SIDE_BY_SIDE;
	}
	if (modeSetting == "FRAME_PACKING")
	{
		default3DMode = MODE_FRAME_PACKING;
	}
}