//
//  Mesh.cpp
//  ShadowMapsTestProject
//
//  Created by easy on 25.11.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "Mesh.h"
#include "FlagsForShaderCompile.h"
#include "XTypes.h"
#include "XUDID.h"

Mesh* Mesh::create() {
    Mesh* mesh = new Mesh();
    mesh->linkClass = mesh;
	
    return mesh;
}


Mesh::Mesh():XObject()
{
	udid = 0;
	assignedBaseAnimationsID = -1;
	nameMesh = "";

    typeIndexes = BASE_TYPE_INDEXES;
    
    numIndexPerData = 3;
    typeVertexData = GL_TRIANGLES;
	offsetInFile = 0;
    bindFrame = NULL;
    morphVertOriginal = NULL;
    numBones = 0;
    asyncLoaded = false;
    loadingViaHttp = false;
    idModel = 0;
    vertexDeclaration = NORMAL_MESH;


	initRotation = XVector3(0,0,0);
	initScale = XVector3(0,0,0);
	initTranslation = XVector3(0,0,0);
	numChunks = 0 ;
	particleData = false;
    normal   = false;
    binormal = false;
    tangent  = false;
    skinned  = false;
    color    = false;
    color4   = false;
	morphed  = false;

    sizeFloats = 0;
    typeVBO  = GL_STATIC_DRAW;
    
    numTextureCoordinats = 0;
    indexMeshInFile = 0;
    isDamaged = true;
    for (int i = 0; i < MAX_CHUNKS; i++) {
        indexChunksVBODamaged[i] = true;
        indexChunksIBODamaged[i] = true;


        faces[i] = NULL;
        vertex[i] = NULL;
    }
    
    
}


Mesh::~Mesh() {
    printf("remove mesh\n");

    clear();
}


void Mesh::deinitSkin() {
	if (bindFrame) {
        bindFrame->remove();
        free(bindFrame);
		bindFrame = NULL;
    }
	boneIerarhy.clear();
}

void Mesh::deinitMorph() {
	if (morphVertOriginal) {
        free(morphVertOriginal);
        morphVertOriginal = NULL;
    }
}


void Mesh::clear() {
	deinitSkin();
	deinitMorph();

	for (uint i = 0 ; i < numChunks; i++) {
        if (vertex[i]) {
            free(vertex[i]);
            vertex[i] = NULL;
        }
        
        if (faces[i]) {
            free(faces[i]);
            faces[i] = NULL;
        }
    }
}


void Mesh::initSkin() {
	deinitSkin();
	bindFrame = (SkinningFrame*)malloc(sizeof(SkinningFrame));
    bindFrame->init();
}

void Mesh::setAsDamaged() {
	isDamaged = true;
    for (int i = 0; i < MAX_CHUNKS; i++) {
        indexChunksVBODamaged[i] = true;
        indexChunksIBODamaged[i] = true;
    }
}

bool Mesh::isDamagedAll() {
	bool damagedALL = true;
	for (int i = 0; i < MAX_CHUNKS; i++) {
		if (indexChunksVBODamaged[i] && indexChunksIBODamaged[i]) {

		}
		else {
			damagedALL = false;
		}
	}
	return damagedALL;
}


void Mesh::setAsDamagedVBOChunk(uint chunk){
    indexChunksVBODamaged[chunk] = true;
    isDamaged = true;
}
void Mesh::setAsDamagedIBOChunk(uint chunk){
    indexChunksIBODamaged[chunk] = true;
    isDamaged = true;
}

uint Mesh::getSizeVertex() {
    sizeVertex      = 3*sizeof(float);
    if (numTextureCoordinats == 1) {
        sizeVertex += 2*sizeof(float);
    }
    if (tangent) {
        sizeVertex += 3*sizeof(float);
    }
    if (binormal) {
        sizeVertex += 3*sizeof(float);
    }
    if (normal) {
        sizeVertex += 3*sizeof(float);
    }
    if (color) {
        sizeVertex += 3*sizeof(float);
    }
    if (color4) {
        sizeVertex += 4*sizeof(float);
    }
	if (particleData) {
		sizeVertex += 4 * sizeof(float);
	}

    if (skinned) {
        sizeVertex += 4*sizeof(float);
        sizeVertex += 4*sizeof(float);
    }
    
    
    sizeFloats = sizeVertex/sizeof(float);
    
    return sizeVertex;
}




void Mesh::calculateOffsets(uint offset,uint index)
{
    sizeVertex      = 3*sizeof(float);
    offsets[index][ATTRIBUTE__OBJECT_POSITION] = (0+offset);
    
    if (numTextureCoordinats == 1) {
        offsets[index][ATTRIBUTE_TEXTURE_COORDINATS_1] = (sizeVertex+offset);
        sizeVertex += 2*sizeof(float);
    }
    if (tangent) {
        offsets[index][ATTRIBUTE_TANGENT] = (sizeVertex+offset);
        sizeVertex += 3*sizeof(float);
    }
    if (binormal) {
        offsets[index][ATTRIBUTE_BINORMAL] = (sizeVertex+offset);
        sizeVertex += 3*sizeof(float);
    }
    if (normal) {
        offsets[index][ATTRIBUTE_NORMAL] = (sizeVertex+offset);
        sizeVertex += 3*sizeof(float);
    }
    if (color) {
        offsets[index][ATTRIBUTE_COLOR] = (sizeVertex+offset);
        sizeVertex += 3*sizeof(float);
    }
    
    if (color4) {//
        offsets[index][ATTRIBUTE_COLOR4] = (sizeVertex+offset);
        sizeVertex += 4*sizeof(float);
    }

	if (particleData) {
		offsets[index][ATTRIBUTE_PARTICLE_DATA] = (sizeVertex + offset);
		sizeVertex += 4 * sizeof(float);
	}
    
    if (skinned) {
        offsets[index][ATTRIBUTE_BONES_WEIGHT] = (sizeVertex+offset);
        sizeVertex += 4*sizeof(float);
        
        offsets[index][ATTRIBUTE_BONES_IDS] = (sizeVertex+offset);
        sizeVertex += 4*sizeof(float);
    }
    
};
