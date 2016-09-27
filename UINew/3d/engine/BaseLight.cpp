//
//  BaseLight.cpp
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 26.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//

#include "BaseLight.h"

void BaseLight::setOrthoMatrix(float left, float right,
                                float bottom, float top,
                                float nearZ, float farZ) {


    XMatrix4MakeOrtho(&perspectiveMatrix,left, right, bottom, top, nearZ, farZ);
    
    XMatrix4 bias = XMatrix4(0.5f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.5f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.5f, 0.0f,
                     0.5f, 0.5f, 0.5f, 1.0f);
    
    XMatrix4Mult(&scaledTransletedProjMatrix,&bias, &perspectiveMatrix);
	// test
	isProjModelViewMatrixDerty = true;
	isProjModelViewMatrixSTDerty = true;
}



void BaseLight::lookAt(const vec3& pos, const vec3& point, const vec3& up)
{
	vec3 d = (point - pos).normalize();
    vec3 s = d.cross(up).normalize();
    vec3 u = s.cross(d).normalize();
    vec3 e(-s.dot(pos), -u.dot(pos), d.dot(pos));
    
    modelViewMatrix.m[0]  = s.x;modelViewMatrix.m[1]  = u.x;modelViewMatrix.m[2]  = -d.x;modelViewMatrix.m[3]  = 0.0;
    modelViewMatrix.m[4]  = s.y;modelViewMatrix.m[5]  = u.y;modelViewMatrix.m[6]  = -d.y;modelViewMatrix.m[7]  = 0.0;
    modelViewMatrix.m[8]  = s.z;modelViewMatrix.m[9]  = u.z;modelViewMatrix.m[10] = -d.z;modelViewMatrix.m[11] = 0.0;
    modelViewMatrix.m[12] = e.x;modelViewMatrix.m[13] = e.y;modelViewMatrix.m[14] =  e.z;modelViewMatrix.m[15] = 1.0;

	lookAtPos.x = pos.x;
	lookAtPos.y = pos.y;
	lookAtPos.z = pos.z;
    
    _pos.x = pos.x;
    _pos.y = pos.y;
    _pos.z = pos.z;

	lookAtView.x = point.x;
	lookAtView.y = point.y;
	lookAtView.z = point.z;

	lookAtUP.x = up.x;
	lookAtUP.y = up.y;
	lookAtUP.z = up.z;

	isTransformMatrixDerty = false;
    isNormalMatrixDerty    = true;
}


void BaseLight::setInfo(LightInfo*_info) {
	info = _info;
	this->setQuaternion(&info.quaternion);
	this->setTranslate(&info.translate);
}

LightInfo* BaseLight::getInfo() {
	info.quaternion = this->getQuaternion();
	info.translate = this->getPosition();
	return &info;
}

void BaseLight::init(){

	printf("hi here\n");

	typeObject = ObjectToolbarTypes::LIGHT_OBJECT;

    //XMatrix4Perspective(&perspectiveMatrix,60.0, 1.0, 0.01, 300.0);
    XMatrix4MakeOrtho(&perspectiveMatrix,-50.0f, 50.0f, -50.0f, 50.0f, 0.01f, 100.0f);
    
    XMatrix4 bias = XMatrix4(0.5f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.5f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.5f, 0.0f,
                     0.5f, 0.5f, 0.5f, 1.0f);
    
    XMatrix4Mult(&scaledTransletedProjMatrix,&bias, &perspectiveMatrix);

	isProjModelViewMatrixDerty = true;
	isProjModelViewMatrixSTDerty = true;
}


void BaseLight::calculateTransformMatrix() {
	SceneNode::calculateTransformMatrix();

	isProjModelViewMatrixDerty = true;
	isProjModelViewMatrixSTDerty = true;
}

XMatrix4* BaseLight::getPerspectiveMatrix(){
    return &perspectiveMatrix;
}



XMatrix4* BaseLight::getProjModelViewMatrix() {
    if (isProjModelViewMatrixDerty) {
        isProjModelViewMatrixDerty = false;
		XMatrix4Mult(&projModelViewMatrix,&perspectiveMatrix, getModelViewMatrix());
    }
    return &projModelViewMatrix;
}

XMatrix4* BaseLight::getProjModelViewMatrixST() {
    if (isProjModelViewMatrixSTDerty) {
        isProjModelViewMatrixSTDerty = false;
        XMatrix4Mult(&projModelViewMatrixST,&scaledTransletedProjMatrix, getModelViewMatrix());
    }
    return &projModelViewMatrixST;
}
