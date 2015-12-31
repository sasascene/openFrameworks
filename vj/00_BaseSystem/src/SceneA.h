//
//  SceneA.h
//  vj
//
//  Created by sasascene on 2014/12/13.
//
//

#ifndef __vj__SceneA__
#define __vj__SceneA__

#pragma once
#include "ofMain.h"
#include "BaseScene.h"

// BaseSceneを継承したSceneA

class SceneA : public BaseScene {
    
public:
    void setup();
    void update();
    void draw();

};

#endif /* defined(__vj__SceneA__) */
