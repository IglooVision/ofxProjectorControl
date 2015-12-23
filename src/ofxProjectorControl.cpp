#include "ofxProjectorControl.h"

ofxProjectorControl::ofxProjectorControl()
{
	
}

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

void ofxProjectorControl::setupConnection()
{
	loadXmlSettings("ProjectorSettings.xml");

	if (communicationMode == "RC232OverEthernet")
	{
		setupRC232Conenction();
	}
		
}

void ofxProjectorControl::setupRC232Conenction()
{
	//This is were the vector of connections is created 
	for (int i = 0; i < projectorIPs.size(); i++)
	{
		ofxTCPClient* _tcpClient = new ofxTCPClient();
		
		bool connected = _tcpClient->setup(projectorIPs[i], 23);
		
		//if the connections is not possible then it is not pushed in the vector
		//When this for loop finishes inside projectorConnections we have all the active connections
		if (!connected)
		{
			ofLogNotice() << "Projector "  << i << " couldn't connect" << endl;
		}
		else
		{
			projectorConnections.push_back(_tcpClient);
		}		
	}
}



void ofxProjectorControl::projector3DActivate(int emitter)
{
	string emmitterString = std::to_string(emitter);
	string message = "V99S0315"+ emmitterString +"\r\n";
	for (int i=0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

void ofxProjectorControl::projector3DMode(int mode)
{
	string modeString = std::to_string(mode);
	string message = "V99S0317"+ modeString +"\r\n";
	for (int i=0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

void ofxProjectorControl::projector3DSyncIvenrt(int activated)
{
	string modeString = std::to_string(activated);
	string message = "V99S0316" + modeString + "\r\n";
	for (int i=0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

void ofxProjectorControl::projectorClose()
{

	string message = "V99S0002\r\n";
	for (int i=0; i < projectorConnections.size(); i++)
	{
		projectorConnections[i]->sendRaw(message);
	}
}

void ofxProjectorControl::loadXmlSettings(string path)
{
	bool _isLoaded = xml.load(path);
	if (_isLoaded)
	{
		// load the mode type in which the application will communicate with the projector
		communicationMode = xml.getValue("settings:communicationMode", "");

		// load the communication port it should be the same for all projectors
		port = xml.getValue("settings::port", 0);

		// load the IPs
		xml.pushTag("settings");
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
		ofLogNotice() << "[ERROR] Heartbeart - cannot load heartbeatSettings.xml" << endl;
	}
		
}