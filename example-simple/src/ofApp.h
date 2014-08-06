#pragma once

#include "ofMain.h"
#include "ofxVoronoi2D.h"

class ofApp : public ofBaseApp{
  public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mousePressed(int x, int y, int button);

  private:
    ofxVoronoi2D voronoi;
    vector<ofVec2f> pts;
    ofMesh mesh;
    unsigned int count;
    int dispMode;
};
