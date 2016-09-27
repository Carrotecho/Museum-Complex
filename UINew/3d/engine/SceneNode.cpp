//
//  SceneNode.cpp
//  ShadowMapsTestProject
//
//  Created by easy on 25.11.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "SceneNode.h"
#include "ShaderUtils.h"
#include "ShaderCache.h"



SceneNode::SceneNode():BaseRenderObject()
{
	lastFrameID = 0;
    zTest = true;
    blendFunction    = false;
    blendFunctionIn  = 0;
    blendFunctionOut = 0;
    
	parent = nullptr;

    double now = 0;
    uid = now;
    
    
    needValidateMaterial = true;
    
    isVisible = true;
    

    resetTransformMatrix();

    
    isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;

	normalMatrix.identity();
}

SceneNode::~SceneNode(){
    for (uint i = 0 ; i < this->childs.size(); i++) {
        SceneNode* n = this->childs[i];
        delete n;
        this->childs[i] = NULL;
    }
    this->childs.clear();
    this->childs.resize(0);
    
    printf("scene node delete\n");
}



void SceneNode::resetViewMatrixToInit() {
    if (model) {
        this->setTranslate(&model->initTranslation);
        this->setQuaternion(&model->initQuaternion);
        this->setScale(&model->initScale);
    }
    
    for (int i = 0; i < childs.size(); i++) {
        childs[i]->resetViewMatrixToInit();
    }
}


void SceneNode::calculateNormalMatrix()
{

	XMatrix4 inverted,transposed;


    bool isInvertable = false;
    normalMatrix = XMatrix3(modelViewMatrix.m[0], modelViewMatrix.m[1], modelViewMatrix.m[2],
                            modelViewMatrix.m[4], modelViewMatrix.m[5], modelViewMatrix.m[6],
                            modelViewMatrix.m[8], modelViewMatrix.m[9], modelViewMatrix.m[10]);

    normalMatrix = XMatrix3Invert(&normalMatrix, &isInvertable);
	


    isNormalMatrixDerty = false;
}

void SceneNode::calculateTransformMatrix() {
	
	for (uint i = 0;i < childs.size(); i++) {
		childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
	}

	XMatrix4MakeWithQuaternionAndTranslation2(&modelViewMatrix,this->getQuaternion(),&_pos);
	XMatrix4Scale(&modelViewMatrix, _scale.x, _scale.y, _scale.z);


	if (this->parent != nullptr) {
        modelViewMatrix = XMatrix4Mult(this->parent->getModelViewMatrix(),&modelViewMatrix);
	}

    isTransformMatrixDerty = false;
	isNormalMatrixDerty = true;
}

void SceneNode::setModelViewMatrixF(float*data) {
    
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
    
    isNormalMatrixDerty    = true;
    isTransformMatrixDerty = false;
    
    
    
    XMatrix4MakeWithArray(&modelViewMatrix, data);
}

void SceneNode::setNormalMatrix(float*data){
    
    isNormalMatrixDerty    = false;
    normalMatrix = XMatrix3MakeWithArray(data);
}


XMatrix4* SceneNode::getModelViewMatrix() {

    
    if (isTransformMatrixDerty ) {
        calculateTransformMatrix();
    }
    return &modelViewMatrix;
}

XMatrix3* SceneNode::getNormalMatrix() {

    
    if (isTransformMatrixDerty){
        calculateTransformMatrix();
    }
    if (isNormalMatrixDerty){
        calculateNormalMatrix();
    }
    return &normalMatrix;
}

void SceneNode::setScale(XVector3 *scale)
{
	_scale = scale;
	isTransformMatrixDerty = true;
	isNormalMatrixDerty = true;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}

void SceneNode::setScaleX(float value)
{
	if (_scale.x == value) return;
    isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;
    _scale.x = value;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}

void SceneNode::setScaleY(float value)
{
	if (_scale.y == value) return;
    isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;
    _scale.y = value;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}

void SceneNode::setScaleZ(float value)
{
	if (_scale.z == value) return;
    isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;
    _scale.z = value;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}


void SceneNode::setTranslate(XVector3*pos) {
	isTransformMatrixDerty = true;
	isNormalMatrixDerty = true;
	_pos = pos;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}

void SceneNode::setX(float value)
{
	if (_pos.x == value) return;
    isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;
    _pos.x = value;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}

void SceneNode::setY(float value)
{
	if (_pos.y == value) return;
    isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;
    _pos.y = value;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}

void SceneNode::setZ(float value)
{
	if (_pos.z == value) return;
    isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;
    _pos.z = value;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}






XVector4* SceneNode::getQuaternion() {
	if (isQuaternionDerty) {
		XQuaternionFromEuler(&quaternion, &_rotation);
		isQuaternionDerty = false;
	}
	return &quaternion;
}

XVector3* SceneNode::getEulerRadians() {
	if (isRotationEulerDerty) {
		XEulerFromQuaternion(&_rotation,&quaternion);
		isRotationEulerDerty = false;
	}
	return &_rotation;
}

void SceneNode::setQuaternion(XVector4*ref) {
	quaternion = ref;

	isRotationEulerDerty = true;
	isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}

float SceneNode::formatDegress(float value) {
	bool minus = value < 0 ? true : false;
	value = abs(value);
	if (value > 360.0) {
		value = value - (360.0 * floor(value / 360.0));
	}
	value = minus ? -value: value;
	return value;
}

void SceneNode::setRotationEulerinDegress(XVector3*rhs)
{
	_rotation.x = formatDegress(rhs->x);
	_rotation.y = formatDegress(rhs->y);
	_rotation.z = formatDegress(rhs->z);



	_rotation*=DEG_TO_RAD;

	isQuaternionDerty = true;
	isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}

void SceneNode::setRotationEulerinRadians(XVector3*rhs)
{
	_rotation = rhs;
	isQuaternionDerty = true;
	isTransformMatrixDerty = true;
    isNormalMatrixDerty    = true;
    for (uint i = 0;i < childs.size(); i++) {
        childs[i]->isTransformMatrixDerty = true;
        childs[i]->isNormalMatrixDerty = true;
    }
}



























void SceneNode::addChildsToRenderList(std::vector<SceneNode*> *list)
{
    list->push_back(this);
    for (unsigned int i = 0;i<childs.size();i++)
    {
        childs[i]->addChildsToRenderList(list);
    }
}

void SceneNode::addLight(Object*object)
{
    lights.push_back(object);
}

void SceneNode::addChild(SceneNode*object)
{
    childs.push_back(object);
    object->parent = this;
}




