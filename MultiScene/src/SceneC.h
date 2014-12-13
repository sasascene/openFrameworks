//
//  SceneC.h
//  MultiScene
//
//  Created by sasascene on 2014/12/13.
//
//

#ifndef __MultiScene__SceneC__
#define __MultiScene__SceneC__

#pragma once
#include "ofMain.h"
#include "BaseScene.h"

// BaseSceneを継承したSceneC

class SceneC : public BaseScene {
    
public:
    void setup();
    void update();
    void draw();
};

#endif /* defined(__MultiScene__SceneC__) */
