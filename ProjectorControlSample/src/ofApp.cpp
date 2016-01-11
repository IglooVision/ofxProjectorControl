#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	projectControl.setupConnection();	

	receiver.setup(PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
	while (receiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		string oscMsgAddress = m.getAddress();
		string oscMsgFromIP = m.getRemoteIp();



		if (ofIsStringInString(oscMsgAddress, "/projector/")) 
		{
			projectControl.handleOSCMessage(m);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofClear(0);
	ofDrawBitmapString("Press q to deactivate 3D", 100, 100);
	ofDrawBitmapString("Press w to activate DLP-Link", 100, 150);
	ofDrawBitmapString("Press e to activate IR", 100, 200);
	ofDrawBitmapString("Press a to set mode to Frame Sequential", 500, 100);
	ofDrawBitmapString("Press s to set mode to Top/Bottom", 500, 150);
	ofDrawBitmapString("Press d to set mode to Side-by-side", 500, 200);
	ofDrawBitmapString("Press f to set mode to Frame Packing", 500, 250);
	ofDrawBitmapString("Press z to deactivate 3D Sync Invert", 250, 400);
	ofDrawBitmapString("Press x to activate 3D Sync Invert", 250, 450);
	ofDrawBitmapString("Press v to close the projector", 250, 500);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
		
	if (key == 'q')
	{
		projectControl.projector3DActivate(ofxProjectorControl::EMITTER_3D_OFF);
	}
	if (key == 'w')
	{
		projectControl.projector3DActivate(ofxProjectorControl::EMITTER_DLP_LINK);
	}
	if (key == 'e')
	{
		projectControl.projector3DActivate(ofxProjectorControl::EMITTER_IR);
	}
	if (key == 'a')
	{
		projectControl.projector3DMode(ofxProjectorControl::MODE_FRAME_SEQUENTIAL);
	}
	if (key == 's')
	{
		projectControl.projector3DMode(ofxProjectorControl::MODE_TOP_BOTTOM);
	}
	if (key == 'd')
	{
		projectControl.projector3DMode(ofxProjectorControl::MODE_SIDE_BY_SIDE);
	}
	if (key == 'f')
	{
		projectControl.projector3DMode(ofxProjectorControl::MODE_FRAME_PACKING);
	}
	if (key == 'z')
	{
		projectControl.projector3DSyncIvenrt(ofxProjectorControl::SYNC_INVERT_OFF);
	}
	if (key == 'x')
	{
		projectControl.projector3DSyncIvenrt(ofxProjectorControl::SYNC_INVERT_ON);
	}
	if (key == 'v')
	{
		projectControl.projectorClose();
	}
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}