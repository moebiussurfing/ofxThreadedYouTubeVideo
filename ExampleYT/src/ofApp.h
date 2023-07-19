#pragma once

#include "ofMain.h"
#include "ofxThreadedYouTubeVideo.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    void onVideoLoaded(ofxYouTubeURLEvent& event);
    ofxThreadedYouTubeVideo loader;
    ofVideoPlayer video;

    void drawBg();
    bool bWaiting = 1;
    bool bError = 0;
    // Bg flash. set to 1 to start.
    float v = 0;
    ofParameter<ofColor> colorBg{"ColorBg", ofColor(48), ofColor(), ofColor()};
};
