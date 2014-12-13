//
//  SceneA.h
//  MultiScene
//
//  Created by sasascene on 2014/12/13.
//
//

#ifndef __MultiScene__SceneA__
#define __MultiScene__SceneA__

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

#endif /* defined(__MultiScene__SceneA__) */
