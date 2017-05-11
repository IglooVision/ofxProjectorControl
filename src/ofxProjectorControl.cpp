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
	for (int i = 0; i < projectors.size(); i++)
	{
		projectors[i]->connection.close();
		delete (projectors[i]);
	}
	projectors.clear();
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
		
}

//--------------------------------------------------------------
void ofxProjectorControl::setupRC232Conenction()
{
	// This method is now depreciated and the connection is tested and made on each sending of a command to handle connection dropouts
	// TODO: Test each connection here and add in a flag to stop trying to connect to missing projectors on each command if they wern't availiable at startup

	logEverywhere("PROJECTOR CONTROL: setupRC232Conenction");
}

//--------------------------------------------------------------
void ofxProjectorControl::setupPJLinkConenction()
{
//	cout << "Setting up PJLink connection" << endl;
	logEverywhere("Setting up PJLink connection");
	//This is were the vector of connections is created 
	for (int i = 0; i < projectors.size(); i++)
	{
		string msgRx = "";

		bool connected = projectors[i]->connection.setup(projectors[i]->ip, projectors[i]->port, true);

		//if the connections is not possible then it is not pushed in the vector
		//When this for loop finishes inside projectorConnections we have all the active connections
		if (!connected)
		{
			logEverywhere("Projector couldn't connect" + i);
		}
		else
		{
			logEverywhere("Connection established: " + projectors[i]->ip);
			logEverywhere("Number " + ofToString(i));
			string response = "";
			while (msgRx.length() < 8) {
				cout << "msgRx: " << msgRx << endl;
				msgRx = projectors[i]->connection.receiveRaw();
			}
			ofLogNotice() << "received response: " << msgRx << endl;
			logEverywhere("Received response: " + msgRx);

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
			projectors[i]->connection.sendRaw(msgSend);

			msgRx = "";
			while (msgRx.length() < 4) {
				msgRx = projectors[i]->connection.receiveRaw();
			}

			logEverywhere("received response: " + msgRx);

			projectors[i]->connection.close();

			if (authenticationNeeded)
			{
				authenticatePJLink(msgRx, &projectors[i]->connection);
			}

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
	sendSerialCommand(message);
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
	else if (format == FORMAT_VESA)
	{
		formatString = OPTOMA_VESA;
	}
	
	string message = "~00230 " + formatString + "\r\n";

	//Optoma doens't have a 3D off option so this is a hack that sets 3d mode to auto which disables 3d 
	if (format == FORMAT_3D_OFF)
	{
		message = OPTOMA_3D_OFF;
	}
	logEverywhere(message);
	sendSerialCommand(message);
}

//Implementation functions 3D mode for Vivitek and Optoma. 
//--------------------------------------------------------------
void ofxProjectorControl::projector3DModeVivitek(int mode)
{
	string modeString = std::to_string(mode);
	string message = "V99S0317" + modeString + "\r\n";
	sendSerialCommand(message);
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
	sendSerialCommand(message);
}

//Implementation functions 3D Invert for Vivitek and Optoma. 
//--------------------------------------------------------------
void ofxProjectorControl::projector3DSyncInvertVivitek(int activated)
{
	string modeString = std::to_string(activated);
	string message = "V99S0316" + modeString + "\r\n";
	sendSerialCommand(message);
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
	sendSerialCommand(message);
}

//Implementation functions to close projectors for Vivitek and Optoma.
//--------------------------------------------------------------
void ofxProjectorControl::projectorCloseVivitek()
{

	string message = "V99S0002\r\n";
	sendSerialCommand(message);
}

void ofxProjectorControl::projectorCloseOptoma()
{
	string message = "~0000 0\r\n";
	sendSerialCommand(message);
}

//Implementation function to open projectors for Optoma.
//--------------------------------------------------------------
void ofxProjectorControl::projectorOpenOptoma()
{
	string message = "~0000 1\r\n";
	sendSerialCommand(message);
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
		logEverywhere("3D Format: " + tempDefault3DFormat);
		convertToEnumFormatSettings(tempDefault3DFormat);
		string tempDefault3DMode = xml.getValue("Settings::default3DMode", "SIDE_BY_SIDE");
		logEverywhere("3D Mode: " + tempDefault3DMode);

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
		projectors.clear();

		for (int i = 0; i< numProjectors; i++) 
		{
			Projector * projector = new Projector();			
			xml.pushTag("projector", i);
			string _IP = xml.getValue("IP", "fail");
			projector->ip = _IP;
			projector->port = port;
			projectors.push_back(projector);
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
	else if (formatSetting == "DLP_LINK")
	{
		default3DFormat = FORMAT_DLP_LINK;
	}
	else if (formatSetting == "IR")
	{
		default3DFormat = FORMAT_IR;
	}
	else if (formatSetting == "VESA")
	{
		default3DFormat = FORMAT_VESA;
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


void ofxProjectorControl::sendSerialCommand(string command) {
	for (int projectorNum = 0; projectorNum < projectors.size(); projectorNum++) sendSerialCommand(command, projectorNum);
}

void ofxProjectorControl::sendSerialCommand(string command, int projectorNum) {
	sendSerialCommand(command, projectors[projectorNum]);
}

void ofxProjectorControl::sendSerialCommand(string command, Projector* projector) {
	bool connected = projector->connection.isConnected();

	if (!connected) {
		logEverywhere("PROJECTOR CONTROL: connecting to " + projector->ip);
		connected = projector->connection.setup(projector->ip, projector->port);			
		if (!connected) logEverywhere("ERROR: Projector couldn't connect");
	}

	if (connected) projector->connection.sendRaw(command);
}
