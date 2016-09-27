//
//  ParticleEmiter.cpp
//  UltraVision
//
//  Created by Easy Proger on 02.07.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#include "ParticleEmiter.hpp"
#include "ResourceManager.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <time.h>
#else
#include <sys/time.h>
#endif

#include "Renderer.h"


ParticleEmiter::ParticleEmiter(std::string _resourcePath):SceneNode() {

    resourcePath                   = _resourcePath;
	info.blendIn                   = GL_ONE;
	info.blendOut                  = GL_ONE_MINUS_SRC_ALPHA;
	info.blend                     = true;
	info.typeSort                  = PrticleSortTypes::NO_SORT_PARTICLE;

	info.pitch                     = 90.0;
	info.azimuth                   = 0.0;
	info.position                  = XVector4(0.0, 0.0, 0.0, 0.0); //position
	info.azimuthVariance           = 45.0; //azimuthVariance
	info.pitchVariance             = 45.0; //pitchVariance
	info.speed                     = 1.0; //speed
	info.speedVariance             = 0.2; //speedVariance
	info.particlesEmittedPerSecond = 50.0; //particlesPerSecond
	info.particleEmitVariance      = 5.0; //particlesPerSecondVariance
	info.particleLifespan          = 23.0; //particleLifespan
	info.particleLifespanVariance  = 3.0; //particleLifespanVariance
	info.startColor                = XVector4(0.9, 0.4, 0.0, 1.0); //startColor
	info.startColorVariance        = XVector4(0.2, 0.2, 0.0, 0.0); //startColorVariance
	info.finishColor               = XVector4(0.0, 0.0, 0.0, 0.0); //finishColor
	info.finishColorVariance       = XVector4(0.0, 0.0, 0.0, 0.0); //finishColorVariance
	info.force                     = XVector3(0.0, 0.0, 0.0); //force
	info.forceVariance             = XVector3(0.0, 0.0, 0.0); //forceVariance
	info.particleSize              = 150.0; //particleSize
	info.particleSizeVariance      = 15.0; //particleSizevarians
	info.colorInterpolate          = true;
	info.nodePosition              = XVector3(0, 0, 0);
	info.nodeScale                 = XVector3(1,1,1);
	create();
}

ParticleEmiter::ParticleEmiter(ParticleInfo info,std::string _resourcePath) :SceneNode() {
	this->info = info;

    resourcePath           = _resourcePath;
	this->blendFunction    = info.blend;
	this->blendFunctionIn  = info.blendIn;
	this->blendFunctionOut = info.blendOut;

	this->setTranslate(&info.nodePosition);
	this->setScaleX(info.nodeScale.x);
	this->setScaleY(info.nodeScale.y);
	this->setScaleZ(info.nodeScale.z);

	create();
}


ParticleEmiter::~ParticleEmiter() {
    // XTODO: need remove particles 


}


ParticleInfo& ParticleEmiter::getInfo() {

	info.nodePosition = this->getPosition();
	info.nodeScale = this->getScale();

	return info;
}
void ParticleEmiter::setInfo(ParticleInfo*val) {

	bool needUpdateBuffer = isNeedUpdateBuffer(val);

	info = val;
	
	this->blendFunction = info.blend;
	this->blendFunctionIn = info.blendIn;
	this->blendFunctionOut = info.blendOut;

	this->setTranslate(&val->nodePosition);
	this->setScaleX(val->nodeScale.x);
	this->setScaleY(val->nodeScale.y);
	this->setScaleZ(val->nodeScale.z);

	if (needUpdateBuffer)
	updatebuffer();
}

bool ParticleEmiter::isNeedUpdateBuffer(ParticleInfo*val) {
	if (info.particlesEmittedPerSecond != val->particlesEmittedPerSecond ||
		info.particleEmitVariance      != val->particleEmitVariance ||
		info.particleLifespan          != val->particleLifespan ||
		info.particleLifespanVariance  != val->particleLifespanVariance) {
		return true;
	}
	return false;
}

void ParticleEmiter::create() {

    GLSyncExtEnabled = true;
#ifdef __ANDROID__
    
    glMapBufferRangeEXT         = NULL;
    glFlushMappedBufferRangeEXT = NULL;
    glClientWaitSyncAPPLE       = NULL;
    glFenceSyncAPPLE            = NULL;
    
    initialiseFunctionsGL20ExtFunctions();
    
    if (glMapBufferRangeEXT         != NULL &&
        glFlushMappedBufferRangeEXT != NULL &&
        glClientWaitSyncAPPLE       != NULL &&
        glFenceSyncAPPLE            != NULL) {
    }else {
        GLSyncExtEnabled = false;
    }
#endif

	this->typeObject = PARTICLES_OBJECT;
	this->timeNow = 0;

	this->blendFunction    = info.blend;
	this->blendFunctionIn  = info.blendIn;
	this->blendFunctionOut = info.blendOut;

	this->zTest = true;
	this->waitEndUpdate = false;
	this->particlePool = NULL;
	this->particle = NULL;
	this->lastDrawTime = 0.0;
	this->currentParticleCount = 1;

    Mesh*mesh = new Mesh();
	mesh->nameMesh = "Particles";
    this->setMesh(mesh);
    mesh->release();
    
	
    meshParticles = model;
    meshParticles->numTextureCoordinats = 1;

    meshParticles->tangent      = false;
    meshParticles->binormal     = false;
    meshParticles->normal       = false;
    meshParticles->color        = false;
    meshParticles->color4       = true;
    meshParticles->skinned      = false;
	meshParticles->particleData = true;

    meshParticles->numChunks = 1;
	meshParticles->isDamaged = false;
	meshParticles->typeIndexes = GL_UNSIGNED_SHORT;
	meshParticles->numIndexPerData = 6;
	meshParticles->typeVertexData = GL_TRIANGLES;
	meshParticles->indexOffset[0] = 0;

	sizeVertex = meshParticles->getSizeVertex();

	modelIndexBuffer = 0;
	modelVertexBuffer = 0;

	glGenBuffers(1, &modelIndexBuffer);
	glGenBuffers(1, &modelVertexBuffer);

	meshParticles->indexBufferID[0] = modelIndexBuffer;
	meshParticles->vertexBufferID[0] = modelVertexBuffer;
	meshParticles->calculateOffsets(0, 0);


	Material*particleMaterial = new Material();
	Texture*diffuseTexture = new Texture();

	std::string path = resourcePath;
	path += "/particleImages/smoke_1.png";

	diffuseTexture->createTexture(path.c_str());
	particleMaterial->diffuseTexture = diffuseTexture;
	particleMaterial->setFlag(PARTICLE_MATERIAL,true);


	particleMaterial->color.x = 1.0;
	particleMaterial->color.y = 0.0;
	particleMaterial->color.z = 1.0;

	this->setMaterial(particleMaterial);

	updatebuffer();
}


void ParticleEmiter::updatebuffer() {
	int maxParticle = theoreticalMaxParticles();
	meshParticles->numVertexC[0] = maxParticle * 4;
	meshParticles->numFacesC[0] = maxParticle * 6;
	uint sizeChunkFaces = meshParticles->numFacesC[0] * sizeof(unsigned short);
	uint sizeChunkVertex = (meshParticles->numVertexC[0] * 4)*sizeVertex;
	meshParticles->faces[0] = malloc(sizeChunkFaces);
	for (unsigned short i = 0; i < maxParticle; i++)
	{
		((unsigned short*)meshParticles->faces[0])[(i * 6)] = (i * 4);
		((unsigned short*)meshParticles->faces[0])[(i * 6) + 1] = (i * 4) + 1;
		((unsigned short*)meshParticles->faces[0])[(i * 6) + 2] = (i * 4) + 2;

		((unsigned short*)meshParticles->faces[0])[(i * 6) + 3] = (i * 4);
		((unsigned short*)meshParticles->faces[0])[(i * 6) + 4] = (i * 4) + 2;
		((unsigned short*)meshParticles->faces[0])[(i * 6) + 5] = (i * 4) + 3;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeChunkFaces, meshParticles->faces[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	free(meshParticles->faces[0]);

	glBindBuffer(GL_ARRAY_BUFFER, modelVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeChunkVertex, 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);




	Particle*tree = this->particle;
	while (tree) {
		Particle*toRemove = tree;
		toRemove->removed = true;
		tree = tree->next;
	}

	tree = particlePool;
	while (tree) {
		Particle*toRemove = tree;
		tree = tree->next;
		if (!toRemove->removed) {
			free(toRemove);
		}
	}

	tree = this->particle;
	while (tree) {
		Particle*toRemove = tree;
		tree = tree->next;
		free(toRemove);
	}

	this->particle = NULL;
	particlePool = NULL;

	this->timeNow = 0.0;
	this->lastDrawTime = 0.0;
	this->currentParticleCount = 1;

	if (this->particle == NULL)
	{
		XVector4 direction;
		double howLongToLive = info.particleLifespan + (particleRandom() * info.particleLifespanVariance);
		this->particle = particle3DMake(direction, timeNow + howLongToLive, info.finishColor, info.startColor, info.particleSize, timeNow);
	}


	this->model->isDamaged = false;
}


void ParticleEmiter::particleMoveParticleToPool(Particle *particle, ParticleEmiter *emitter)
{
    Particle *particleNext = particle->next;
    Particle *particlePrev = particle->prev;
    
    // Repair gap in particle list caused by removal of this particle
    if (particleNext)
        particleNext->prev = particlePrev;
    if (particlePrev)
        particlePrev->next = particleNext;
    
    // Particle will become first object in pool, so no prev
    particle->prev = NULL;
    
    // Since it's first, the old first becomes its next
	if (particle == emitter->particlePool) {
		particle->next = NULL;
	}
	else {
		particle->next = emitter->particlePool;
	}

    
    if (emitter->particlePool)
        emitter->particlePool->prev = particle;
    
    // And point the pool at the new first member
    emitter->particlePool = particle;
    
}
void ParticleEmiter::particleGetParticleFromPool(ParticleEmiter *emitter)
{
    // Grab the first particle in the pool
    Particle *particle = emitter->particlePool;
    
    // Get pointers to particles affected by the move
    // Since we always grab the first one, don't worry about repairing prevous link
    Particle *poolNext  = particle->next;
    Particle *particleFirst = emitter->particle;
    
    if (poolNext)
        poolNext->prev = NULL;
    emitter->particlePool = poolNext;
    
    particle->prev = NULL;

	if (particleFirst == particle) {
		particle->next = NULL;
	}
	else {
		particle->next = particleFirst;
	}

    
    if (particleFirst)
        particleFirst->prev = particle;
    emitter->particle = particle;
}

Particle * ParticleEmiter::particle3DMake(XVector4 direction, double timeToDie, XVector4 colorAtEnd, XVector4 colorAtBeginning, float inParticleSize,double timeWasBorn)
{
    Particle *ret = (Particle *)malloc(sizeof(Particle));
	ret->removed = false;
	ret->Poolremoved = false;
    ret->position.x = 0.0;
	ret->position.y = 0.0;
	ret->position.z = 0.0;
	ret->lastPosition.x = 0.0;
	ret->lastPosition.y = 0.0;
	ret->lastPosition.z = 0.0;

    ret->direction = direction;
    ret->timeToDie = timeToDie;
    ret->colorAtEnd = colorAtEnd;
    ret->colorAtBeginning = colorAtBeginning;
    ret->color = colorAtBeginning;
    ret->particleSize = inParticleSize;
    ret->timeWasBorn = timeWasBorn;
    ret->prev = NULL;
    ret->next = NULL;
    
    return ret;
}





static inline void Color3DInterpolate(XVector4 *ret,XVector4 *color1, XVector4 *color2, GLfloat percent)
{
    ret->r = color1->r + ((color2->r - color1->r) * percent);
    ret->b = color1->b + ((color2->b - color1->b) * percent);
    ret->g = color1->g + ((color2->g - color1->g) * percent);
    ret->a = color1->a + ((color2->a - color1->a) * percent);
    
    if (ret->r > 1.0)
        ret->r -= 1.0;
    if (ret->g > 1.0)
        ret->g -= 1.0;
    if (ret->b > 1.0)
        ret->b -= 1.0;
    if (ret->a > 1.0)
        ret->a = 1.0;
    if (ret->r < 0.0)
        ret->r += 1.0;
    if (ret->g < 0.0)
        ret->g += 1.0;
    if (ret->b < 0.0)
        ret->b += 1.0;
    if (ret->a < 0.0)
        ret->a += 1.0;
}

struct particlesortfuncFrontToBack
{
	inline bool operator() (const Particle* struct1, const Particle* struct2)
	{
		return (struct1->cameraDot < struct2->cameraDot);
	}
};

struct particlesortfuncBackToFront
{
	inline bool operator() (const Particle* struct1, const Particle* struct2)
	{
		return (struct1->cameraDot > struct2->cameraDot);
	}
};

void ParticleEmiter::update(uint frameID, double delta) {
    if (!GLSyncExtEnabled) {
        return;
    }

	if (this->model->isDamaged) return;
	timeNow += delta;

    if (lastDrawTime == 0.0) {
        lastDrawTime = timeNow;
        return;
    }

	uint numParticlesNow = 0;
	
	Particle *oneParticle = this->particle;
    
    waitEndUpdate = true;
    
    uint offset = 0;
    uint length = sizeVertex*(this->currentParticleCount*4); // length needle data
    
    glBindBuffer(GL_ARRAY_BUFFER, meshParticles->vertexBufferID[0]);
    ParticleVertexData *vertices = (ParticleVertexData*)glMapBufferRangeEXT(GL_ARRAY_BUFFER, offset, length,
                                                                            GL_MAP_WRITE_BIT_EXT | GL_MAP_FLUSH_EXPLICIT_BIT_EXT |
                                                                            GL_MAP_UNSYNCHRONIZED_BIT_EXT );
    
    
    
    
    // Wait for fence (set below) before modifying buffer.
    glClientWaitSyncAPPLE(fence, GL_SYNC_FLUSH_COMMANDS_BIT_APPLE,GL_TIMEOUT_IGNORED_APPLE);
    
    
    if (vertices == NULL) {
        // Wait for fence (set below) before modifying buffer.
        glUnmapBufferOES(GL_ARRAY_BUFFER);
        return;
    }

    
    
    double now = timeNow;
    float timeElapsed = (float)(timeNow-lastDrawTime);
    
    uint newParticleCount = uint((info.particlesEmittedPerSecond + ( particleRandom() * info.particleEmitVariance )) * timeElapsed);
    
    // create new
    
    for (uint i = 0; i < newParticleCount; i++)
    {
        double howLongToLive = info.particleLifespan + (particleRandom() * info.particleLifespanVariance);
        GLfloat azimuth = (float)(info.azimuth + (info.azimuthVariance * particleRandom()));
        GLfloat pitch   = float(info.pitch+ (info.pitchVariance * particleRandom()));
        XVector4 direction;
        
        
        direction.x = -sinf((float)DEGREES_TO_RADIANS(azimuth)) * cosf((float)DEGREES_TO_RADIANS(pitch));
        direction.y =  sinf((float)DEGREES_TO_RADIANS(pitch));
        direction.z =  cosf((float)DEGREES_TO_RADIANS(pitch))   * cosf((float)DEGREES_TO_RADIANS(azimuth));
         

        
        GLfloat speed = info.speed + (info.speedVariance * particleRandom());
        direction.x *= (speed);
        direction.y *= (speed);
        direction.z *= (speed);
        
        XVector4 start = info.startColor;
        start.r *= info.startColor.r + (info.startColorVariance.r * particleRandom());
        start.b *= info.startColor.b + (info.startColorVariance.b * particleRandom());
        start.g *= info.startColor.g + (info.startColorVariance.g * particleRandom());
        
        XVector4 finish = info.finishColor;
        finish.r *= info.finishColor.r + (info.finishColorVariance.r * particleRandom());
        finish.b *= info.finishColor.b + (info.finishColorVariance.b * particleRandom());
        finish.g *= info.finishColor.g + (info.finishColorVariance.g * particleRandom());
        
        GLfloat particleSize = info.particleSize + (info.particleSizeVariance * particleRandom());
 
        double timeToDie = now + howLongToLive;
        
        if (this->particlePool == NULL)
        {
            Particle *newParticle =  particle3DMake(direction, timeToDie, finish, start, particleSize,timeNow);
            Particle *currentFirstParticle = this->particle;
            currentFirstParticle->prev = newParticle;
            newParticle->next = currentFirstParticle;
            this->particle = newParticle;

            this->currentParticleCount++;
			this->lifetimeParticlesCount++;
        }
        else
        {
            particleGetParticleFromPool(this);
            this->currentParticleCount++;
			this->lifetimeParticlesCount++;


			


			this->particle->position.x = 0.0;
			this->particle->position.y = 0.0;
			this->particle->position.z = 0.0;
			this->particle->lastPosition.x = 0.0;
			this->particle->lastPosition.y = 0.0;
			this->particle->lastPosition.z = 0.0;
            this->particle->direction = direction;
            this->particle->timeToDie = timeToDie;
            this->particle->colorAtEnd = finish;
            this->particle->particleSize = particleSize;
            this->particle->colorAtBeginning = start;
            this->particle->color = start;
            this->particle->timeToDie = timeToDie;
            this->particle->timeWasBorn = timeNow;
        }
       
    }
    
    // update current
    
    
    numParticlesNow = 0;
	if (info.typeSort == PrticleSortTypes::NO_SORT_PARTICLE) {
		while (oneParticle) {
			// TODO: transfer to neon !;
			oneParticle->lastPosition = oneParticle->position;
			oneParticle->position.x += oneParticle->direction.x * timeElapsed;
			oneParticle->position.y += oneParticle->direction.y * timeElapsed;
			oneParticle->position.z += oneParticle->direction.z * timeElapsed;
			oneParticle->direction.x += (info.force.x + (info.forceVariance.x * particleRandom())) * timeElapsed;
			oneParticle->direction.y += (info.force.y + (info.forceVariance.y * particleRandom())) * timeElapsed;
			oneParticle->direction.z += (info.force.z + (info.forceVariance.z * particleRandom())) * timeElapsed;

			if (info.colorInterpolate) {
				GLfloat percentThroughLife = (now < oneParticle->timeWasBorn) ? 0.0f : (now - oneParticle->timeWasBorn) / (oneParticle->timeToDie - oneParticle->timeWasBorn);
				if (percentThroughLife > 1.0)
					percentThroughLife = 1.0;

				Color3DInterpolate(&oneParticle->color, &oneParticle->colorAtBeginning, &oneParticle->colorAtEnd, percentThroughLife);
			}

			XVector4 *partColor = &oneParticle->color;
			XVector4*point = &oneParticle->position;
			float polyScale = 0.01f * oneParticle->particleSize;

			if (now > oneParticle->timeToDie)
			{
				Particle *next = oneParticle->next;
				particleMoveParticleToPool(oneParticle, this);
				this->currentParticleCount--;
				if (this->currentParticleCount < 1) this->currentParticleCount = 1.0;
				oneParticle = next;
			}
			else {
				oneParticle = oneParticle->next;
			}


			ParticleVertexData *vertex1 = &vertices[(numParticlesNow * 4)];
			ParticleVertexData *vertex2 = &vertices[(numParticlesNow * 4) + 1];
			ParticleVertexData *vertex3 = &vertices[(numParticlesNow * 4) + 2];
			ParticleVertexData *vertex4 = &vertices[(numParticlesNow * 4) + 3];
			numParticlesNow++;


			vertex1->x = point->x;
			vertex1->y = point->y;
			vertex1->z = point->z;

			vertex1->squareVerticesx = -0.5f;
			vertex1->squareVerticesy = -0.5f;
			vertex1->sizeX = polyScale;
			vertex1->sizeY = polyScale;

			vertex2->x = point->x;
			vertex2->y = point->y;
			vertex2->z = point->z;

			vertex2->squareVerticesx = -0.5f;
			vertex2->squareVerticesy = 0.5f;
			vertex2->sizeX = polyScale;
			vertex2->sizeY = polyScale;

			vertex3->x = point->x;
			vertex3->y = point->y;
			vertex3->z = point->z;

			vertex3->squareVerticesx = 0.5f;
			vertex3->squareVerticesy = 0.5f;
			vertex3->sizeX = polyScale;
			vertex3->sizeY = polyScale;

			vertex4->x = point->x;
			vertex4->y = point->y;
			vertex4->z = point->z;

			vertex4->squareVerticesx = 0.5f;
			vertex4->squareVerticesy = -0.5f;
			vertex4->sizeX = polyScale;
			vertex4->sizeY = polyScale;

			vertex1->u = 0.0;
			vertex1->v = 0.0;

			vertex1->r = partColor->r;
			vertex1->g = partColor->g;
			vertex1->b = partColor->b;
			vertex1->a = partColor->a;

			vertex2->u = 0.0;
			vertex2->v = 1.0;

			vertex2->r = partColor->r;
			vertex2->g = partColor->g;
			vertex2->b = partColor->b;
			vertex2->a = partColor->a;

			vertex3->u = 1.0;
			vertex3->v = 1.0;

			vertex3->r = partColor->r;
			vertex3->g = partColor->g;
			vertex3->b = partColor->b;
			vertex3->a = partColor->a;

			vertex4->u = 1.0;
			vertex4->v = 0.0;

			vertex4->r = partColor->r;
			vertex4->g = partColor->g;
			vertex4->b = partColor->b;
			vertex4->a = partColor->a;

		}
	}
	else {
		std::vector<Particle*> particles;
		while (oneParticle) {
			// TODO: transfer to neon !
			particles.push_back(oneParticle);

			oneParticle->lastPosition = oneParticle->position;
			oneParticle->position.x += oneParticle->direction.x * timeElapsed;
			oneParticle->position.y += oneParticle->direction.y * timeElapsed;
			oneParticle->position.z += oneParticle->direction.z * timeElapsed;
			oneParticle->direction.x += (info.force.x + (info.forceVariance.x * particleRandom())) * timeElapsed;
			oneParticle->direction.y += (info.force.y + (info.forceVariance.y * particleRandom())) * timeElapsed;
			oneParticle->direction.z += (info.force.z + (info.forceVariance.z * particleRandom())) * timeElapsed;

			oneParticle->cameraDot = cameraDir.dot(oneParticle->position);

			if (info.colorInterpolate) {
				GLfloat percentThroughLife = (now < oneParticle->timeWasBorn) ? 0.0f : (now - oneParticle->timeWasBorn) / (oneParticle->timeToDie - oneParticle->timeWasBorn);
				if (percentThroughLife > 1.0)
					percentThroughLife = 1.0;

				Color3DInterpolate(&oneParticle->color, &oneParticle->colorAtBeginning, &oneParticle->colorAtEnd, percentThroughLife);
			}

			XVector4 *partColor = &oneParticle->color;
			XVector4*point = &oneParticle->position;
			float polyScale = 0.01f * oneParticle->particleSize;

			if (now > oneParticle->timeToDie)
			{
				Particle *next = oneParticle->next;
				particleMoveParticleToPool(oneParticle, this);
				this->currentParticleCount--;
				if (this->currentParticleCount < 1) this->currentParticleCount = 1.0;
				oneParticle = next;
			}
			else {
				oneParticle = oneParticle->next;
			}
		}

		if (info.typeSort == PrticleSortTypes::BACK_TO_FRONT_SORT_PARTICLE) {
			sort(particles.begin(), particles.end(), particlesortfuncBackToFront());
		}
		else if (info.typeSort == PrticleSortTypes::FRONT_TO_BACK_SORT_PARTICLE) {
			sort(particles.begin(), particles.end(), particlesortfuncFrontToBack());
		}

		numParticlesNow = 0;
		for (uint i = 0; i < particles.size(); i++) {

			Particle* particle = particles[i];
			XVector4* partColor = &particle->color;
			XVector4* point = &particle->position;
			float polyScale = 0.01f * particle->particleSize;

			ParticleVertexData *vertex1 = &vertices[(numParticlesNow * 4)];
			ParticleVertexData *vertex2 = &vertices[(numParticlesNow * 4) + 1];
			ParticleVertexData *vertex3 = &vertices[(numParticlesNow * 4) + 2];
			ParticleVertexData *vertex4 = &vertices[(numParticlesNow * 4) + 3];
			numParticlesNow++;


			vertex1->x = point->x;
			vertex1->y = point->y;
			vertex1->z = point->z;

			vertex1->squareVerticesx = -0.5f;
			vertex1->squareVerticesy = -0.5f;
			vertex1->sizeX = polyScale;
			vertex1->sizeY = polyScale;

			vertex2->x = point->x;
			vertex2->y = point->y;
			vertex2->z = point->z;

			vertex2->squareVerticesx = -0.5f;
			vertex2->squareVerticesy = 0.5f;
			vertex2->sizeX = polyScale;
			vertex2->sizeY = polyScale;

			vertex3->x = point->x;
			vertex3->y = point->y;
			vertex3->z = point->z;

			vertex3->squareVerticesx = 0.5f;
			vertex3->squareVerticesy = 0.5f;
			vertex3->sizeX = polyScale;
			vertex3->sizeY = polyScale;

			vertex4->x = point->x;
			vertex4->y = point->y;
			vertex4->z = point->z;

			vertex4->squareVerticesx = 0.5f;
			vertex4->squareVerticesy = -0.5f;
			vertex4->sizeX = polyScale;
			vertex4->sizeY = polyScale;

			vertex1->u = 0.0;
			vertex1->v = 0.0;

			vertex1->r = partColor->r;
			vertex1->g = partColor->g;
			vertex1->b = partColor->b;
			vertex1->a = partColor->a;

			vertex2->u = 0.0;
			vertex2->v = 1.0;

			vertex2->r = partColor->r;
			vertex2->g = partColor->g;
			vertex2->b = partColor->b;
			vertex2->a = partColor->a;

			vertex3->u = 1.0;
			vertex3->v = 1.0;

			vertex3->r = partColor->r;
			vertex3->g = partColor->g;
			vertex3->b = partColor->b;
			vertex3->a = partColor->a;

			vertex4->u = 1.0;
			vertex4->v = 0.0;

			vertex4->r = partColor->r;
			vertex4->g = partColor->g;
			vertex4->b = partColor->b;
			vertex4->a = partColor->a;
		}
	}


    
    meshParticles->numFacesC[0]  = numParticlesNow;
    
	
    
    
    // modify end
    
    
    // flush, and un map.
    glFlushMappedBufferRangeEXT(GL_ARRAY_BUFFER, offset, length);
    glUnmapBufferOES(GL_ARRAY_BUFFER);
    
    
    
    meshParticles->isDamaged = false;
    lastDrawTime = timeNow;
}

void ParticleEmiter::endUpdate() {
    if (!GLSyncExtEnabled) {
        return;
    }
    if (!waitEndUpdate) {
        return ;
    }
    
    for (uint i = 0; i < childs.size(); i++) {
        childs[i]->endUpdate();
    }

    fence = glFenceSyncAPPLE(GL_SYNC_GPU_COMMANDS_COMPLETE_APPLE, 0);
    
    waitEndUpdate = false;
}


int ParticleEmiter::theoreticalMaxParticles() {
    return (info.particlesEmittedPerSecond + (info.particleEmitVariance/2+1)) * (info.particleLifespan + (info.particleLifespanVariance/2+1));
}



void ParticleEmiter::setParticlesEmittedPerSecond(float inParticlesEmittedPerSecond) {
	this->model->isDamaged = true;
	info.particlesEmittedPerSecond = inParticlesEmittedPerSecond;
	updatebuffer();
}

void ParticleEmiter::setParticleEmitVariance(float inParticleEmitVariance) {
	this->model->isDamaged = true;
	info.particleEmitVariance = inParticleEmitVariance;
	updatebuffer();
}

void ParticleEmiter::setParticleLifespan(float inParticleLifespan) {
	this->model->isDamaged = true;
	info.particleLifespan = inParticleLifespan;
	updatebuffer();
}

void ParticleEmiter::setParticleLifespanVariance(float inParticleLifespanVariance) {
	this->model->isDamaged = true;
	info.particleLifespanVariance = inParticleLifespanVariance;
	updatebuffer();
}

