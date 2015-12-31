#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // シーン設定
    sceneSetup();
    
    // サウンド初期設定
    gain = 2;
    sound_init();
}

// シーン設定
void ofApp::sceneSetup(){
    // A
    BaseScene *sa = new SceneA();
    sa->setup();
    scenes.push_back(sa);
    // B
    BaseScene *sb = new SceneB();
    sb->setup();
    scenes.push_back(sb);
    
    // 初期パターン、シーンの指定
    currentScene = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    // サウンド取得
    try{
        sound_get();
    }catch (...){
        
    }
    
    // 現在のシーンに音量とスペクトルを渡す
    scenes[currentScene]->pLevel = &level;
    scenes[currentScene]->pMagnitude = magnitude;
    scenes[currentScene]->pLeft = left;
    scenes[currentScene]->pRight = right;
    
    // 現在のシーンを更新
    scenes[currentScene]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // 現在のシーンを描画
    scenes[currentScene]->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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

// サウンド初期設定
void ofApp::sound_init(){
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // BUFFER_SIZE samples per buffer
    // 4 num buffers (latency)
    
    ofSoundStreamSetup(0,2,this, 44100,BUFFER_SIZE, 4);
    left = new float[BUFFER_SIZE];
    right = new float[BUFFER_SIZE];
    
    for (int i = 0; i < NUM_WINDOWS; i++)
    {
        for (int j = 0; j < BUFFER_SIZE/2; j++)
        {
            freq[i][j] = 0;
        }
    }
}

// サウンド取得
void ofApp::sound_get(){
    static int index=0;
    float avg_power = 0.0f;
    
    if(index < 80)
        index += 1;
    else
        index = 0;
    
    /* do the FFT */
    myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
    
    /* start from 1 because mag[0] = DC component */
    /* and discard the upper half of the buffer */
    level = 0;
    for(int j=1; j < BUFFER_SIZE/2; j++) {
        freq[index][j] = magnitude[j]*gain*3;
        level += magnitude[j];
        magnitude[j] *= gain*2;
    }
    level = level*gain;
}

// サウンド取得時イベント
void ofApp::audioReceived(float * input, int bufferSize, int nChannels){
    // samples are "interleaved"
    for (int i = 0; i < bufferSize; i++){
        left[i] = input[i*2];
        right[i] = input[i*2+1];
    }
    bufferCounter++;
}