#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"

#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxOsc.h"

#include "BaseScene.h"
#include "SceneA.h"
#include "SceneB.h"
#include "SceneC.h"

// listen on port 10000
#define PORT 10000

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // OSC
    ofxOscReceiver receiver;
    
    // kinect関係
    ofxKinect kinect; //Kinectインスタンス
    ofImage kinectImage; // Kinectカメラ映像(未使用)
    ofImage depthImage; // Kinect深度映像(未使用)
    ofxCv::ContourFinder contourFinder; //CV輪郭抽出
    
    // シーンスイッチ
    vector <BaseScene *> scenes;
    int currentScene;
    
    // gui
    bool valid_gui;
    
    // Syphon
    ofxSyphonServer server;
    
};
