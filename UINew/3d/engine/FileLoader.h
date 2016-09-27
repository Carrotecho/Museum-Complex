//
//  FileLoader.h
//  
//
//  Created by easy proger on 05.07.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef ____FileLoader__
#define ____FileLoader__

#include <iostream>
#include "MeshCache.h"
#include "XAnimation.h"
#include "XAnimations.h"


#include "Object.hpp"
#include "SceneNode.h"

using namespace Xengine;



typedef struct {
	XVector4 quaternion;
	XVector3 scale;
	XVector3 translation;
}WritedData;



typedef struct{
    Mesh*mesh;
    SkinningAnimation*anim;
}MeshAnimationData;


class FileLoader {
public:
    
    uint genericID;
    
    FileLoader(MeshCache*meshCache,XAnimations*animationsCache);
    ~FileLoader(){
        clear();
    }
    
    
	int getNumMeshs(std::vector<SceneNode*>* childsList);

	long getEndFile(FILE * file, uint numMeshs);

	void safeParticle(ParticleInfo*emiter);
	void safeXMESH(const char*path, std::vector<SceneNode*> *childsList, std::vector<CameraInfo>*cameras, std::vector<LightInfo>*lights);
	void loadXMESH(const char*path, std::vector<Mesh*>*meshs, std::vector<GenericObjects*>*genericNodes, std::vector<CameraInfo>*cameras, std::vector<LightInfo>*lights, bool loadSceneData);
	XAnimation* readXMESHAnimation(FILE*file);
	std::string loadStaticMesh(FILE*file,Mesh*model,float version,bool reload = false,bool* exportAnimations = nullptr);
	void reloadChunkInMesh(Mesh*model);

    
    
    void clear();
    void loadXMESHData(const char*path,std::map<std::string,MeshAnimationData*> * meshsMap);
    void readXMESHAnimation2(FILE*file,SkinningAnimation*animation);
    
private:
	MeshCache*_meshCache; // for fill GPU
	XAnimations* _animationsCache; // for fill animations // bad idea 
	std::map<std::string,std::vector<Mesh*>> _meshsCacheMap; // for cache loaded meshs // very simple need better idea

	void loadOffsets(FILE*file, int numMeshes, std::vector<long>*offsets, std::vector<long>*sizes);

};





#endif /* defined(____FileLoader__) */
