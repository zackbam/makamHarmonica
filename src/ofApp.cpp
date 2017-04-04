#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	notesNumber = 15;
	noteWidth = ofGetWidth() / notesNumber;
	curNote = -1;
	prNote = -1;
	scale[0] = 62;//d
	scale[1] = 64;//e
	scale[2] = 65;//f
	scale[3] = 66;//gb
	scale[4] = 69;//a
	scale[5] = 70;//bb
	scale[6] = 72;//c
	scale[7] = 73;//db
	scale[8] = 76;//e
	scale[9] = 77;//f
	scale[10] = 78;//gb
	scale[11] = 81;//a
	scale[12] = 82;//bb
	scale[13] = 84;//c
	scale[14] = 85;//db
	midiOut.listPorts();
	printf("Select midi Port to connect to (number 0 to %d): ",midiOut.getNumPorts()-1);
	int port;
	scanf("%d", &port);
	getchar();
	if (port >= 0 && port < midiOut.getNumPorts())
		midiOut.openPort(port);
	else
		printf("Wrong number given!\n");
	printf("midi connected\n");
	pitchBend = 8192;
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
	prNote = curNote;
	curNote = ofGetMouseX()/ noteWidth;
	pitchBend = (float)(ofGetHeight() - ofGetMouseY()) / (float)ofGetHeight() * 16383;
	cout << pitchBend << endl;
	midiOut.sendPitchBend(1, pitchBend);
	if (curNote != prNote) {
		if (prNote != -1)
			midiOut.sendNoteOff(1, prNote, 0);
		if (curNote != -1) {
			midiOut.sendNoteOn(1, scale[curNote] - 12, 100);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	for (int i = 0; i < notesNumber; i++) {
		ofLine(ofPoint(i*noteWidth, 0), ofPoint(i*noteWidth, ofGetHeight()));
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
	noteWidth = ofGetWidth() / notesNumber;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}