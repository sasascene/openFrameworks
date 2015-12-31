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
    ofBackground(0, 200, 200, 200);
    //    ofEnableDepthTest();  // GUIが表示されない
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    //ベクトル場の初期化 (画面いっぱいに、横102コ、縦76コのベクトル)
    VF.setupField(102, 76, ofGetWidth(), ofGetHeight());
    
    // 頂点座標、色情報をセット
    for(int i = 0; i < NUM_PEOPLE; i++){
        ofVbo vbo;
        vbo.setVertexData(myVerts, NUM_VERTEX, GL_DYNAMIC_DRAW);
        vbo.setColorData(myColor, NUM_VERTEX, GL_DYNAMIC_DRAW);
        vbo_collection.push_back(vbo);
        
        // ofFloatColor color = ofFloatColor(ofRandom(1), ofRandom(1), ofRandom(1), 0.7);
        ofFloatColor color = ofFloatColor(0.8, 0.8, 1, 0.7);
        color_collection.push_back(color);
    }
    
    //　パーティクルを生成
    for (int i = 0; i < NUM_PERTICLES; i++){
        particle myParticle;
        myParticle.setInitialCondition(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0);
        particles.push_back(myParticle);
        
        // ノイズの種を登録
        offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
    }
    
    // 頂点座標、色情報をセット
    perticleVbo.setVertexData(perticleVerts, NUM_PERTICLES, GL_DYNAMIC_DRAW);
    perticleVbo.setColorData(perticleColor, NUM_PERTICLES, GL_DYNAMIC_DRAW);
    
    // shader
    myFbo.allocate(ofGetWidth(), ofGetHeight());
    myGlitch.setup(&myFbo);
    
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
    gui.add(min.setup("min", 30, 0, 400));
    gui.add(max.setup("max", 148, 0, 400));
    gui.add(threshold.setup("threshold", 240, 0, 400));
    gui.add(offset.setup("offset", 20, 1, 30));
    gui.add(contour.setup("contour", true));
}

// 人を検出する処理
void SceneA::detectHuman(){
    // CV設定
    contourFinder.setMinAreaRadius(min);
    contourFinder.setMaxAreaRadius(max);
    
    // Kinect状態更新
    kinect->update();
    
    if (kinect->isFrameNew()) {
        // ofxCvで輪郭抽出
        cv::Mat mat = cv::Mat(kinect->height, kinect->width, CV_8UC1, kinect->getDepthPixels(), 0);
        contourFinder.setThreshold(threshold);
        contourFinder.findContours(mat);
        
        // 人の輪郭コレクションをクリア
        humanContour_collection.clear();
        
        // 人を発見した場合
        if(contourFinder.size() > 0){
            // 全ての人をスキャンし、人ごとに次の処理を行う
            // 輪郭の抽出
            // 輪郭のフィールドを歪ませる（パーティクルが集まるように）
            for(int i = 0; i < contourFinder.size(); i++){
                
                // ひとり分の輪郭座標を保持する配列
                vector<ofVec2f> humanContour;
                
                // 人単位で輪郭の座標を取得(イテレータを使ったアクセス)
                vector<cv::Point> human = contourFinder.getContour(i);
                for (vector<cv::Point>::iterator it = human.begin(); it != human.end(); ++it)
                {
                    // ひとつひとつの輪郭座標を配列に追加
                    humanContour.push_back(ofVec2f(it->x, it->y));
                    
                    // ひとつひとつの輪郭座標を中心にフィールドを歪ませる
                    VF.addInwardCircle(it->x, it->y, 200, 0.5);
                }
                
                // ひとり分の輪郭座標を保持した配列をコレクションに追加
                humanContour_collection.push_back(humanContour);
            }
        }
        
    }
    
    // 人を検出した場合は輪郭の座標を取得
    for(int i = 0; i < humanContour_collection.size(); i++){
        
        ofFloatColor color = color_collection[i];
        
        vector<ofVec2f> humanContour = humanContour_collection[i];
        for(int j = 0, index = 0; j < humanContour.size(); j+=offset, index++){
            myVerts[index].set(humanContour[j].x, humanContour[j].y);
            myColor[index].set(color);
        }
        // 頂点座標を更新
        ofVbo *vbo = &vbo_collection[i];
        vbo->updateColorData(myColor, humanContour_collection[i].size());
        vbo->updateVertexData(myVerts, humanContour_collection[i].size());
    }
}

// パーティクルの更新
void SceneA::updatePerticle(){
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
        perticleColor[i].set(0, 0.5, 1, 0.8);
        // 座標をセット
        perticleVerts[i].set(particles[i].pos.x, particles[i].pos.y);
    }
    
    // パーティクルを更新
    perticleVbo.updateColorData(perticleColor, NUM_PERTICLES);
    perticleVbo.updateVertexData(perticleVerts, NUM_PERTICLES);
    
    // ベクターフィールドの減衰
    VF.fadeField(0.8f);
}

void SceneA::update(){
    
    // TODO:ここの処理を複数の関数に分割する
    // 人を検出
    detectHuman();
    
    // パーティクルの更新
    updatePerticle();
    
    // fboへの描画
//    myFbo.begin();
//    ofClear(0, 100, 100, 200);  // 画面の初期化
//
//    // VBOの描画
//    ofPushMatrix();
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    
//    // 描画位置の調整
//    ofScale(ofGetWidth() / float(kinect->width), ofGetHeight() / float(kinect->height));
//    ofTranslate(float(kinect->width), 0);
//    ofRotateY(180);
//    // 人の描画
//    for(int i = 0; i < humanContour_collection.size(); i++){
//        ofVbo *vbo = &vbo_collection[i];
//        vbo->draw(GL_TRIANGLE_STRIP, 0, humanContour_collection[i].size()/offset);
//    }
//    
//    // particleの描画
//    glPointSize(1.8);
//    perticleVbo.draw(GL_POINTS, 0, NUM_PERTICLES);
//    
//    ofDisableBlendMode();
//    ofPopMatrix();
    
//    myFbo.end();
    
    // エフェクトの選択
//    myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN,true);
//    myGlitch.setFx(OFXPOSTGLITCH_SWELL,true);
//    myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE,true);
//    myGlitch.setFx(OFXPOSTGLITCH_GLOW,true);
//    myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE,true);
//    myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER,true);

}

void SceneA::draw(){
    
    // エフェクトの適用
    needEffect = false;
    if(ofRandom(1)>0.5){
        needEffect = true;
    }
    if(needEffect){
        myGlitch.generateFx();
    }
    
    // fboの描画
//    myFbo.draw(0,0);
    
    // VBOの描画
    ofPushMatrix();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // 描画位置の調整
    ofScale(ofGetWidth() / float(kinect->width), ofGetHeight() / float(kinect->height));
    ofTranslate(float(kinect->width), 0);
    ofRotateY(180);
    // 人の描画
    for(int i = 0; i < humanContour_collection.size(); i++){
        ofVbo *vbo = &vbo_collection[i];
        vbo->draw(GL_TRIANGLE_STRIP, 0, humanContour_collection[i].size()/offset);
    }
    
    // particleの描画
    glPointSize(1.8);
    perticleVbo.draw(GL_POINTS, 0, NUM_PERTICLES);
    
    ofDisableBlendMode();
    ofPopMatrix();

    
    // GUIを表示
    if(valid_gui){
        gui.draw();
    }
}