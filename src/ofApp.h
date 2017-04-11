#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

typedef struct {
	char name[30];
	unsigned int intervals[7];
	unsigned int starts;
}makam;

typedef struct {
	string name;
	vector<int> intervals;
}tetrachord;

class ofApp : public ofBaseApp, public ofxMidiListener {
	public:
		void setup();
		void update();
		void draw();
		void newMidiMessage(ofxMidiMessage& msg);
		void setMakam(unsigned short int k);
		void insterTetracord(tetrachord temp, int pos);
		ofxMidiOut midiOut;
		ofxMidiIn midiIn;
		ofxMidiMessage midiMessage;
		void exit();
		stringstream text;
		unsigned int breath;
		unsigned int fixationSpeed;
		ofPoint prMouse;
		int notesNumber;
		int scale[200];
		int curMakam[200];
		int cargahTonalityMidi;
		int pitchBends[200];
		int tempPitch;
		float noteWidth;
		int curNote;
		int prNote;
		int pitchBend;
		int selectedMakam;
		unsigned int thr,sensitivity;
		bool noteOn;
		vector<makam> makams;
		vector<tetrachord> tetrachords;
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


};
