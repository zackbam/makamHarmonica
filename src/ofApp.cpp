#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
	notesNumber = 15;
	noteWidth = ofGetWidth() / notesNumber;
	curNote = -1;
	prNote = -1;
	scale[0] = 50;//d
	scale[1] = 52;//e
	scale[2] = 53;//f
	scale[3] = 54;//gb
	scale[4] = 57;//a
	scale[5] = 58;//bb
	scale[6] = 60;//c
	scale[7] = 61;//db
	scale[8] = 64;//e
	scale[9] = 65;//f
	scale[10] = 66;//gb
	scale[11] = 69;//a
	scale[12] = 70;//bb
	scale[13] = 72;//c
	scale[14] = 73;//db
	midiOut.listPorts();
	printf("Select midi OUT device (number 0 to %d): ",midiOut.getNumPorts()-1);
	int port;
	cin >> port;
	if (port >= 0 && port < midiOut.getNumPorts())
		midiOut.openPort(port);
	else
		printf("Wrong number given!\n");
	printf("midi Out connected\n");
	midiIn.listPorts();
	printf("Select midi IN (breath sensor) device (number 0 to %d): ", midiIn.getNumPorts() - 1);
	cin >> port;
	if (port >= 0 && port < midiIn.getNumPorts())
		midiIn.openPort(port);
	else
		printf("Wrong number given!\n");
	printf("midi In connected\n");
	midiIn.addListener(this);
	midiIn.setVerbose(true);
	thr = 5;
	//printf("Set threshold for triggering notes (0-126, enter for default = 5: ");
	//cin >> thr;
	pitchBend = 8192;
}

void ofApp::mouseMoved(int x, int y) {
	pitchBend = (float)(ofGetHeight() - y) / (float)ofGetHeight() * 16383;
	midiOut.sendPitchBend(1, pitchBend);
	prNote = curNote;
	curNote = x / noteWidth;
	if (curNote != prNote) {
		if (prNote != -1)
			midiOut.sendNoteOff(1, scale[prNote], 0);
		if (curNote != -1 && breath > thr) {
			midiOut.sendNoteOn(1, scale[curNote], breath);
			noteOn = true;
		}
	}
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	text << "value: " << midiMessage.value;
	ofDrawBitmapString(text.str(), 20, 20);
	text.str(""); // clear
	if (midiMessage.status == MIDI_PITCH_BEND) {
		ofDrawRectangle(20, 50, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth() - 40), 20);
	}
	else {
		ofDrawRectangle(20, 50, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth() - 40), 20);
	}

	
	for (int i = 0; i < notesNumber; i++) {
		ofLine(ofPoint(i*noteWidth, 0), ofPoint(i*noteWidth, ofGetHeight()));
	}
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	// make a copy of the latest message
	midiMessage = msg;
	breath = msg.value;
	if (breath < thr) {
		midiOut.sendNoteOff(1, scale[curNote], 0);
		noteOn = false;
	}
	else {
		midiOut.sendControlChange(1, 7, breath);
		if (!noteOn) {
			midiOut.sendNoteOn(1, scale[curNote], breath);
			noteOn = true;
		}
	}
}

void ofApp::exit() {
	midiOut.closePort();
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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