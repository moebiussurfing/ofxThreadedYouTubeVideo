#include "ofxThreadedYouTubeVideo.h"

ofxThreadedYouTubeVideo::ofxThreadedYouTubeVideo()
{
	startThread();
}

ofxThreadedYouTubeVideo::~ofxThreadedYouTubeVideo()
{
	urls_to_load.close();
	waitForThread(true);
}

// Load a url
//--------------------------------------------------------------
void ofxThreadedYouTubeVideo::loadYouTubeURL(string _url, int _id)
{
	ofLogNotice(__FUNCTION__);

	ofYouTubeLoaderEntry entry(_url, _id);

	urls_to_load.send(entry);
}

//------------------------------------------------------------------------------
const string ofxThreadedYouTubeVideo::genRandomString(const int len) {
	ofLogNotice(__FUNCTION__);
	
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	string s;

	for (int i = 0; i < len; ++i) {
		char c = alphanum[rand() % (sizeof(alphanum) - 1)];
		s.push_back(c);
	}

	return s;
}

//------------------------------------------------------------------------------
const string ofxThreadedYouTubeVideo::doQuery(string text)
{
	ofLogNotice(__FUNCTION__);

	string url = "";
	size_t maxResults = 10;
	const string api_url = "https://www.googleapis.com/youtube/v3/search?q=";
	const string search_url = api_url + "\"v=" + text + "\"&key=" + API_KEY + "&part=snippet&maxResults=" + ofToString(maxResults) + "&";

	ofLogNotice("ofxThreadedYouTubeVideo") << "--------------------------------------------------" << endl;
	ofLogNotice("ofxThreadedYouTubeVideo") << "URL=" << search_url;

	if (!response.open(search_url)) {
		ofLogNotice("ofxThreadedYouTubeVideo") << "Failed to parse JSON\n";
	}
	else {
		//ofLogNotice("ofxThreadedYouTubeVideo") << "Response:\n" << response.toStyledString();
	}

	unsigned int numVideos = response["items"].size();
	unsigned int totalVideos = response["pageInfo"]["totalResults"].asUInt();
	ofLogNotice("ofxThreadedYouTubeVideo") << "Total videos = " << totalVideos;
	ofLogNotice("ofxThreadedYouTubeVideo") << "numVideos = " << numVideos;

	if (numVideos == 0) {
		ofLogError("ofxThreadedYouTubeVideo") << "No videos returned";
		return "";
	}

	int i = ofRandom(0, numVideos);

	Json::Value entryNode = response["items"][i];

	ofLogNotice("ofxThreadedYouTubeVideo") << "title = " << entryNode["snippet"]["title"].asString();
	ofLogNotice("ofxThreadedYouTubeVideo") << "video id = " << entryNode["id"]["videoId"].asString();
	ofLogNotice("ofxThreadedYouTubeVideo") << "--------------------------------------------------";

	url = "https://www.youtube.com/watch?v=" + entryNode["id"]["videoId"].asString();

	return url;
}

//------------------------------------------------------------------------------
const string ofxThreadedYouTubeVideo::getRandomURL()
{
	ofLogNotice(__FUNCTION__);

	//const string search_url = "https://www.googleapis.com/youtube/v3/search?q=\"v="+genRandomString(4)+"\"&key=AIzaSyDGYA7woinSUM_eQStrJWgLaCA5fugJ3IA&part=snippet&maxResults=50&";

	//const string search_url = "https://www.googleapis.com/youtube/v3/search?q=\"v=" + genRandomString(4) + "\"&key=YOUR_NEW_API_KEY&part=snippet&maxResults=50&";

	string text = genRandomString(4);
	string url = "";
	size_t maxResults = 10;
	const string api_url = "https://www.googleapis.com/youtube/v3/search?q=";
	const string search_url = api_url + "\"v=" + text + "\"&key=" + API_KEY + "&part=snippet&maxResults=" + ofToString(maxResults) + "&";

	ofLogNotice("ofxThreadedYouTubeVideo") << "--------------------------------------------------" << endl;
	ofLogNotice("ofxThreadedYouTubeVideo") << "URL=" << search_url;

	if (!response.open(search_url)) {
		ofLogNotice("ofxThreadedYouTubeVideo") << "Failed to parse JSON\n";
	}
	else {
		//ofLogNotice("ofxThreadedYouTubeVideo") << "Response:\n" << response.toStyledString();
	}

	unsigned int numVideos = response["items"].size();
	unsigned int totalVideos = response["pageInfo"]["totalResults"].asUInt();
	ofLogNotice("ofxThreadedYouTubeVideo") << "Total videos = " << totalVideos;
	ofLogNotice("ofxThreadedYouTubeVideo") << "numVideos = " << numVideos;

	if (numVideos == 0) {
		ofLogError("ofxThreadedYouTubeVideo") << "No videos returned";
		return "";
	}

	int i = ofRandom(0, numVideos);

	Json::Value entryNode = response["items"][i];

	ofLogNotice("ofxThreadedYouTubeVideo") << "title = " << entryNode["snippet"]["title"].asString();
	ofLogNotice("ofxThreadedYouTubeVideo") << "video id = " << entryNode["id"]["videoId"].asString();
	ofLogNotice("ofxThreadedYouTubeVideo") << "--------------------------------------------------";

	url = "https://www.youtube.com/watch?v=" + entryNode["id"]["videoId"].asString();

	return url;
}

// Get a new url - called from within thread
//--------------------------------------------------------------
bool ofxThreadedYouTubeVideo::getNewURL(ofYouTubeLoaderEntry& entry)
{
	ofLogNotice(__FUNCTION__);

	string new_url = entry.input_url;

	if (new_url == "") {
		new_url = getRandomURL();
		if (new_url == "") {
			ofLogError("ofxThreadedYouTubeDownloader") << "Empty new URL!";
			return false;
		}
	}

	// original
	//string commandVideoUrl = "youtube-dl -g -f 18 \"" + new_url + "\"";

	string args_url_YT = ofToString(" \"") + new_url + ofToString("\"");

	string app_exe = "yt-dlp";
	//string app_exe = "youtube-dl";

	string args_opt = ofToString("--verbose");
	//string args_opt = ofToString("--verbose --list-formats");

	// yt-dlp -f best [URL]
	// yt-dlp -f bestaudio "https://www.youtube.com/watch?v=dQw4w9WgXcQ"
	// yt-dlp -r 50K "https://www.youtube.com/watch?v=dQw4w9WgXcQ"
	// yt-dlp -f bestvideo "https://www.youtube.com/watch?v=dQw4w9WgXcQ"
	// yt-dlp -f bestvideo[ext=mp4] "https://www.youtube.com/watch?v=dQw4w9WgXcQ"
	// yt-dlp -f bestvideo[ext=mp4]+bestaudio "https://www.youtube.com/watch?v=dQw4w9WgXcQ"

	string args_command = "-f bestvideo[ext=mp4]"; // V mp4
	//string args_command = "-f bestvideo[ext=mp4]+bestaudio"; // AV mkv
	//string args_command = "-f bestvideo";
	//string args_command = "-f best";
	//string args_command = "-f b";
	//string args_command = "-g -f 18"; //This command will print the direct URL for the specified video in MP4 format with a resolution of 640x360 and an audio bitrate of 96 kbps, without actually downloading the video.

	string path = ofToDataPath("video.mp4");
	string arg_path = "-o " + path;
	args_opt += string(" ") + arg_path;

	string commandVideoUrl = app_exe + string(" ") + args_opt + string(" ") + args_command + args_url_YT;

	ofLogNotice("ofxThreadedYouTubeVideo") << commandVideoUrl << endl;

	FILE* in;
	char buff[2048];

	if (!(in = _popen(commandVideoUrl.c_str(), "r"))) {
		ofLogError("ofxThreadedYouTubeVideo") << "failed to popen" << endl;
		return false;
	}

	while (fgets(buff, sizeof(buff), in) != NULL) {
		;//cout << buff;
	}
	_pclose(in);

	entry.bLoaded = false;
	commandVideoUrl = buff;
	commandVideoUrl.erase(std::remove_if(commandVideoUrl.begin(), commandVideoUrl.end(), ::isspace), commandVideoUrl.end());
	//commandVideoUrl = "\"" + commandVideoUrl + "\"";

	entry.url = commandVideoUrl;

	//TODO: not sure we can load from an url directly..
	//TODO: we should get the path where the file is saved..
	entry.path = path;
	//entry.path = "../Rick Astley - Never Gonna Give You Up (Official Music Video) [dQw4w9WgXcQ].mp4";
	//entry.path = "../Rick Astley - Never Gonna Give You Up (Official Music Video) [dQw4w9WgXcQ].mkv";

	return true;
}

void ofxThreadedYouTubeVideo::threadedFunction()
{
	ofLogNotice(__FUNCTION__);

	setThreadName("ofxThreadedYouTubeVideo " + ofToString(thread.get_id()));

	while (isThreadRunning()) {

		ofYouTubeLoaderEntry entry;
		while (urls_to_load.receive(entry)) {

			if (!getNewURL(entry)) {
				ofLogError("ofxThreadedYouTubeVideo") << "Couldn't load url: \n\"" << entry.input_url << "\"";
				//get another random video and try again
				loadYouTubeURL("", entry.id);
			}
			else {
				ofLogNotice("ofxThreadedYouTubeVideo") << "Got video url: \n" << entry.url << endl;
				ofVideoPlayer* vid = new ofVideoPlayer();
				vid->setUseTexture(false);
				vid->load(entry.path);
				//vid->load(entry.url);

				//ofxYouTubeURLEvent e = ofxYouTubeURLEvent(entry.url, entry.id, vid);
				ofxYouTubeURLEvent e = ofxYouTubeURLEvent(entry.path, entry.id, vid);
				ofNotifyEvent(youTubeURLEvent, e, this);
			}
		}
	} // is thread running
}
