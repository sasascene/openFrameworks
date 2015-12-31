//
//  SceneA.h
//  Violon_02
//
//  Created by sasascene on 2015/08/22.
//
//

#ifndef __MultiScene__SceneA__
#define __MultiScene__SceneA__

#pragma once
#include "ofMain.h"
#include "BaseScene.h"

#include "vectorField.h"
#include "particle.h"

#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

// BaseSceneを継承したSceneA

class SceneA : public BaseScene {
    
public:
    void setup();
    void update();
    void draw();
    
    // GUIのパラメーター
    void initGui();
    ofxPanel gui;
    ofxFloatSlider radius;
    ofxColorSlider color;
    ofxVec2Slider position;
    ofxToggle contour;
    
    // ベクタフィールド関係
    vectorField VF;
    vector<particle> particles; // パーティクル
    vector<ofVec3f> offsets;    // ノイズの種
    
    // VBO関係
    static const int NUM_PERTICLES = 100000;
    static const int MAX_NUM_VERTEX = 120000;
    ofVbo myVbo;    // VBO
    ofVec3f myVerts[MAX_NUM_VERTEX];         // 頂点座標
    ofFloatColor myColor[MAX_NUM_VERTEX];    // 頂点の色情報
    int offset = 0; // 人を描画する際に使用
    
    // 人の輪郭
    vector<ofVec2f> humanContour;

    
};

#endif /* defined(__MultiScene__SceneA__) */
