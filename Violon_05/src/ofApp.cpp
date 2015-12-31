#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // 各シーンに共通した初期化処理
    ofSetFrameRate(24);
    
    // カメラ画像と深度カメラのずれを解消
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    // シーンごとの初期化
    // A
    BaseScene *sa = new SceneA();
    sa->kinect = &kinect;
    sa->setup();
    scenes.push_back(sa);
    // B
    BaseScene *sb = new SceneB();
    sb->kinect = &kinect;
    sb->setup();
    scenes.push_back(sb);
    // C
    BaseScene *sc = new SceneC();
    sc->kinect = &kinect;
    sc->setup();
    scenes.push_back(sc);
    
    // 初期シーン
    currentScene = 0;
    
    // gui
    valid_gui = true;
    
    // OSC
    receiver.setup(PORT);
    
    // Syphon
    server.setName("from openFrameworks");

}

//--------------------------------------------------------------
void ofApp::update(){
    // OSC
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        // check for mouse moved message
        if(m.getAddress() == "/mode"){
            // both the arguments are int32's
            int value = m.getArgAsInt32(0);
            
            currentScene = value;
        }
    }
    
    
    // 現在のシーンを更新
    scenes[currentScene]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // 現在のシーンを描画
    scenes[currentScene]->draw();
    
    // Syohon
    server.publishScreen();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // シーンスイッチ
    if(key == 's'){
        currentScene++;
        
        int max = 2;
        if(currentScene > max){
            currentScene = 0;
        }
    }
    
    // gui
    if(key == 'g'){
        valid_gui = not(valid_gui);
        for(int i = 0 ; i < scenes.size(); i++){
            scenes[i]->valid_gui = valid_gui;
        }
    }
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
