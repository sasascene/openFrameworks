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

// BaseSceneを継承したSceneB

class SceneB : public BaseScene {
    
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
    
    ofxBox2d box2d; // Box2Dの世界
    vector <ofPtr<ofxBox2dCircle> > circles;
    vector <ofPtr<ofxBox2dRect> > boxes;
    
    // 人のかたちをしたポリゴン
    ofPolyline humanContour;
    ofPtr<ofxBox2dPolygon> humanContour_in_box2d;
    
    // オブジェクトの作成
    void createObject();
    // 画面から消えたオブジェクトの削除
    void killObject();

};

#endif /* defined(__MultiScene__SceneB__) */
