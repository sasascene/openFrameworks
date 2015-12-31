//
//  SceneC.h
//  Violon_02
//
//  Created by sasascene on 2015/08/22.
//
//

#ifndef __MultiScene__SceneC__
#define __MultiScene__SceneC__

#pragma once
#include "ofMain.h"
#include "BaseScene.h"

#include "vectorField.h"
#include "particle.h"

#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

#include "ofxPostGlitch.h"
#include "ofxDelaunay.h"

// BaseSceneを継承したSceneC

class SceneC : public BaseScene {
    
public:
    void setup();
    void update();
    void draw();
    
    // GUIのパラメーター
    void initGui();
    ofxPanel gui;
    ofxFloatSlider min;
    ofxFloatSlider max;
    ofxFloatSlider threshold;
    ofxIntSlider offset;
    ofxToggle contour;
    
    // VBO関係
    static const int NUM_VERTEX = 30000;
    vector<ofVbo> vbo_collection;           // vboのコレクション
    vector<ofFloatColor> color_collection;  // 色情報のコレクション
    ofVec3f myVerts[NUM_VERTEX];         // 頂点座標
    ofFloatColor myColor[NUM_VERTEX];    // 頂点の色情報
    
    // 人の輪郭
    static const int NUM_PEOPLE = 6;
    vector<vector<ofVec2f> > humanContour_collection;   // 人の輪郭コレクション
    void detectHuman(); // 人を検出する処理
    
    // ベクタフィールド関係
    vectorField VF;
    vector<particle> particles; // パーティクル
    vector<ofVec3f> offsets;    // ノイズの種
    
    // パーティクル
    static const int NUM_PERTICLES = 100;
    ofVbo perticleVbo;    // VBO
    ofVec3f perticleVerts[NUM_PERTICLES];         // 頂点座標
    ofFloatColor perticleColor[NUM_PERTICLES];    // 頂点の色情報
    void updatePerticle();  // パーティクルの更新
    
    // Delaunay;
    ofxDelaunay delaunay;
    vector<ofxDelaunay> delaunays;
    
    // shader
    ofShader shader;
    
    // エフェクト関係
    ofFbo myFbo;
    ofxPostGlitch myGlitch;
    bool needEffect;

};

#endif /* defined(__MultiScene__SceneC__) */
