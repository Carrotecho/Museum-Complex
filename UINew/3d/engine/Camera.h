//
//  CameraOld.h
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 08.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//


#ifndef __XCamera__
#define __XCamera__

#include "XTypes.h"
#include "XMatrix4Math.h"
#include "XMathTypes.h"
#include "XMathBase.h"

#include "Types.h"
#include "Vectors.h"
#include "Object.hpp"


class Camera:public Object{
private:
    bool isFarCornersDerty,isProjModelViewDerty, isProjBillboardMatrixDerty;
    float mFarPlane,mAspectRatio,mAngle,mNearZ;
    
    //farCorners
    
    XVector3 farCorners[4];
    
    XMatrix4 perspectiveMatrix;
    XMatrix4 modelViewMatrix;
	XMatrix4 modelViewMatrixBillboard;

	XMatrix4 projModelViewMatrixBillboard;
    XMatrix4 projModelViewMatrix;


	CameraInfo info;
	

	vec3 ang;


    XVector3 mStrafe; // Вектор для стрейфа (движения влево и вправо) камеры

	XVector3 cross(XVector3 vV1, XVector3 vV2, XVector3 vVector2);
	float magnitude(XVector3 vNormal);
	XVector3 normalize(XVector3 vVector);
	void extractCameraPos(XVector3*result, XMatrix4 * a_modelView);

	float viewport[4];
public:
	CameraInfo* getInfo();
	void setInfo(CameraInfo* val);
	float getNearPlane();
	void setViewport(float x,float y,float width,float height);

	XLine getRay(float x, float y);
	XVector3 project(const XVector3 &v);
	XVector3 unproject(const XVector3 &v);

	XVector3 getDirection() const;
	void moveForward(float d);
	void moveBackward(float d);
	void moveLeft(float d);
	void moveRight(float d);

	void look();
	void upDown(float speed);
	void strafe(float speed);
	void moveCamera(float speed);
	void rotatePosition(float angle, float x, float y, float z);
	void rotateView(float speed);


    XMatrix4* getPerspectiveMatrix();
    XMatrix4* getModelViewMatrix();
	XMatrix4* getModelViewMatrixBillboard();

    
    XMatrix4* getProjModelViewMatrixBillboard();
    XMatrix4* getProjModelViewMatrix();
    XVector3* getCameraPosition();
    
    void setPerspectivematrix(float* data);
    void resetModelViewMatrix();
    
    XVector3* getFarcorners();
    
    float getFarPlane();
    void calculateFarCorners();
    void lookAt(const vec3& pos, const vec3& point, const vec3& up);
   
    
    
    void setModelViewMatrix(float* matrix);
    void setPerspective(float angle, float width, float height, float nearZ,  float farPlane);
    Camera();
};

#endif /* defined(__XCamera__) */