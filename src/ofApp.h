#pragma once

#include "ofMain.h"
#include "ofxMidi.h"

typedef struct {
	char name[20];
	unsigned int intervals[7];
	unsigned int starts;
}makam;

class ofApp : public ofBaseApp, public ofxMidiListener {
	public:
		void setup();
		void update();
		void draw();
		void newMidiMessage(ofxMidiMessage& msg);
		void setMakam(unsigned short int k);

		ofxMidiOut midiOut;
		ofxMidiIn midiIn;
		ofxMidiMessage midiMessage;
		void exit();
		stringstream text;
		unsigned int breath;
		int notesNumber;
		int scale[15];
		int pitchBends[15];
		int noteWidth;
		int curNote;
		int prNote;
		int pitchBend;
		unsigned int thr;
		bool noteOn;
		vector<makam> makams;

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
