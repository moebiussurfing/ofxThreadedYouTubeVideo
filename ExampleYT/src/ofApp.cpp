#include "ofApp.h"

void ofApp::setup() {
	ofLogNotice(__FUNCTION__);
	
	// you should replace this url with an actual youtube video url
	string URL_YT = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
	loader.loadYouTubeURL(URL_YT, 1);
	v = 1;

	// Add an event listener to handle the video loaded event.
	ofAddListener(loader.youTubeURLEvent, this, &ofApp::onVideoLoaded);
}

void ofApp::update() {
	if (video.isLoaded()) video.update();
}

void ofApp::draw() {
	drawBg();

	if (video.isLoaded()) video.draw(0, 0);
}

// Function to handle the loaded video
void ofApp::onVideoLoaded(ofxYouTubeURLEvent& event) {
	ofLogNotice(__FUNCTION__);

	if (event.id == 1) 
	{
		video = *event.vid;
		video.play();
	}
}

void ofApp::keyPressed(int key)
{
	ofLogNotice("ofApp") << "keyPressed:" << char(key);

	if (key == '1') {
		loader.doRandom();
		v = 1;
	}

	if (key == '2') {
		ofLogNotice("ofApp") << loader.doQuery("nirvana");
		v = 1;
	}
	if (key == '3') {
		ofLogNotice("ofApp") << loader.doQuery("Portishead_music");
		v = 1;
	}
	if (key == '4') {
		ofLogNotice("ofApp") << loader.doQuery("massive_attack");
		v = 1;
	}
}

void ofApp::drawBg()
{
	if (bError)
	{
		// Red if error.
		float v = glm::cos(10 * ofGetElapsedTimef());
		float a1 = ofMap(v, -1, 1, 100, 200, true);
		ofColor c = ofColor(a1, 0, 0);
		ofClear(c);
	}
	else if (bWaiting)
	{
		// Fade blink when waiting. 
		ofColor c = colorBg.get();
		//ofColor c = bigTextInput.getColor();
		auto br = c.getBrightness();
		float g = 5; //gap
		float v1 = glm::cos(10 * ofGetElapsedTimef());
		float a1 = ofMap(v1, -1, 1, br - g, br, true);
		//float a1 = ofMap(v1, -1, 1, 100, 150, true);
		c.setBrightness(a1);
		ofClear(c);
	}
	else
	{
		// Flash when submit
		if (v > 0) v -= 0.05f;
		else v = 0;
		int bgMin = 100;
		if (v > 0) ofClear(bgMin + (255 - bgMin) * v);
		else
		{
			ofClear(colorBg.get());
		}
	}
}