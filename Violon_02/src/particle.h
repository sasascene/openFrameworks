#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class particle
{
public:
	ofVec2f pos;
	ofVec2f vel;
	ofVec2f frc;
	
	particle();
	virtual ~particle(){};
	
	void resetForce();
	void addForce(float x, float y);	
	void addDampingForce();
	
	void setInitialCondition(float px, float py, float vx, float vy);
	void update();
	void draw();
	
	void bounceOffWalls();
    
    // 力を更新
    void updateForce();
	
	bool  bFixed;
	
	float damping;
    
    // 摩擦係数
    float friction;
	
protected:
private:
};

#endif // PARTICLE_H
