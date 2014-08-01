/*
* OpenFramework wrapper for Shane O Sullivans' implementation of Steven Fortune's
* sweep line algorithm.
*
* Released under the MIT License (MIT)
*
* Copyright (c) 2014 Alexandre Kaspar <alexandre.kaspar@a3.epfl.ch>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once

#include <limits>
#include "ofMain.h"
#include "VoronoiDiagramGenerator.h"

struct ofxSegment {
    ofVec2f p1, p2;
};

class ofxSegmentIterator{
public:
    explicit ofxSegmentIterator(VoronoiDiagramGenerator *vdg) : v(vdg), hasNext(true){
        v->resetIterator();
        consume();
    }
    operator bool() const {
        return hasNext;
    }
    ofxSegment operator *() const {
        return e;
    }
    ofxSegmentIterator &operator++() {
        consume();
        return *this;
    }
private:
    void consume() {
        if(hasNext){
            hasNext = v->getNext(e.p1.x, e.p1.y, e.p2.x, e.p2.y);
        }
    }

    VoronoiDiagramGenerator *v;
    ofxSegment e;
    bool hasNext;
};

class ofxVoronoi2D {
public:
    template<typename T>
    void compute(const vector<T> &pts, const ofRectangle &bounds, float minDist = -1.0f){
        if(minDist < 0.0f) minDist = getMinDist(pts);
        vector<float> xPts, yPts;
        const int N = pts.size();
        xPts.reserve(N);
        yPts.reserve(N);
        for(int i = 0; i < N; ++i){
            const T &p = pts[i];
            xPts.push_back(p.x);
            yPts.push_back(p.y);
        }
        
        // real computation
        voro.generateVoronoi(&xPts[0], &yPts[0], N,
            bounds.getMinX(), bounds.getMaxX(),
            bounds.getMinY(), bounds.getMaxY(),
        minDist);
    }
    
    ofxSegmentIterator edges() {
        return ofxSegmentIterator(&voro);
    }
    
    // TODO ofxFaceIterator faces();
    
private:
    VoronoiDiagramGenerator voro;
    
    template<typename T>
    static float getMinDist(const vector<T> &pts) {
        // brute-force, no kd-tree or space splitting here
        // TODO use something smarter
        float minDist = std::numeric_limits<float>::max();
        for(int i = 0; i < pts.size() - 1; ++i){
            const T &p1 = pts[i];
            for(int j = i + 1; j < pts.size(); ++j){
                const T &p2 = pts[j];
                float d = ofDist(p1.x, p1.y, p2.x, p2.y);
                if(d < minDist) minDist = d;
            }
        }
        return minDist + std::numeric_limits<float>::epsilon();
    }
};

