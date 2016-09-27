//
//  MeshCache.h
//  ShadowMapsTestProject
//
//  Created by easy on 25.11.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef MESHCACHE_H__
#define MESHCACHE_H__

#include "XSingleton.h"

#include "Mesh.h"
#include "Types.h"
#include "stdlib.h"
#include <map>
#include <string>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <GL/glew.h>
#else

#ifdef __ANDROID__
#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2ext.h>
#else
#include <OpenGLES/ES2/gl.h>
#endif
#endif

#include <vector>


namespace Xengine
{

    typedef struct{
        uint startOffset;
        uint size;
        
        uint indexChunk;
        Mesh * mesh;
    }StoredData;
    
    class MeshCache;
    
    class  MeshCache 
    {
    public:
         
        MeshCache(std::map<uint, Mesh*> *damagedMeshs);
        ~MeshCache();
        
        void clear();
        void initialize();
        uint indexModels;
        uint allocedMeshs;
        
        std::vector<Mesh*> meshs;
        
        void createVBOIndex();
        void createVBOVertex(uint type = GL_STATIC_DRAW,uint size = maxSizeVBO,void*data = 0,bool restoreBuffer = false);
        void removeVBO(uint index);
		StoredData * findStoredData(std::vector<StoredData*>* vectorFind, uint meshIndex, uint chunkIndex, int * indexFind);
        void loadIChunk(Mesh*model,uint chunkI);
		void fillGPU(Mesh * model);
		void removeChunk(Mesh * damagedMesh, uint chunkIndex, std::vector<StoredData*>::iterator * itV, std::vector<StoredData*>::iterator * itI);
		void loadVChunk(Mesh*model,uint chunkI);
    private:
        uint currentIDVBOV;
        uint currentIDVBOI;
        
        uint createdNumVBO;
        uint createdNumIBO;
        
		std::map<uint,Mesh*> *_damagedMeshs;
        
        std::vector<StoredData*> storedDatasV[MAX_VBOS];
        std::vector<StoredData*> storedDatasI[MAX_VBOS];
        
        int findIndexBufferbyID(GLuint*inBuffer,int index);
        GLuint modelVertexBuffers[MAX_VBOS];
        GLuint modelIndexBuffers[MAX_VBOS];
        uint offsetModelVertexBuffer[MAX_VBOS];
        uint offsetModelIndexBuffer[MAX_VBOS];
        
        uint sizeModelBuffer[MAX_VBOS];
        int typeModelBuffer[MAX_VBOS];
    };
}

#endif