//
//  CustomRect.cpp
//  Violon_03
//
//  Created by sasascene on 2015/08/26.
//
//

#include "CustomRect.h"

CustomRect::CustomRect() {
}

void CustomRect::draw() {
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();
    ofTranslate(getPosition().x, getPosition().y);
    ofRotate(getRotation());
    ofSetColor(255, 255, 255);
    img.draw(0, 0, customRectWidth, customRectHeight);
    ofPopMatrix();
}

void CustomRect::setImage(ofImage _img){
    img = _img;
}

ofImage CustomRect::getImage(){
    return img;
}