//
//  SceneB.h
//  Violon_02
//
//  Created by sasascene on 2015/08/22.
//
//

#ifndef __MultiScene__SceneB__
#define __MultiScene__SceneB__

#pragma once
#include "ofMain.h"
#include "BaseScene.h"

#include "ofxBox2d.h"

#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

#include "CustomRect.h"
#include "vectorField.h"
#include "particle.h"

// BaseSceneを継承したSceneB

class SceneB : public BaseScene {
    
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
    
    ofxBox2d box2d; // Box2Dの世界
    vector <ofPtr<ofxBox2dCircle> > circles;
    vector <ofPtr<ofxBox2dRect> > boxes;
    
    ofImage myImage;
    vector <ofImage> images;
    vector <ofPtr<CustomRect> >drops;
    vector <ofPtr<CustomRect> >rects;

    // 人のかたちをしたポリゴン
    vector <ofPtr<ofxBox2dPolygon> > humanContour_collection_in_box2d;
    
    float rain = 0;
    
    static const int NUM_VERTEX = 30000;
    vector<ofVbo> vbo_collection;           // vboのコレクション
    vector<ofFloatColor> color_collection;  // 色情報のコレクション
    ofVec3f myVerts[NUM_VERTEX];         // 頂点座標
    ofFloatColor myColor[NUM_VERTEX];    // 頂点の色情報
    
    // 人の輪郭
    static const int NUM_PEOPLE = 6;
    vector<vector<ofVec2f> > humanContour_collection;   // 人の輪郭コレクション
    void detectHuman(); // 人を検出する処理
    
    // オブジェクトの作成
    void createObject();
    // 画面から消えたオブジェクトの削除
    void killObject();
    
    void makeWind();

};

#endif /* defined(__MultiScene__SceneB__) */
