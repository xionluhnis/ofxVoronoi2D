#include "ofApp.h"
#include <iostream>
#define NUM_CIRCLES 7

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetFrameRate(24);
  count = 0;
  dispMode = 0;
  ofEnableBlendMode(OF_BLENDMODE_ALPHA);
  // voronoi.setMinDist(std::epsilon<float>());
  voroMode = true;
  cam.disableMouseInput();
  cam.setVFlip(true);
  zJitter = 0.0f;
  
  // setup UI
  ui.setModal(true);
  ui.addLabel("<h> toggle help");
  ui.addLabel("<c> clear seeds");
  ui.addLabel("<v> switch camera/voronoi");
  ui.addLabel("<f> toggle fullscreen");
  ui.addLabel("<1-4> display mode");
  ui.addSpacer();
  ui.addMinimalSlider("Z Jitter", 0.0f, 10.0f, &zJitter);
  vector<ofxUIWidget*> widgets = ui.getWidgets();
  for(unsigned int i = 0; i < widgets.size(); ++i) widgets[i]->setPadding(20.0f);
  ui.setPosition(10, 10);
  ui.autoSizeToFitWidgets();
  ui.centerWidgetsOnCanvas();
  ofAddListener(ui.newGUIEvent, this, &ofApp::guiEvent); 
  ui.loadSettings("settings.xml");  
}

//--------------------------------------------------------------
void ofApp::exit(){
  ui.saveSettings("settings.xml"); 
}

//--------------------------------------------------------------
void ofApp::update(){
  // jitter
  float jitterIncr = 0.0f;
  if(ofGetKeyPressed(OF_KEY_RIGHT)) jitterIncr = 1.0f;
  else if(ofGetKeyPressed(OF_KEY_LEFT)) jitterIncr = -1.0f;
  if(jitterIncr != 0.0f){
    zJitter = std::min(10.0f, std::max(0.0f, zJitter + jitterIncr * 0.3f));
  }
  // resize event
  if(lastW != ofGetWindowWidth() || lastH != ofGetWindowHeight()){
    lastW = ofGetWindowWidth();
    lastH = ofGetWindowHeight();
    cam.setTarget(ofVec3f(lastW, lastH) * 0.5f);
  }
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
      colors[mesh.getIndex(i)] = c;
      for(int j = 1; j < 3; ++j){
        colors[mesh.getIndex(i + j)] = ofFloatColor(1.0f, 1.0f, 1.0f, 0.0f);
      }
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

  if(!voroMode) cam.begin();
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
      // 3d model
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
  if(!voroMode) cam.end();
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e) {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  std::cout << "Pressed " << char(key) << " (int=" << key << ")\n";
  switch(key){
    case 'c':
      pts.clear();
      mesh.clear();
      count = 0;
      break;
    case 'f':
      ofToggleFullscreen();
      break;
    case 'h':
      ui.toggleVisible();
      break;
    case 'v':
      voroMode = !voroMode;
      if(voroMode){
        cam.disableMouseInput();
      } else {
        cam.enableMouseInput();
      }
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
void ofApp::keyReleased(int key){
  float dir = 1.0f;
  switch(key){
    case OF_KEY_LEFT:
      dir = -1.0f;
    case OF_KEY_RIGHT:
      
      break;
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
  if(!cam.getMouseInputEnabled()){
    pts.push_back(ofVec2f(float(x), float(y)));
  }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
