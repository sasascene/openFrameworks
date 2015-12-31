//
//  BaseScene.h
//  MultiScene
//
//  Created by sasascene on 2014/12/13.
//
//

#ifndef __MultiScene__BaseScene__
#define __MultiScene__BaseScene__

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxOsc.h"

// 全てのシーンの雛形

class BaseScene{

public:
    
    //仮想関数 (virtual) として定義する
    virtual void setup(){};
    virtual void update(){};
    virtual void draw(){};
    virtual void keyPressed (int key){};
    virtual void keyReleased(int key){};
    virtual void mouseMoved(int x, int y ){};
    virtual void mouseDragged(int x, int y, int button){};
    virtual void mousePressed(int x, int y, int button){};
    virtual void mouseReleased(int x, int y, int button){};
    virtual void windowResized(int w, int h){};
    
    // 画像インデックス
    int *pPictureIndex;
    
    // 画像コレクション
    vector<ofImage> images;
    
    // kinect関係
    ofxKinect kinect; //Kinectインスタンス
    ofImage kinectImage; // Kinectカメラ映像
    ofImage depthImage; // Kinect深度映像
    ofxCv::ContourFinder contourFinder; //CV輪郭抽出
    
};

#endif /* defined(__MultiScene__BaseScene__) */
