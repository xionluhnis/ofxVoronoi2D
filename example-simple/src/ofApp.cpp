#include "ofApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetFrameRate(24);
  count = 0;
  dispMode = 0;
  // ofEnableAntiAliasing();
  ofEnableBlendMode(OF_BLENDMODE_ALPHA);
  // ofEnableSmoothing();
  // voronoi.setMinDist(std::epsilon<float>());
}

//--------------------------------------------------------------
void ofApp::update(){
  // update voronoi
  if(pts.size() != count){
    voronoi.compute(pts, ofRectangle(0.0f, 0.0f, ofGetWidth(), ofGetHeight()), 0.0f);
    count = pts.size();
    // rebuild mesh
    mesh.clear();
    voronoi.buildMesh(mesh);

    // random face colors
    unsigned int N = mesh.getIndices().size(), M = mesh.getVertices().size();
    vector<ofFloatColor> colors;
    colors.resize(M);
    for(unsigned int i = 0; i < N; i += 3){
      ofFloatColor c(
        ofRandom(1.0f), // R
        ofRandom(1.0f), // G
        ofRandom(1.0f)  // B
      );
      colors[mesh.getIndex(i + 0)] = c; // cell center
      colors[mesh.getIndex(i + 1)] = ofFloatColor(1.0f, 1.0f, 1.0f, 0.0f); // cell border
      colors[mesh.getIndex(i + 2)] = ofFloatColor(1.0f, 1.0f, 1.0f, 0.0f); // cell border
    }
    for(unsigned int i = 0; i < M; ++i) mesh.addColor(colors[i]);
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  // draw background
  ofColor centerColor = ofColor(85, 78, 68);
  ofColor edgeColor(0, 0, 0);
  ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);
  if(count < 1) return;

  // draw circles
  for(unsigned int i = 0; i < pts.size(); ++i)
    ofCircle(pts[i].x, pts[i].y, 10);

  switch(dispMode){
    case '1':
      glPointSize(5.0f);
      mesh.drawVertices();
      // mesh.draw(OF_MESH_POINTS);
      break;
    case '2':
      mesh.drawWireframe();
      // mesh.draw(OF_MESH_WIREFRAME);
      break;
    case '3':
      //mesh.draw(OF_MESH_FILL);
      mesh.drawFaces();
      break;
    default:
      // draw cells
      ofxSegmentIterator it = voronoi.edges();
      for(; it; ++it){
        ofxSegment e = *it;
        ofLine(e.p1, e.p2);
      }
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  std::cout << "Pressed " << char(key) << " (int=" << key << ")\n";
  switch(key){
    case 'c':
      pts.clear();
      mesh.clear();
      count = 0; // commented to trigger voronoi update

      break;
    case 'f':
      ofToggleFullscreen();
      break;
    case '1':
    case '2':
    case '3':
    case '4':
      dispMode = key;
      break;
  }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  pts.push_back(ofVec2f(float(x), float(y)));
}

