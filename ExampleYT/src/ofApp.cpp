#include "ofApp.h"

void ofApp::setup() {
	// you should replace this url with an actual youtube video url
	string URL_YT = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
	loader.loadYouTubeURL(URL_YT, 1);

	// Add an event listener to handle the video loaded event.
	ofAddListener(loader.youTubeURLEvent, this, &ofApp::onVideoLoaded);
}

void ofApp::update() {
	if (video.isLoaded())
		video.update();
}

void ofApp::draw() {
	if (video.isLoaded())
		video.draw(0, 0);
}

// Function to handle the loaded video
void ofApp::onVideoLoaded(ofxYouTubeURLEvent& event) {
	if (event.id == 1) {
		video = *event.vid;
		video.play();
	}
}
