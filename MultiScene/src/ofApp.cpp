#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    
    // A
    BaseScene *sa = new SceneA();
    sa->setup();
    scenes.push_back(sa);
    // B
    BaseScene *sb = new SceneB();
    sb->setup();
    scenes.push_back(sb);
    // C
    BaseScene *sc = new SceneC();
    sc->setup();
    scenes.push_back(sc);
    
    currentScene = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    scenes[currentScene]->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    scenes[currentScene]->draw();
    
//    ofSetColor(0, 0, 1);
//    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
//    ofBoxPrimitive(ofGetWidth()/4, ofGetHeight()/4, ofGetHeight()/4);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '1':
            currentScene = 0;
            break;
        case '2':
            currentScene = 1;
            break; 
            
        case '3':
            currentScene = 2;
            break;
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
