#pragma once

#include "ofMain.h"

// サウンド
#include "sound_input.h"

// シーン切替
#include "BaseScene.h"
#include "SceneA.h"
#include "SceneB.h"

// サウンド定数
#define BUFFER_SIZE 1024
#define NUM_WINDOWS 80

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
    
    // シーン切替
    void sceneSetup();
    vector <BaseScene *> scenes;
    int currentScene;
    
    // サウンド関連
    void sound_init();
    void sound_get();
    void audioReceived  (float * input, int bufferSize, int nChannels);
    float * left;
    float * right;
    float gain;
    float level;
    float timeline_array[257][2];
    int  bufferCounter;
    fft  myfft;
    float phase[BUFFER_SIZE];
    float power[BUFFER_SIZE];
    float freq[NUM_WINDOWS][BUFFER_SIZE/2];
    float freq_phase[NUM_WINDOWS][BUFFER_SIZE/2];
    float magnitude[BUFFER_SIZE];

    
};
