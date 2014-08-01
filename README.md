ofxVoronoi2D
============

OpenFramework addon to use the 2D Voronoi implementation of Shane O Sullivans

Usage
-----
Simply put:
  0. Create
  1. Computation from the seed points
  2. Cell usage

```cpp
// 0. Instance
ofxVoronoi2D voronoi;
vector<ofVec2f> pts;
// init pts
// ...

// 1. Voronoi computation
voronoi.compute(pts, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));

// 2. Edge drawing
for(ofxSegmentIterator it = voronoi.edges(); it; ++it){
  ofxSegment s = *it;
  ofLine(s.p1, s.p2);
}
```

TODO
----
  - generate Voronoi cells (possibly linked to the original points)
  - provide example directory with a basic project
  - faster minDist computation (here brute-force = O(n²)
  - smarter point usage to avoid having to go through the points to split into xPts and yPts (or fix in original code)

