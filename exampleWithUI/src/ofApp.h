#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxVoronoi2D.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		// UI
		void exit(); 
        void guiEvent(ofxUIEventArgs &e);

    private:
        ofxVoronoi2D voronoi;
        vector<ofVec2f> pts;
        ofMesh mesh;
        unsigned int count;
        int dispMode;
        float zJitter;
        // Camera and Voronoi modes
        ofEasyCam cam;
        bool voroMode;
        int lastW, lastH;
        // UI Stuff
        ofxUICanvas ui;
};
