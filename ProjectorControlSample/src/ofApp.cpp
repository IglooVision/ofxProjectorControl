#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	projectControl.setupConnection();	
}

//--------------------------------------------------------------
void ofApp::update(){

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
		projectControl.projector3DActivate(0);
	}
	if (key == 'w')
	{
		projectControl.projector3DActivate(1);
	}
	if (key == 'e')
	{
		projectControl.projector3DActivate(2);
	}
	if (key == 'a')
	{
		projectControl.projector3DMode(0);
	}
	if (key == 's')
	{
		projectControl.projector3DMode(1);
	}
	if (key == 'd')
	{
		projectControl.projector3DMode(2);
	}
	if (key == 'f')
	{
		projectControl.projector3DMode(3);
	}
	if (key == 'z')
	{
		projectControl.projector3DSyncIvenrt(0);
	}
	if (key == 'x')
	{
		projectControl.projector3DSyncIvenrt(1);
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