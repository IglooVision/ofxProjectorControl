ofxProjectorControl
============

Basic interface for Vivitek and Optoma projectors to activate 3D, select 3D mode, activate 3D Synce Invert and close projector.

It uses RS232 command control for LAN/RJ45 interface.

There is a basic implementation for communication using the PJlink protocol (this part of the code uses the relevant code from https://github.com/nullboundary/ofxPJControl ) and a basic test implementation for the control of a Kramer Matrix

#XML Tags

<communicationMode> - Pick the type of connection RC232OverEthernet, PJLink, KramerSwitch
<projectorBrand> - Pick the brand Optoma and Vivitek at the moment
<default3DFormat> - Pick the default3DFormat for projector3DOn Parameters: 0:Off 1:DLP - Link 2 : IR
<default3DMode> - Pick the default3DMode for projector3DOn Parameters: 0:Frame Sequential 1:Top/Bottom 2 : Side-By-Side 3: Frame Packing
<port> - The communication port that will be used
<authenticationNeeded> - If authentication is needed for the PJLink is not working at the moment
<password> - The password for PJLink authentication is not working at the moment
<projectors> - The Projectors IPs
<kramerIP> - The ip for communication with a Kramer switch
<startingChannel> - Not used
<numberOfInputs> - Not used

#Author

Igloovision

#Dependencies

ofxNetwork
ofxXmlSettings



  