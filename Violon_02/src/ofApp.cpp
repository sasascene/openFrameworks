#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // 各シーンに共通した初期化処理
    
    // シーンごとの初期化
    // A
    BaseScene *sa = new SceneA();
//    sa->setup();
    scenes.push_back(sa);
    // B
    BaseScene *sb = new SceneB();
    sb->setup();
    scenes.push_back(sb);
    
    // 初期シーン
    currentScene = 1;
    
    // Syphon
    server.setName("from openFrameworks");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // シーンスイッチ
//    float flg = ofRandom(1);
//    if(flg>0.5){
//        currentScene = 0;
//    }else{
//        currentScene = 1;
//    }
    
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
