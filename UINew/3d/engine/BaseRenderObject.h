//
//  BaseRenderObject.h
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 26.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//
#ifndef BASE_RENDER_OBJECT_H__
#define BASE_RENDER_OBJECT_H__


#include "mathLib.h"
#include "Material.h"
#include "Mesh.h"
#include "XRendererDefines.h"

#include "XMathTypes.h"
#include "XMatrix4Math.h"
#include "XMatrix3Math.h"
#include "XBoundingBox.h"
#include "XAnimTypes.h"
#include "Object.hpp"

typedef struct {
    int *passes;
    int length;
}PassesObject;


class BaseRenderObject:public Object {
    
private:
    
    XBoundingBox*bbox;
    uint udid;
protected:
    XMatrix4 modelViewMatrix;
    Material*material;
    Mesh*model;
    XMatrix3 normalMatrix;
    
	XVector4 quaternion;

	bool isQuaternionDerty;
	bool isRotationEulerDerty;
    bool isNormalMatrixDerty;
    bool isTransformMatrixDerty;

	

	XVector3 _pos,_scale,_rotation;
	bool _isRemoved;
public:
	bool isRemoved() {
		return _isRemoved;
	};
	void isRemoved(bool val) {
		isVisible = !val; // XTODO: hack removing obj
		_isRemoved = val;
	};

    
    virtual void setVisible(bool val) {
        isVisible = val;
    };
    
	bool isVisible;
	void setNormalRender();
	void setWireFrameRender();

	uint customVertexData;
	bool isCustomTypeVertex;

    virtual void update(uint frameID,double delta = 0.0) = 0;
    virtual void endUpdate() = 0;
    
    

    
    bool castShadow;
    bool resaveShadow;
	XVector3 cameraDir;
    float indexMesh;
    PassesObject objectsPasses;
    
    

	virtual XVector3* getManipulatorPosition();
	XVector3* getRotation();
	XVector3 getRotationDegress();
	XVector3* getPosition();
	XVector3* getScale();

	bool waitEndUpdate;


    void resetTransformMatrix();
    
    // bbox methods !
    XBoundingBox* getBBox();
    XVector3* getCentroid();
  
    Mesh*getMesh();
    void setMesh(Mesh*mesh);
    
    bool visible;
    Material*getMaterial();
    void setMaterial(Material*mtl);
    
    
    virtual void resetViewMatrixToInit() = 0;
    
    
    uint tehnic;
    
    virtual void zoom(float value) = 0;
    virtual void touchesBegan(float x,float y) = 0;
    virtual void touchesMoved(float x,float y) = 0;
    virtual void swipeEvent(uint direction) = 0;
    
    BaseRenderObject();
    virtual ~BaseRenderObject();
    
    virtual void setModelViewMatrixF(float*data);
    virtual void setNormalMatrix(float*data);
    
    virtual XMatrix4* getModelViewMatrix();
    virtual XMatrix3* getNormalMatrix();

	virtual void setRotationEulerinDegress(XVector3*rhs) = 0;
	virtual void setRotationEulerinRadians(XVector3*rhs) = 0;
	virtual void setQuaternion(XVector4*ref) = 0;

	virtual XVector4* getQuaternion() = 0;
	virtual XVector3* getEulerRadians() = 0;

	virtual void setTranslate(XVector3 * pos) = 0;
	virtual void setX(float value) = 0;
    virtual void setY(float value) = 0;
    virtual void setZ(float value) = 0;
    
    virtual void setScaleX(float value) = 0;
    virtual void setScaleY(float value) = 0;
    virtual void setScaleZ(float value) = 0;

	virtual void setScale(XVector3 *scale) = 0;

    virtual void calculateNormalMatrix(){};
    virtual void calculateTransformMatrix(){};

    virtual SkinningFrame* getSkinAnimation(bool update){return NULL;};
    
    
	

    uint getUDID();
};

#endif