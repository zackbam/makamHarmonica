#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
	int temp;
	thr = 60;
	sensitivity = 1;
	notesNumber = 15;
	noteWidth = (float)ofGetWidth() / (float)notesNumber;
	curNote = -1;
	prNote = -1;
	tempPitch = 8192;
	FILE *initParam,*loadMakams;
	loadMakams = fopen("makams.txt", "r");
	if (loadMakams == NULL)
		cout << "No makams.txt file found\n";
	else {
		makam tempMakam;
		while (fscanf(loadMakams, "%s %d %d %d %d %d %d %d %d", tempMakam.name, &tempMakam.starts, &tempMakam.intervals[0], &tempMakam.intervals[1], &tempMakam.intervals[2], &tempMakam.intervals[3], &tempMakam.intervals[4], &tempMakam.intervals[5], &tempMakam.intervals[6]) != EOF) {
			makams.push_back(tempMakam);
		}
	}
	initParam = fopen("settings.txt", "r");
	char paramName[30];
	
	cargahTonalityMidi = 48;
	if (initParam == NULL)
		cout << "No settings.txt file found\n";
	else {
		while (fscanf(initParam, "%s %d", paramName, &temp) != EOF) {
			//printf("%s, %d\n", paramName, temp);
			if (strcmp(paramName, "threshold") == 0)
				thr = temp;
			else if (strcmp(paramName, "sensitivity") == 0)
				sensitivity = temp;
			else if (strcmp(paramName, "framerate") == 0)
				ofSetFrameRate(temp);
			else if (strcmp(paramName, "notesNum") == 0)
				notesNumber = temp;
			else if (strcmp(paramName, "cargahTonalityMidi") == 0)
				cargahTonalityMidi = temp;

		}
	}
	cout << "Loaded " << makams.size() << " makams:" << endl;
	for (int i = 0; i < makams.size(); i++) {
		printf("%d: %s %d %d %d %d %d %d %d %d\n", i, makams[i].name, makams[i].starts, makams[i].intervals[0], makams[i].intervals[1], makams[i].intervals[2], makams[i].intervals[3], makams[i].intervals[4], makams[i].intervals[5], makams[i].intervals[6]);
	}
	printf("Select Makam (number + enter): ");
	cin >> selectedMakam;
	cout << "loading " << makams[selectedMakam].name << endl;
	setMakam(selectedMakam);
	midiOut.listPorts();
	breath = 100;
	printf("Select midi OUT device (number 0 to %d): ",midiOut.getNumPorts()-1);
	int port;
	cin >> port;
	if (port >= 0 && port < midiOut.getNumPorts())
		midiOut.openPort(port);
	else
		printf("Wrong number given!\n");
	printf("midi Out connected\n");
	midiIn.listPorts();
	printf("Select midi IN (breath sensor) device (number 0 to %d). \nPress 9 for no breath sensor: ", midiIn.getNumPorts() - 1);
	cin >> port;
	if (port >= 0 && port < midiIn.getNumPorts())
		midiIn.openPort(port);
	else {
		printf("Breath Sensor OFF\n");
		breath = 100;
	}
	printf("midi In connected\n");
	midiIn.addListener(this);
	midiIn.setVerbose(true);
	pitchBend = 8192;
	fclose(initParam);
}

void ofApp::mouseMoved(int x, int y) {
	prNote = curNote;
	curNote = x / noteWidth;
	pitchBend = (float)(ofGetHeight() - y) / (float)ofGetHeight() * 16383 - 8192; //how much pitch bend additional to the pitch bend of the note
	tempPitch = pitchBend + pitchBends[curNote];
	if (tempPitch < 0)
		tempPitch = 0;
	else if (tempPitch > 16383)
		tempPitch = 16383;
	midiOut.sendPitchBend(1, tempPitch);
	if (curNote != prNote) {
		if (prNote != -1)
			midiOut.sendNoteOff(1, scale[prNote], 0);
		if (curNote != -1 && breath > thr) {
			//printf("%d %d %d\n", curNote,scale[curNote], pitchBends[curNote]);
			midiOut.sendPitchBend(1, tempPitch);
			midiOut.sendNoteOn(1, scale[curNote], breath);
			noteOn = true;
		}
	}
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	// make a copy of the latest message
	midiMessage = msg;
	
	if (msg.status == MIDI_CONTROL_CHANGE || msg.status == MIDI_AFTERTOUCH || msg.status == MIDI_POLY_AFTERTOUCH) {
		breath = msg.value * sensitivity;
		if (breath > 127)
			breath = 127;
		if (breath < thr) {
			midiOut.sendNoteOff(1, scale[curNote], 0);
			noteOn = false;
		}
		else {
			midiOut.sendControlChange(1, 7, breath);
			if (!noteOn) {
				//printf("%d %d %d\n", curNote, scale[curNote], pitchBends[curNote]);
				midiOut.sendPitchBend(1, tempPitch);
				midiOut.sendNoteOn(1, scale[curNote], breath);
				noteOn = true;
			}
		}
	}
	else if (msg.status == MIDI_NOTE_OFF || msg.velocity == 0) {
		midiOut.sendNoteOff(1, scale[curNote], 0);
		noteOn = false;
		breath = 0;
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(0);
	// draw the last recieved message contents to the screen
	text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
	ofDrawBitmapString(text.str(), 20, 20);
	text.str(""); // clear

	text << "channel: " << midiMessage.channel;
	ofDrawBitmapString(text.str(), 20, 34);
	text.str(""); // clear

	text << "pitch: " << midiMessage.pitch;
	ofDrawBitmapString(text.str(), 20, 48);
	text.str(""); // clear
	ofDrawRectangle(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth() - 40), 20);

	text << "velocity: " << midiMessage.velocity;
	ofDrawBitmapString(text.str(), 20, 96);
	text.str(""); // clear
	ofDrawRectangle(20, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth() - 40), 20);

	text << "control: " << midiMessage.control;
	ofDrawBitmapString(text.str(), 20, 144);
	text.str(""); // clear
	ofDrawRectangle(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth() - 40), 20);

	text << "value: " << midiMessage.value;
	ofDrawBitmapString(text.str(), 20, 192);
	text.str(""); // clear
	if (midiMessage.status == MIDI_PITCH_BEND) {
		ofDrawRectangle(20, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth() - 40), 20);
	}
	else {
		ofDrawRectangle(20, 300, ofMap(breath, 0, 127, 0, ofGetWidth() - 40), 20);
	}

	text << "delta: " << midiMessage.deltatime;
	ofDrawBitmapString(text.str(), 20, 240);
	text.str(""); // clear
	ofSetColor(255); 
	ofSetLineWidth(ofGetHeight()*0.01);
	ofLine(ofPoint(0, ofGetHeight() / 2), ofPoint(ofGetWidth(), ofGetHeight() / 2));
	ofSetLineWidth(ofGetWidth()*0.01);
	for (int i = 0; i < notesNumber; i++) {
		if (i%7 == 3) {
			ofSetColor(255, 0, 0);
			ofLine(ofPoint(i*noteWidth, 0), ofPoint(i*noteWidth, ofGetHeight()));
			ofSetColor(255);
		}
		else
			ofLine(ofPoint(i*noteWidth, 0), ofPoint(i*noteWidth, ofGetHeight()));
		if (i > 0) {
			ofSetColor(0);
			ofDrawBitmapString(ofToString(curMakam[i] - curMakam[i - 1]), ofPoint(i*noteWidth, ofGetHeight()*0.5));
			ofSetColor(255);
		}
	}
	
}


void ofApp::exit() {
	midiOut.closePort();
	midiIn.closePort();
	midiIn.removeListener(this);
}
void ofApp::setMakam(unsigned short int k) {	
	int scaleComas = (int)makams[k].starts;
	float interval = cargahTonalityMidi + (float)(scaleComas) / 53.0*12.0;
	scale[3] = round(interval);
	pitchBends[3] = 8192 + (interval - scale[3]) * 8192;
	curMakam[3] = scaleComas;
	for (int i = 4; i < notesNumber; i++) {
		interval = cargahTonalityMidi + (float)(makams[k].intervals[(i-4) % 7] + scaleComas) / 53.0*12.0;
		scale[i] = round(interval);
		pitchBends[i] = 8192 + (interval - scale[i]) * 8192; //the pitch bend sould be set to 8192->1 semitone.
		scaleComas += makams[k].intervals[(i - 4) % 7];
		curMakam[i] = scaleComas;
	}
	for (int i = 0; i < 3; i++) {
		scale[i] = scale[i+7]-12;
		pitchBends[i] = pitchBends[i+7]; //the pitch bend sould be set to 8192->1 semitone.
		curMakam[i] = curMakam[i + 7] - 53;
	}
	for (int i = 0; i < notesNumber; i++) {
		printf("scale[%d] = %d, pitchBend = %d\n", i, scale[i], pitchBends[i]);
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case 'f':
		ofToggleFullscreen();
		break;
	default:
		break;
	}
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
	noteWidth = (float)w / (float)notesNumber;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}