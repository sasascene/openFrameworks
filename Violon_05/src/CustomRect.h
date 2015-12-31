//
//  CustomRect.h
//  Violon_03
//
//  Created by sasascene on 2015/08/26.
//
//

#ifndef __Violon_03__CustomRect__
#define __Violon_03__CustomRect__

#include <stdio.h>
#include "ofMain.h"
#include "ofxBox2d.h"

class CustomRect : public ofxBox2dRect
{
private:
    ofImage img;
    
public:
    CustomRect();
    void draw();
    float customRectWidth, customRectHeight;
    void setImage(ofImage img);
    ofImage getImage();
};

#endif /* defined(__Violon_03__CustomRect__) */
