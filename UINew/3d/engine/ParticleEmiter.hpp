//
//  ParticleEmiter.hpp
//  UltraVision
//
//  Created by Easy Proger on 02.07.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#ifndef ParticleEmiter_cpp
#define ParticleEmiter_cpp

#include <stdio.h>
#include "XVector4Math.h"
#include "XMathTypes.h"
#include "Mesh.h"
#include "SceneNode.h"


#include "GLES20ExtAndroid.h"

#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)
#define particleRandom() (float)(((float)(rand() % 100) / 50.0f) - 1.0f)

typedef struct Particle{
    XVector4		position;
    XVector4		lastPosition;
    XVector4		direction;
	bool removed;
	bool Poolremoved;
     double	timeToDie;
     double  timeWasBorn;
    
    XVector4			color;
    XVector4			colorAtEnd;
    XVector4			colorAtBeginning;
    
    GLfloat			particleSize;
    
    Particle 			*prev;
    Particle 			*next;


	

	float cameraDot;
} Particle;


typedef struct {
    // position point
    float x,y,z;
    // texture coordinate
    float u;
    float v;
    // color
    float r,g,b,a;
	// particleData
	float squareVerticesx, squareVerticesy, sizeX, sizeY;
} ParticleVertexData;


class ParticleEmiter :public SceneNode {
public:
	ParticleEmiter(std::string _resourcePath);

	ParticleEmiter(ParticleInfo info,std::string _resourcePath);
    ~ParticleEmiter();
    
    std::string resourcePath;

	virtual void update(uint frameID, double delta = 0.0);
	virtual void endUpdate();

    

	void setParticlesEmittedPerSecond(float inParticlesEmittedPerSecond);
	void setParticleEmitVariance(float inParticleEmitVariance);
	void setParticleLifespan(float inParticleLifespan);
	void setParticleLifespanVariance(float inParticleLifespanVariance);

	float getParticlesEmittedPerSecond() { return info.particlesEmittedPerSecond; };
	float getParticleEmitVariance() { return info.particleEmitVariance; };
	float getParticleLifespan() { return info.particleLifespan; };
	float getParticleLifespanVariance() { return info.particleLifespanVariance; };


	bool getColorInterpolate() const { return info.colorInterpolate; }
	void setColorInterpolate(bool val) { info.colorInterpolate = val; }
	float getAzimuthVariance() const { return info.azimuthVariance; }
	void setAzimuthVariance(float val) { info.azimuthVariance = val; }
	float getPitchVariance() const { return info.pitchVariance; }
	void setPitchVariance(float val) { info.pitchVariance = val; }
	float getSpeed() const { return info.speed; }
	void setSpeed(float val) { info.speed = val; }
	float getSpeedVariance() const { return info.speedVariance; }
	void setSpeedVariance(float val) { info.speedVariance = val; }
	uint getCurrentParticleCount() const { return currentParticleCount; }
	XVector4 getStartColor() const { return info.startColor; }
	void setStartColor(XVector4 val) { info.startColor = val; }
	XVector4 getStartColorVariance() const { return info.startColorVariance; }
	void setStartColorVariance(XVector4 val) { info.startColorVariance = val; }
	XVector4 getFinishColor() const { return info.finishColor; }
	void setFinishColor(XVector4 val) { info.finishColor = val; }
	XVector4 getFinishColorVariance() const { return info.finishColorVariance; }
	void setFinishColorVariance(XVector4 val) { info.finishColorVariance = val; }
	XVector3 getForce() const { return info.force; }
	void setForce(XVector3 val) { info.force = val; }
	XVector3 getForceVariance() const { return info.forceVariance; }
	void setForceVariance(XVector3 val) { info.forceVariance = val; }
	float getParticleSize() const { return info.particleSize; }
	void setParticleSize(float val) { info.particleSize = val; }
	float getParticleSizeVariance() const { return info.particleSizeVariance; }
	void setParticleSizeVariance(float val) { info.particleSizeVariance = val; }



    GLsync fence;

	ParticleInfo& getInfo();
	void setInfo(ParticleInfo*val);
	bool isNeedUpdateBuffer(ParticleInfo*val);
private:
    bool GLSyncExtEnabled;
	ParticleInfo info;
	void create();
	int theoreticalMaxParticles();
	void updatebuffer();



	double timeNow;

	bool waitEndUpdate;
	Mesh* meshParticles;
	

	long      identifier;

	Particle* particle;
	Particle* particlePool;

	XVector4  position;
	XVector4  rotation;


	bool	  isEmitting;



	uint lifetimeParticlesCount;
	uint currentParticleCount;
	double	lastDrawTime;
	uint sizeVertex;
	uint modelIndexBuffer;
	uint modelVertexBuffer;
	void particleGetParticleFromPool(ParticleEmiter *emitter);
	void particleMoveParticleToPool(Particle *particle, ParticleEmiter *emitter);
	Particle * particle3DMake(XVector4 direction, double timeToDie, XVector4 colorAtEnd, XVector4 colorAtBeginning, float inParticleSize, double timeWasBorn);

};


#endif /* ParticleEmiter_cpp */
