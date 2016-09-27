//
//  Camera.cpp
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 08.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//


#include "Camera.h"


#include "ResourceManager.h"

Camera::Camera(){
    typeObject = CAMERA_OBJECT;
    isFarCornersDerty = false;
    isProjModelViewDerty = true;
	isProjBillboardMatrixDerty = true;
	modelViewMatrixBillboard.identity();
	info.typeCamera = CameraTypes::CAMERA_FREE;
    ang	 = vec3(0,0,0);
};

CameraInfo* Camera::getInfo() { return &info; }

void Camera::setInfo(CameraInfo* val) { 
	info = val; 
	this->look();
}


XVector3 Camera::normalize(XVector3 vVector) {
    float magnitude_ = magnitude(vVector);
    
    vVector.x = vVector.x / magnitude_;
    vVector.y = vVector.y / magnitude_;
    vVector.z = vVector.z / magnitude_;
    
    return vVector;
}

float Camera::magnitude(XVector3 vNormal) {
    return (float)sqrt((vNormal.x * vNormal.x) +
                       (vNormal.y * vNormal.y) +
                       (vNormal.z * vNormal.z));
}

XVector3 Camera::cross(XVector3 vV1, XVector3 vV2, XVector3 vVector2) {
    XVector3 vNormal;
    XVector3 vVector1;
    vVector1.x = vV1.x - vV2.x;
    vVector1.y = vV1.y - vV2.y;
    vVector1.z = vV1.z - vV2.z;

    vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
    vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
    vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
    return vNormal;
}


void Camera::look()
{
    lookAt(vec3(info.lookAtPos.x, info.lookAtPos.y, info.lookAtPos.z),
           vec3(info.lookAtView.x, info.lookAtView.y, info.lookAtView.z),
           vec3(info.lookAtUp.x, info.lookAtUp.y, info.lookAtUp.z));
}

void Camera::upDown(float speed)
{
    info.lookAtPos.y += speed;
}


XVector3 Camera::getDirection() const {
	XVector3 temp;
    temp.y = -cos(ang.z * PI / 180.0f)*sin(ang.x * PI / 180.0f);
    temp.x = -sin(ang.z * PI / 180.0f)*sin(ang.x * PI / 180.0f);
    temp.z = -cos(ang.x * PI / 180.0f);
    return temp;
}

void Camera::moveForward(float d) {


    info.lookAtPos.y += (float)cos((ang.z) * PI / 180.0f) * d;
    info.lookAtPos.x += (float)sin((ang.z) * PI / 180.0f) * d;
}

//--------------------------------------------------------------------------

void Camera::moveBackward(float d) {
    info.lookAtPos.y -= (float)cos((ang.z) * PI / 180.0f) * d;
    info.lookAtPos.x -= (float)sin((ang.z) * PI / 180.0f) * d;
}

//--------------------------------------------------------------------------

void Camera::moveLeft(float d) {
    info.lookAtPos.y -= (float)cos((ang.z + 90) * PI / 180.0f) * d;
    info.lookAtPos.x -= (float)sin((ang.z + 90) * PI / 180.0f) * d;
}

//--------------------------------------------------------------------------

void Camera::moveRight(float d) {
    info.lookAtPos.y += (float)cos((ang.z + 90) * PI / 180.0f) * d;
    info.lookAtPos.x += (float)sin((ang.z + 90) * PI / 180.0f) * d;
}



float Camera::getNearPlane() {
    return mNearZ;
}



XVector3 Camera::project(const XVector3 &v)  {
    
	XVector3 r;
    
    float fTempo[8];
    
    fTempo[0] = modelViewMatrix.m[0] * v.x + modelViewMatrix.m[4] * v.y + modelViewMatrix.m[8] * v.z + modelViewMatrix.m[12];
    fTempo[1] = modelViewMatrix.m[1] * v.x + modelViewMatrix.m[5] * v.y + modelViewMatrix.m[9] * v.z + modelViewMatrix.m[13];
    fTempo[2] = modelViewMatrix.m[2] * v.x + modelViewMatrix.m[6] * v.y + modelViewMatrix.m[10] * v.z + modelViewMatrix.m[14];
    fTempo[3] = modelViewMatrix.m[3] * v.x + modelViewMatrix.m[7] * v.y + modelViewMatrix.m[11] * v.z + modelViewMatrix.m[15];
    
    fTempo[4] = perspectiveMatrix.m[0] * fTempo[0] + perspectiveMatrix.m[4] * fTempo[1] + perspectiveMatrix.m[8] * fTempo[2] + perspectiveMatrix.m[12] * fTempo[3];
    fTempo[5] = perspectiveMatrix.m[1] * fTempo[0] + perspectiveMatrix.m[5] * fTempo[1] + perspectiveMatrix.m[9] * fTempo[2] + perspectiveMatrix.m[13] * fTempo[3];
    fTempo[6] = perspectiveMatrix.m[2] * fTempo[0] + perspectiveMatrix.m[6] * fTempo[1] + perspectiveMatrix.m[10] * fTempo[2] + perspectiveMatrix.m[14] * fTempo[3];
    fTempo[7] = -fTempo[2];
    
    if (abs(fTempo[7]) < EPSILON) {
        return XVector3();
    }
    
    fTempo[7] = 1.0f / fTempo[7];
    
    fTempo[4] *= fTempo[7];
    fTempo[5] *= fTempo[7];
    fTempo[6] *= fTempo[7];
    
    r.v[0] = (fTempo[4] * 0.5f + 0.5f) * viewport[2] + viewport[0];
    r.v[1] = (fTempo[5] * 0.5f + 0.5f) * viewport[3] + viewport[1];
    r.v[2] = (1.0f + fTempo[6]) * 0.5f;
    
    return XVector3(r.v[0], float(viewport[3]) - r.v[1], r.v[2]);
}


XVector3 Camera::unproject(const XVector3 &v)  {
    XVector4 v4,tmp;
    
    XMatrix4 m;
    XMatrix4* modelViewProj = getProjModelViewMatrix();
    XMatrix4Inverse(&m,modelViewProj);
    
    
    v4.x = (v.x - float(viewport[0])) / float(viewport[2]) * 2.0f - 1.0f;
    v4.y = viewport[3] - v.y;
    v4.y = (v4.y - float(viewport[1])) / float(viewport[3]) * 2.0f - 1.0f;
    v4.z = 2.0f * v.z - 1.0f;
    v4.w = 1.0f;
    
    
    XMatrix4MultiplyVector4(&tmp,&m,&v4);
    
    v4 = tmp;
    
    if (abs(v4.w) < EPSILON) {
        return XVector3();
    }
    v4.w = 1.0f / v4.w;
    
	XVector3 result = XVector3(v4.x * v4.w, v4.y * v4.w, v4.z * v4.w);
    return result;
}



XLine Camera::getRay(float x, float y)  {
    if (viewport[3] == 0 || viewport[2] == 0) return XLine(XVector3(0, 0, 0), XVector3(0, 0, 1));
	XVector3 src = unproject(XVector3(x, y, 0.1f));
	XVector3 dst = unproject(XVector3(x, y, 0.9f));
    return XLine(src, dst);
}


void Camera::setViewport(float x,float y,float width,float height) {
    viewport[0] = x;
    viewport[1] = y;
    viewport[2] = width;
    viewport[3] = height;
}

void Camera::strafe(float speed)
{
    
    info.lookAtPos.x += mStrafe.x * speed;
    info.lookAtPos.z += mStrafe.z * speed;
    info.lookAtView.x += mStrafe.x * speed;
    info.lookAtView.z += mStrafe.z * speed;
}

void Camera::moveCamera(float speed)
{
    
    XVector3 vVector;
    vVector.x = info.lookAtView.x - info.lookAtPos.x;
    vVector.y = info.lookAtView.y - info.lookAtPos.y;
    vVector.z = info.lookAtView.z - info.lookAtPos.z;
    
    vVector.y = 0.0f;
    vVector = normalize(vVector);
    
    info.lookAtPos.x += vVector.x * speed;
    info.lookAtPos.z += vVector.z * speed;
    info.lookAtView.x += vVector.x * speed;
    info.lookAtView.z += vVector.z * speed;
}


void Camera::rotatePosition(float angle, float x, float y, float z)
{
    
    info.lookAtPos.x = info.lookAtPos.x - info.lookAtView.x;
    info.lookAtPos.y = info.lookAtPos.y - info.lookAtView.y;
    info.lookAtPos.z = info.lookAtPos.z - info.lookAtView.z;
    
    XVector3 vVector = info.lookAtPos;
    XVector3 AVector;
    
    float SinA = (float)sin(M_PI * angle / 180.0);
    float CosA = (float)cos(M_PI * angle / 180.0);
    
    AVector.x = (CosA + (1 - CosA) * x * x) * vVector.x;
    AVector.x += ((1 - CosA) * x * y - z * SinA) * vVector.y;
    AVector.x += ((1 - CosA) * x * z + y * SinA) * vVector.z;
    
    AVector.y = ((1 - CosA) * x * y + z * SinA) * vVector.x;
    AVector.y += (CosA + (1 - CosA) * y * y) * vVector.y;
    AVector.y += ((1 - CosA) * y * z - x * SinA) * vVector.z;
    
    AVector.z = ((1 - CosA) * x * z - y * SinA) * vVector.x;
    AVector.z += ((1 - CosA) * y * z + x * SinA) * vVector.y;
    AVector.z += (CosA + (1 - CosA) * z * z) * vVector.z;
    
    info.lookAtPos.x = info.lookAtView.x + AVector.x;
    info.lookAtPos.y = info.lookAtView.y + AVector.y;
    info.lookAtPos.z = info.lookAtView.z + AVector.z;
}

void Camera::rotateView(float speed)
{
    XVector3 vVector;
    vVector.x = info.lookAtView.x - info.lookAtPos.x;
    vVector.y = info.lookAtView.y - info.lookAtPos.y;
    vVector.z = info.lookAtView.z - info.lookAtPos.z;
    
    
    
    info.lookAtView.z = (float)(info.lookAtPos.z + sin(speed) * vVector.x + cos(speed) * vVector.z);
    info.lookAtView.x = (float)(info.lookAtPos.x + cos(speed) * vVector.x - sin(speed) * vVector.z);
}





void Camera::setPerspective(float angle, float width, float height, float nearZ,  float farPlane) {
    mFarPlane = farPlane;
    mAspectRatio = (float) width / (float) height;
    mAngle = angle;
    mNearZ = nearZ;
    mtxLoadPerspective(&perspectiveMatrix.m[0], mAngle, (float)width / (float)height,nearZ,farPlane);
    
    isFarCornersDerty = false;
	isProjBillboardMatrixDerty = true;
    isProjModelViewDerty = true;
}


void Camera::setPerspectivematrix(float* data) {
    XMatrix4MakeWithArray(&perspectiveMatrix, data);
    
    
    isFarCornersDerty = false;
	isProjBillboardMatrixDerty = true;
    isProjModelViewDerty = true;
}



void Camera::lookAt(const vec3& pos, const vec3& point, const vec3& up)
{
    vec3 d = (point - pos).normalize();
    vec3 s = d.cross(up).normalize();
    vec3 u = s.cross(d).normalize();
    vec3 e(-s.dot(pos), -u.dot(pos), d.dot(pos));
    
    modelViewMatrix.m[0]  = s.x;modelViewMatrix.m[1]  = u.x;modelViewMatrix.m[2]  = -d.x;modelViewMatrix.m[3]  = 0.0;
    modelViewMatrix.m[4]  = s.y;modelViewMatrix.m[5]  = u.y;modelViewMatrix.m[6]  = -d.y;modelViewMatrix.m[7]  = 0.0;
    modelViewMatrix.m[8]  = s.z;modelViewMatrix.m[9]  = u.z;modelViewMatrix.m[10] = -d.z;modelViewMatrix.m[11] = 0.0;
    modelViewMatrix.m[12] = e.x;modelViewMatrix.m[13] = e.y;modelViewMatrix.m[14] =  e.z;modelViewMatrix.m[15] = 1.0;
    
	modelViewMatrixBillboard.m[12] = modelViewMatrix.m[12];
	modelViewMatrixBillboard.m[13] = modelViewMatrix.m[13];
	modelViewMatrixBillboard.m[14] = modelViewMatrix.m[14];


	isProjBillboardMatrixDerty = true;

    info.lookAtPos.x = pos.x;
    info.lookAtPos.y = pos.y;
    info.lookAtPos.z = pos.z;
    
    info.lookAtView.x = point.x;
    info.lookAtView.y = point.y;
    info.lookAtView.z = point.z;
    
    info.lookAtUp.x = up.x;
    info.lookAtUp.y = up.y;
    info.lookAtUp.z = up.z;
    
    XVector3 vCross = cross(info.lookAtView, info.lookAtPos, info.lookAtUp);
    mStrafe = normalize(vCross);
    
    
    
    
    vec3 v = point - pos;
    float cosa = dot_(normalize_(v), vec3(0, 0, 1));
    ang.x = (arccos_(cosa) - PI) * RAD_TO_DEG;
    v = point - pos;
    v.z = 0;
    v = normalize_(v);
    cosa = dot_(v, vec3(0, -1, 0));
    float sign = 1.0f;
    if (cross_(v, vec3(0, -1, 0)).z < 0.0f) sign = -sign;
    ang.z = (arccos_(cosa) * sign - PI) * RAD_TO_DEG;
    
    
	isProjBillboardMatrixDerty = true;
    isProjModelViewDerty = true;
}

void Camera::resetModelViewMatrix() {
    modelViewMatrix.m[0]  = 1.0;modelViewMatrix.m[1]  = 0.0;modelViewMatrix.m[2]  = 0.0;modelViewMatrix.m[3]  = 0.0;
    modelViewMatrix.m[4]  = 0.0;modelViewMatrix.m[5]  = 1.0;modelViewMatrix.m[6]  = 0.0;modelViewMatrix.m[7]  = 0.0;
    modelViewMatrix.m[8]  = 0.0;modelViewMatrix.m[9]  = 0.0;modelViewMatrix.m[10] = 1.0;modelViewMatrix.m[11] = 0.0;
    modelViewMatrix.m[12] = 0.0;modelViewMatrix.m[13] = 0.0;modelViewMatrix.m[14] = 0.0;modelViewMatrix.m[15] = 1.0;
    

	modelViewMatrixBillboard.identity();

    info.lookAtPos.x = 0;
    info.lookAtPos.y = 0;
    info.lookAtPos.z = 0;
    
    info.lookAtView.x = 0;
    info.lookAtView.y = 0;
    info.lookAtView.z = 0;
    
    info.lookAtUp.x = 0;
    info.lookAtUp.y = 0;
    info.lookAtUp.z = 0;
    
	isProjBillboardMatrixDerty = true;
    isProjModelViewDerty = true;
}
void Camera::setModelViewMatrix(float* matrix) {
    XMatrix4MakeWithArray(&modelViewMatrix, matrix);

	modelViewMatrixBillboard.m[12] = modelViewMatrix.m[12];
	modelViewMatrixBillboard.m[13] = modelViewMatrix.m[13];
	modelViewMatrixBillboard.m[14] = modelViewMatrix.m[14];

	isProjBillboardMatrixDerty = true;
    isProjModelViewDerty = true;
}



void Camera::extractCameraPos(XVector3*result, XMatrix4 * a_modelView)
{
    XMatrix4 modelViewT;
    XMatrix4Transpose(&modelViewT,a_modelView);
    
    vec3 n1(modelViewT.m[0],modelViewT.m[1],modelViewT.m[2]);
    vec3 n2(modelViewT.m[4],modelViewT.m[5],modelViewT.m[6]);
    vec3 n3(modelViewT.m[8],modelViewT.m[9],modelViewT.m[10]);
    
    float d1(modelViewT.m[3]);
    float d2(modelViewT.m[7]);
    float d3(modelViewT.m[11]);
    
    vec3 n2n3 = n2.cross(n3);
    vec3 n3n1 = n3.cross(n1);
    vec3 n1n2 = n1.cross(n2);
    
    vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
    float denom = n1.dot(n2n3);
    vec3 tmp = top / -denom;
    
    result->x = tmp.x;
    result->y = tmp.y;
    result->z = tmp.z;
}


XVector3* Camera::getCameraPosition() {
    return &info.lookAtPos;
}
XMatrix4* Camera::getPerspectiveMatrix() {
    return &perspectiveMatrix;
}

XMatrix4* Camera::getModelViewMatrix() {
    return &modelViewMatrix;
}

XMatrix4 * Camera::getModelViewMatrixBillboard()
{
	return &modelViewMatrixBillboard;
}


//

XMatrix4* Camera::getProjModelViewMatrixBillboard() {
    if (isProjBillboardMatrixDerty) {
        isProjBillboardMatrixDerty = false;
        
        XMatrix4Mult(&projModelViewMatrixBillboard, &perspectiveMatrix, &modelViewMatrixBillboard);
    }
    return &projModelViewMatrixBillboard;
}

XMatrix4* Camera::getProjModelViewMatrix() {
    if (isProjModelViewDerty) {
        isProjModelViewDerty = false;
       
        XMatrix4Mult(&projModelViewMatrix, &perspectiveMatrix, &modelViewMatrix);
        
              
    }
    return &projModelViewMatrix;
}


void Camera::calculateFarCorners(){
    
    float FOV =  (mAngle * PI) / 180.0f;
    float tanHalfFOV = tanf(FOV/ 2.0f);
    float farY       = tanHalfFOV * mFarPlane;
    float farX       = farY       * mAspectRatio;
    
    
    
    
    farCorners[0] = XVector3Make(-farX, -farY, -mFarPlane);
    farCorners[1] = XVector3Make( farX, -farY, -mFarPlane);
    farCorners[2] = XVector3Make( farX,  farY, -mFarPlane);
    farCorners[3] = XVector3Make(-farX,  farY, -mFarPlane);
}
float Camera::getFarPlane(){
    return mFarPlane;
}

XVector3* Camera::getFarcorners(){
    if (isFarCornersDerty) {
        isFarCornersDerty = false;
        calculateFarCorners();
    }
    return &farCorners[0];
}



