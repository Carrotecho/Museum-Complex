//
//  Mesh.h
//  ShadowMapsTestProject
//
//  Created by easy on 25.11.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#ifndef MESH_H__
#define MESH_H__
#define MAX_ATTRIBUTES 40

#define FILE_LOADED_LOCAL 1
#define FILE_GENERATED    2

#include "stdio.h"
#include "stdlib.h"
#include "Types.h"
#include "mathLib.h"
#include "XMathTypes.h"
#include "SkinningMath.h"

enum VERTEX_DECLARATION {
    VERTEX_DECLARATION_SCINNED_MESH,
    VERTEX_DECLARATION_NORMAL,
    VERTEX_DECLARATION_TANGENTS,
    VERTEX_DECLARATION_BINORMAL,
    VERTEX_DECLARATION_TEXTURE_COORDINATS_1,
    VERTEX_DECLARATION_COLOR,
    VERTEX_DECLARATION_TEXTURE_COORDINATS_3,
    VERTEX_DECLARATION_TEXTURE_COORDINATS_4
};
enum ATTRIBUTES {
    ATTRIBUTE__OBJECT_POSITION     = 1,
    ATTRIBUTE_NORMAL               = 2,
    ATTRIBUTE_BINORMAL             = 3,
    ATTRIBUTE_TANGENT              = 4,
    ATTRIBUTE_TEXTURE_COORDINATS_1 = 5,
    ATTRIBUTE_COLOR                = 6,
    ATTRIBUTE_TEXTURE_COORDINATS_3 = 7,
    ATTRIBUTE_TEXTURE_COORDINATS_4 = 8,
    ATTRIBUTE_FAR_CORNERS          = 9,
    ATTRIBUTE_BONES_WEIGHT         = 10,
    ATTRIBUTE_BONES_IDS            = 11,
	ATTRIBUTE_COLOR4               = 12,
	ATTRIBUTE_PARTICLE_DATA        = 13
};

#include "XAnimTypes.h"
#include "XObject.h"

class Mesh:public XObject {
    
public:

	uint udid;

    
    void release(){
        if(countUsed<=0) return ;
        countUsed--;
        if(countUsed<=0) {
			clear();
            // XTODO: memory leak need fix threads 
            //delete this;
        };
    };
    

	
    static Mesh* create();
    
	std::string nameMesh;

	int assignedBaseAnimationsID;

	long offsetInFile;
    
    std::vector<BoneInfo> boneIerarhy;
    int numBones;
	float positionData[9];
   
    uint typeMesh;
    
    uint indxInCache;
    
    //XTODO: .... 
    std::string urlForFile;
    bool loadingViaHttp;
    bool asyncLoaded;
    char lightFollowToCamera;
	float lightEyEVector[3];

    
    
    std::string pathForFile;
    
    uint indexMeshInFile;
    
    XVector3 boundingBoxMinPT;
    XVector3 boundingBoxMaxPT;
    
    XVector3 bboxMin;
	XVector3 bboxMax;
    XVector3 bboxCenter;
    
    XVector3 bboxSize;
    
    uint typeVBO;
   
    
    Mesh();
    virtual ~Mesh();
    virtual void calculateOffsets(uint offset,uint index);
 
    virtual XVector3* getBindTranslate(){return NULL;};
    virtual XVector4* getBindPoseRotation(){return NULL;};
    
    
    uint vertexDeclaration;
    
    bool color4;
    bool color;
    bool skinned;
    bool normal;
    bool tangent;
	bool particleData;
    bool binormal;
    bool morphed;
    uint numTextureCoordinats;
    
    
    
    
	
    
    
    
    
    uint sizeFloats;
    uint sizeVertex;
    
    uint numAllVertex;
    uint numAllFaces;
    
	XVector4 initQuaternion;
	XVector3 initRotation;
	XVector3 initScale;
    XVector3 initTranslation;
    
    // new data
    
    //XTODO: delete chunks !
	uint numChunks;





	MorphInfo morphInfo[MAX_CHUNKS];
    uint numVertexC[MAX_CHUNKS];
    uint numFacesC[MAX_CHUNKS];
    uint numSizeVertexC[MAX_CHUNKS];
    uint vertexBufferID[MAX_CHUNKS];
    uint indexBufferID[MAX_CHUNKS];
	bool indexChunksVBODamaged[MAX_CHUNKS];
	bool indexChunksIBODamaged[MAX_CHUNKS];
	uint indexOffset[MAX_CHUNKS];
	long offsets[MAX_CHUNKS][MAX_ATTRIBUTES];
    
	void *vertex[MAX_CHUNKS];
	void *faces[MAX_CHUNKS];



	float *morphVertOriginal;
	SkinningFrame*bindFrame;

    uint getSizeVertex();
    void setAsDamagedVBOChunk(uint chunk);
    void setAsDamagedIBOChunk(uint chunk);
    void setAsDamaged();

	bool isDamagedAll();
    
	
    
    uint sizeSeparate;
    
    uint typeIndexes;
    uint numIndexPerData;
    uint typeVertexData;
    
    bool isDamaged;
	bool isUsed;
    
    uint idModel;
    

	void clear();
	void deinitMorph();
	void deinitSkin();
	void initSkin();
private:
    
    int numVBOChunksDamaged;
};

#endif