//
//  BaseRenderObject.cpp
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 26.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//

#include "BaseRenderObject.h"
#include "XUDID.h"


BaseRenderObject::BaseRenderObject(){
	typeObject = ObjectToolbarTypes::MESH_OBJECT;
    castShadow = false;

	_isRemoved = false;
	isVisible = true;

    
    material = nullptr;
    
    resaveShadow = false;
    tehnic = BACK_BUFFER_TEHNICS;
    model = NULL;
    XMatrix4MakeWithArray(&modelViewMatrix,
		               1.0,0.0,0.0,0.0,
                       0.0,1.0,0.0,0.0,
                       0.0,0.0,1.0,0.0,
                       0.0,0.0,0.0,1.0);

    




    resetTransformMatrix();
    indexMesh = 0;
}




BaseRenderObject::~BaseRenderObject(){
    if (model) {
        model->release();
    }
    printf("BaseRenderObject node delete\n");
}


uint BaseRenderObject::getUDID() {
    return udid;
}

XBoundingBox* BaseRenderObject::getBBox() {
    // XTODO : bbox create ! ( need get bbox from mesh )
    return NULL;
}
XVector3* BaseRenderObject::getCentroid() {
    // XTODO : calulate bbox ;
    return NULL;
}
void BaseRenderObject::setNormalRender() {
	isCustomTypeVertex = false;
	if(model != NULL) {
		customVertexData = model->typeVertexData;
	}
}
void BaseRenderObject::setWireFrameRender() {
	isCustomTypeVertex = true;
	customVertexData = GL_LINES;
}


XVector3* BaseRenderObject::getScale() {
	return &_scale;
}

XVector3* BaseRenderObject::getManipulatorPosition() {
	return &_pos;
}

XVector3* BaseRenderObject::getPosition() {
	return &_pos;
}
XVector3* BaseRenderObject::getRotation() {
	return &_rotation;
}
XVector3 BaseRenderObject::getRotationDegress() {
	return _rotation*RAD_TO_DEG;
}

void BaseRenderObject::resetTransformMatrix() {

    
    modelViewMatrix.identity();
    normalMatrix.identity();

	quaternion.x = 0.0;
	quaternion.y = 0.0;
	quaternion.z = 0.0;
	quaternion.w = 0.0;

    _pos.x = 0;
    _pos.y = 0;
    _pos.z = 0;

    _rotation.x = 0;
    _rotation.y = 0;
    _rotation.z = 0;

	_scale.x = 1;
	_scale.y = 1;
	_scale.z = 1;

	waitEndUpdate = false;

	isQuaternionDerty      = false;
	isRotationEulerDerty   = false;
    isTransformMatrixDerty = false;
    isNormalMatrixDerty    = false;
    
    
}



void BaseRenderObject::setModelViewMatrixF(float*data) {
	isNormalMatrixDerty    = true;
	isTransformMatrixDerty = false;
    XMatrix4MakeWithArray(&modelViewMatrix, data);
}

void BaseRenderObject::setNormalMatrix(float*data){
	isNormalMatrixDerty    = false;
	normalMatrix = XMatrix3MakeWithArray(data);
}

XMatrix4* BaseRenderObject::getModelViewMatrix() {
    if (isTransformMatrixDerty) {
        calculateTransformMatrix();
    }
    return &modelViewMatrix;
}

XMatrix3* BaseRenderObject::getNormalMatrix(){
    if (isTransformMatrixDerty){
        calculateTransformMatrix();
    }
    if (isNormalMatrixDerty){
        calculateNormalMatrix();
    }
    return &normalMatrix;
}

Material*BaseRenderObject::getMaterial(){
    return material;
}
void BaseRenderObject::setMaterial(Material*mtl){
    material = mtl;
}
Mesh*BaseRenderObject::getMesh(){
    return model;
}

void BaseRenderObject::setMesh(Mesh*mesh){
    if (model) model->release();
    model = (Mesh*)mesh->retain();

	if(model != NULL) {
		customVertexData = model->typeVertexData;
	}
}

