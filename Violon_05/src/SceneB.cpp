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
    // ofBackground(0, 200, 200, 200);
    // ofEnableDepthTest();  // GUIが表示されない
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    // 画像の読み込み
    myImage.loadImage("pic/drop.png");
    
    ofImage theImage;
    theImage.loadImage("pic/grass.png");
    images.push_back(theImage);
    theImage.loadImage("pic/mouth01.png");
    images.push_back(theImage);
    theImage.loadImage("pic/triangle01.png");
    images.push_back(theImage);
    theImage.loadImage("pic/hige.png");
    images.push_back(theImage);
    theImage.loadImage("pic/hige02.png");
    images.push_back(theImage);
    theImage.loadImage("pic/glass.png");
    images.push_back(theImage);
    theImage.loadImage("pic/star.png");
    images.push_back(theImage);
    theImage.loadImage("pic/leaf01.png");
    images.push_back(theImage);
    theImage.loadImage("pic/leaf02.png");
    images.push_back(theImage);
    theImage.loadImage("pic/leaf03.png");
    images.push_back(theImage);
    
    // Box2Dの世界の設定
    box2d.init();               // 初期化
    box2d.setGravity(0, 10);    // 重力
    // box2d.createBounds();       // 画面の周囲に壁を作成
    box2d.setFPS(24.0);         // box2Dの世界でのFPS
    // box2d.registerGrabbing();   // 物体をつかめるようにする
    
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
    gui.add(min.setup("min", 30, 0, 400));
    gui.add(max.setup("max", 148, 0, 400));
    gui.add(threshold.setup("threshold", 240, 0, 400));
    gui.add(offset.setup("offset", 20, 1, 30));
    gui.add(contour.setup("contour", true));
}

// オブジェクトの作成
void SceneB::createObject(){
    
    // TODO:この処理は改善すること
    
    rain = sin(ofGetElapsedTimef()/15);
    if(rain > 0) {
        
        int index = (int)ofRandom(images.size());
        ofImage theImage = images[index];
        float scale = 1 + ofRandom(2);
        
        int w = theImage.width/(10*scale);
        int h = theImage.height/(10*scale);
        drops.push_back(ofPtr<CustomRect>(new CustomRect));
        drops.back().get()->setPhysics(ofRandom(3)+0.5, 0.3, 0.1);
        drops.back().get()->setImage(theImage);
        drops.back().get()->setup(box2d.getWorld(), ofRandom(ofGetWidth()), -ofRandom(ofGetHeight()/4), w, h);
        drops.back().get()->customRectWidth = w;
        drops.back().get()->customRectHeight = h;

    }else{
        int w = myImage.width/30;
        int h = myImage.height/30;
        drops.push_back(ofPtr<CustomRect>(new CustomRect));
        drops.back().get()->setPhysics(5, 0, 0.1);
        drops.back().get()->setImage(myImage);
        drops.back().get()->setup(box2d.getWorld(), ofRandom(ofGetWidth()), -ofRandom(ofGetHeight()/4), w, h);
        drops.back().get()->customRectWidth = w;
        drops.back().get()->customRectHeight = h;
    }

}

// 画面から消えたオブジェクトの削除
void SceneB::killObject() {
    
    // TODO:この処理は改善すること
    
    // circles
    for (vector<ofPtr<ofxBox2dCircle> >::iterator it = circles.begin(); it != circles.end();)
    {
        if(it->get()->getPosition().y > ofGetHeight()){
            it = circles.erase(it);
        }else{
            ++it;
        }
    }
    
    // boxes
    for (vector<ofPtr<ofxBox2dRect> >::iterator it = boxes.begin(); it != boxes.end();)
    {
        if(it->get()->getPosition().y > ofGetHeight()){
            it = boxes.erase(it);
        }else{
            ++it;
        }
    }
    
    // drops
    for (vector<ofPtr<CustomRect> >::iterator it = drops.begin(); it != drops.end();)
    {
        if(it->get()->getPosition().y > ofGetHeight()){
            it = drops.erase(it);
        }else{
            ++it;
        }
    }
}

// 人を検出する処理
void SceneB::detectHuman(){
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
        humanContour_collection_in_box2d.clear();
        
        // 人を発見した場合
        if(contourFinder.size() > 0){
            
            // 全ての人をスキャンし、人ごとに次の処理を行う
            for(int i = 0; i < contourFinder.size(); i++){
                
                // 人の形をしたポリゴン
                ofPtr<ofxBox2dPolygon> pBox2dPolygon = ofPtr<ofxBox2dPolygon>(new ofxBox2dPolygon);
                
                // ひとり分の輪郭座標を保持する配列
                vector<ofVec2f> humanContour;
                // ひとり分の輪郭座標から生成するポリゴン
                ofPolyline humanContourPolyLine;
                
                // 人単位で輪郭の座標を取得(イテレータを使ったアクセス)
                vector<cv::Point> human = contourFinder.getContour(i);
                for (vector<cv::Point>::iterator it = human.begin(); it != human.end(); ++it)
                {
                    // ひとつひとつの輪郭座標を配列に追加
                    humanContour.push_back(ofVec2f(it->x, it->y));
                    
                    ofVec2f pos1 = ofVec2f(it->x, it->y);
                    ofVec2f pos2 = ofVec2f((it-1)->x, (it-1)->y);
                    float distance = ofDist(pos1.x, pos1.y, pos2.x, pos2.y);
                    if(distance < 2){
                        // 人の輪郭の座標を追加
                        humanContourPolyLine.addVertex(it->x, it->y);
                    }
                }
                // ポリゴンを閉じる
                humanContourPolyLine.close();
                humanContourPolyLine.simplify();
                
                // ひとり分の輪郭座標を保持した配列をコレクションに追加
                humanContour_collection.push_back(humanContour);
                
                // 上記でひとり分の輪郭座標から定義したポリゴンを追加
                pBox2dPolygon->addVertexes(humanContourPolyLine);
                // 三角形で頂点の間を埋める（これをしないと凹面がつくれない）
                pBox2dPolygon->triangulatePoly(50);
                // 摩擦などをセット
                pBox2dPolygon->setPhysics(0.0, 0.5, 0.1);
                // box2dの世界にポリゴンを登録
                pBox2dPolygon->create(box2d.getWorld());
                
                // ポリゴンを配列に追加
                humanContour_collection_in_box2d.push_back(pBox2dPolygon);
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

// 風の表現
void SceneB::makeWind(){
    box2d.setGravity(0, 10);    // 重力
    if(rain > 0) {
        if(ofRandom(1)>0.7){
            float windFlg = 0;
            windFlg = (30 + (30 * ofRandom(1))) * sin(ofGetElapsedTimef()/3);
            box2d.setGravity(windFlg, 10);    // 重力
        }
    }
}

void SceneB::update(){
    
    // 人を検出
    detectHuman();
    
    // 風の表現
    makeWind();
    
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
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    // 描画位置の調整
    ofScale(ofGetWidth() / float(kinect->width), ofGetHeight() / float(kinect->height));
    ofTranslate(float(kinect->width), 0);
    ofRotateY(180);
    
    //　オブジェクトの描画
    for(int i=0; i<drops.size(); i++){
        drops[i].get()->draw();
    }
    
    // 人の描画
    for(int i = 0; i < humanContour_collection.size(); i++){
        ofVbo *vbo = &vbo_collection[i];
        vbo->draw(GL_TRIANGLE_STRIP, 0, humanContour_collection[i].size()/offset);
    }
    
    ofDisableBlendMode();
    ofPopMatrix();

    
    // GUIを表示
    if(valid_gui){
        gui.draw();
    }
}