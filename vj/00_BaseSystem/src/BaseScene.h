//
//  BaseScene.h
//  vj
//
//  Created by sasascene on 2014/12/13.
//
//

#ifndef __vj__BaseScene__
#define __vj__BaseScene__

#pragma once

#include "ofMain.h"

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
    
    // サウンド
    float *pLevel;
    float *pMagnitude;
    float *pLeft;
    float *pRight;
    
    // コントローラ
    int *pKnob;
    int *pFader;
    int *pSolo;
    int *pMute;
    int *pRec;
    
};

#endif /* defined(__vj__BaseScene__) */
