
//
//  SceneB.h
//  MultiScene
//
//  Created by sasascene on 2014/12/13.
//
//

#ifndef __MultiScene__SceneB__
#define __MultiScene__SceneB__

#pragma once
#include "ofMain.h"
#include "BaseScene.h"

// BaseSceneを継承したSceneB

class SceneB : public BaseScene {
    
public:
    void setup();
    void update();
    void draw();
};

#endif /* defined(__MultiScene__SceneB__) */
