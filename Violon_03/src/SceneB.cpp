//
//  SceneB.cpp
//  Violon_02
//
//  Created by sasascene on 2015/08/22.
//
//

#include "SceneB.h"

void SceneB::setup(){
    
    // GUIを設定
    initGui();
    
    // 描画設定
    ofSetVerticalSync(false);
    ofSetFrameRate(24);
    ofBackground(0, 120, 200, 200);
    // ofEnableDepthTest();  // GUIが表示されない
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // Box2Dの世界の設定
    box2d.init();               // 初期化
    box2d.setGravity(0, 10);    // 重力
    // box2d.createBounds();       // 画面の周囲に壁を作成
    box2d.setFPS(30.0);         // box2Dの世界でのFPS
    // box2d.registerGrabbing();   // 物体をつかめるようにする
    
    // ofBox2dPolygonクラスの要素を持つ配列を作成
    humanContour_in_box2d = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
    //    // 上記で定義したポリゴンを追加
    //    humanContour_in_box2d.get()->addVertexes(humanContour);
    //    // 三角形で頂点の間を埋める（これをしないと凹面がつくれない）
    //    humanContour_in_box2d.get()->triangulatePoly(10);
    //    // 摩擦などをセット
    //    humanContour_in_box2d.get()->setPhysics(0.0, 0.5, 0.1);
    // box2dの世界にポリゴンを登録
    humanContour_in_box2d.get()->create(box2d.getWorld());
    
}

// GUIを設定
void SceneB::initGui(){
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

// オブジェクトの作成
void SceneB::createObject(){
    float flg = 0;
    flg = ofRandom(2);
    if(flg > 1.5) {
        float r = ofRandom(4, 20);
        circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(3.0, 0.53, 0.1);
        circles.back().get()->setup(box2d.getWorld(), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()/4), r);
        
    }else if(flg > 1){
        float w = ofRandom(4, 20);
        float h = ofRandom(4, 20);
        boxes.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
        boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
        boxes.back().get()->setup(box2d.getWorld(), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()/4), w, h);
    }
}

// 画面から消えたオブジェクトの削除
void SceneB::killObject() {
    
    // circles
    for (vector<ofPtr<ofxBox2dCircle> >::iterator it = circles.begin(); it != circles.end();)
    {
        if(it->get()->getPosition().y > 500){
            it = circles.erase(it);
        }else{
            ++it;
        }
    }
    
    // boxes
    for (vector<ofPtr<ofxBox2dRect> >::iterator it = boxes.begin(); it != boxes.end();)
    {
        if(it->get()->getPosition().y > 500){
            it = boxes.erase(it);
        }else{
            ++it;
        }
    }
}

void SceneB::update(){
    // CV設定
    contourFinder.setMinAreaRadius(100);
    contourFinder.setMaxAreaRadius(300);
    
    // Kinect状態更新
    kinect->update();
    
    // 人の輪郭を初期化
    humanContour.clear();
    humanContour_in_box2d.get()->clear();
    
    if (kinect->isFrameNew()) {
        // ofxCvで輪郭抽出
        cv::Mat mat = cv::Mat(kinect->height, kinect->width, CV_8UC1, kinect->getDepthPixels(), 0);
        contourFinder.setThreshold(155);
        contourFinder.findContours(mat);
        
        if(contourFinder.size() > 0){
            // イテレータを使ったアクセス
            vector<cv::Point> human = contourFinder.getContour(0);    // とりあえずひとつ目だけ処理
            for (vector<cv::Point>::iterator it = human.begin(); it != human.end(); ++it)
            {
                // 人の輪郭の座標を追加
                humanContour.addVertex(it->x, it->y);
            }
            humanContour.close();
            humanContour.simplify();
            
            // 初期化
            humanContour_in_box2d.get()->clear();
            // 上記で定義したポリゴンを追加
            humanContour_in_box2d.get()->addVertexes(humanContour);
            // 三角形で頂点の間を埋める（これをしないと凹面がつくれない）
            humanContour_in_box2d.get()->triangulatePoly(10);
            // 摩擦などをセット
            humanContour_in_box2d.get()->setPhysics(0.0, 0.5, 0.1);
            // box2dの世界にポリゴンを登録
            humanContour_in_box2d.get()->create(box2d.getWorld());
        }
    }
    
    // 風
    box2d.setGravity(0, 10);    // 重力
    if(ofRandom(1)>0.6){
        float windFlg = 0;
        windFlg = 30 * sin(ofGetElapsedTimef()/20);
        box2d.setGravity(windFlg, 10);    // 重力
    }
    
    // オブジェクトの作成
    createObject();
    
    // 画面から消えたオブジェクトの削除
    killObject();
    
    // box2Dの更新
    box2d.update();
}

void SceneB::draw(){
    
    //ポリゴンを描画
    ofPushMatrix();
        // 描画位置の調整
        ofScale(ofGetWidth() / float(kinect->width), ofGetHeight() / float(kinect->height));
    
        // 人の輪郭を描画
        // humanContour.draw();
    
        // 面を三角形で描画
        vector <TriangleShape> tries = humanContour_in_box2d.get()->triangles;
        for (int i = 0; i < tries.size(); i++) {
            ofSetColor((i * 64) % 256 + 63);
            ofTriangle(tries[i].a, tries[i].b, tries[i].c);
        }
    
        // 円を描画
        for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0xf6c738);
            circles[i].get()->draw();
        }
        // 四角を描画
        for(int i=0; i<boxes.size(); i++) {
            ofFill();
            ofSetHexColor(0xBF2545);
            boxes[i].get()->draw();
        }
    ofPopMatrix();

    
    // GUIを表示
    gui.draw();
}