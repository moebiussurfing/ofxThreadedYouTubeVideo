#pragma once

#include "ofMain.h"
#include "ofxThreadedYouTubeVideo.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void onVideoLoaded(ofxYouTubeURLEvent& event);

    ofxThreadedYouTubeVideo loader;
    ofVideoPlayer video;
};
