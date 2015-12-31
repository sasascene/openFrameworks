//
//  SceneA.cpp
//  Violon_02
//
//  Created by sasascene on 2015/08/22.
//
//

#include "SceneA.h"

void SceneA::setup(){
    
    // GUIを設定
    initGui();
    
    // 描画設定
    ofSetVerticalSync(false);
    ofSetFrameRate(24);
    ofBackground(0, 120, 200, 200);
//    ofEnableDepthTest();  // GUIが表示されない
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    //ベクトル場の初期化 (画面いっぱいに、横102コ、縦76コのベクトル)
    VF.setupField(102, 76, ofGetWidth(), ofGetHeight());
    
    //　パーティクルを生成
    for (int i = 0; i < NUM_PERTICLES; i++){
        particle myParticle;
        myParticle.setInitialCondition(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0);
        particles.push_back(myParticle);
        
        // ノイズの種を登録
        offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
    }
    
    // 頂点座標、色情報をセット
    myVbo.setVertexData(myVerts, MAX_NUM_VERTEX, GL_DYNAMIC_DRAW);
    myVbo.setColorData(myColor, MAX_NUM_VERTEX, GL_DYNAMIC_DRAW);
    
}

// GUIを設定
void SceneA::initGui(){
    // colorの初期値、最小値、最大値を設定
    ofColor initColor = ofColor(0, 127, 255, 255);
    ofColor minColor = ofColor(0,0,0,0);
    ofColor maxColor = ofColor(255,255,255,255);
    
    // positionの初期値、最小値、最大値を設定
    ofVec2f initPos = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
    ofVec2f minPos = ofVec2f(0, 0);
    ofVec2f maxPos = ofVec2f(ofGetWidth(), ofGetHeight());
    
    gui.setup();
    gui.add(radius.setup("radius", 200, 0, 400));
    gui.add(color.setup("color", initColor, minColor, maxColor));
    gui.add(position.setup("position", initPos, minPos, maxPos));
    gui.add(contour.setup("contour", false));
}

void SceneA::update(){
    // CV設定
    contourFinder.setMinAreaRadius(100);
    contourFinder.setMaxAreaRadius(300);
    
    // Kinect状態更新
    kinect->update();
    
    if (kinect->isFrameNew()) {
        // ofxCvで輪郭抽出
        cv::Mat mat = cv::Mat(kinect->height, kinect->width, CV_8UC1, kinect->getDepthPixels(), 0);
        contourFinder.setThreshold(155);
        contourFinder.findContours(mat);
        
        // 人の輪郭に沿ってベクターフィールドを更新
        humanContour.clear();
        if(contourFinder.size() > 0){
            for(int i = 0; i < contourFinder.size(); i++){
                // 人単位で輪郭の座標を取得(イテレータを使ったアクセス)
                vector<cv::Point> human = contourFinder.getContour(i);
                for (vector<cv::Point>::iterator it = human.begin(); it != human.end(); ++it)
                {
                    // 人の輪郭にベクターフィールドを生成
                    VF.addInwardCircle(it->x, it->y, 100, 0.5);
                    
                    // 人の輪郭座標をベクタに格納
                    humanContour.push_back(ofVec2f(it->x, it->y));
                }
            }
        }
        
    }
    
    
    // パーティクル
    for (int i = 0; i < NUM_PERTICLES; i++){
        
        // パーティクルの力をリセット
        particles[i].resetForce();
        
        // ベクターフィールドからそれぞれのパーティクルにかかる力を取得
        ofVec2f frc = VF.getForceFromPos(particles[i].pos.x, particles[i].pos.y);
        
        // ノイズの発生
        float time = ofGetElapsedTimef();
        float timeScale = 20.0;
        float displacementScale = 0.75;
        ofVec3f timeOffsets = offsets[i];
        float noizx = (ofSignedNoise(time*timeScale+timeOffsets.x)) * displacementScale;
        float noizy = (ofSignedNoise(time*timeScale+timeOffsets.y)) * displacementScale;
        float noizz = (ofSignedNoise(time*timeScale+timeOffsets.z)) * displacementScale;
        
        // パーティクルの状態を更新
        particles[i].addForce(noizx + frc.x, noizy + frc.y); // ノイズを加算
        particles[i].addDampingForce();
        particles[i].bounceOffWalls();
        particles[i].update();
        
        // VBO用にパーティクルの座標をセット
        // 色情報をセット
        myColor[i].set(0, 0.5, 1, 0.8);
        // 座標をセット
        myVerts[i].set(particles[i].pos.x, particles[i].pos.y);
    }
    
    // 人を検出した場合は輪郭の座標を取得
    offset = 0;
    if(humanContour.size() > 0){
        for(int i = 0; i < humanContour.size(); i+=10){
            myVerts[NUM_PERTICLES + offset].set(humanContour[i].x, humanContour[i].y);
            myColor[NUM_PERTICLES + offset].set(0,0.5,1,0.9);
            offset++;
        }
    }
    
    // 頂点座標を更新
    myVbo.updateColorData(myColor, NUM_PERTICLES + humanContour.size());
    myVbo.updateVertexData(myVerts, NUM_PERTICLES + humanContour.size());
    
    // ベクターフィールドの減衰
    VF.fadeField(0.5f);
}

void SceneA::draw(){
    
    // VBOの描画
    ofPushMatrix();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
            // 描画位置の調整
            ofScale(ofGetWidth() / float(kinect->width), ofGetHeight() / float(kinect->height));
    
            // particleの描画
            glPointSize(2);
            myVbo.draw(GL_POINTS, 0, NUM_PERTICLES);
    
            // 輪郭の描画
            if(contour){
                glPointSize(1);
                myVbo.draw(GL_TRIANGLES, NUM_PERTICLES, offset);
            }
        ofDisableBlendMode();
    ofPopMatrix();
    
    // GUIを表示
    gui.draw();
}