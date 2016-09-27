//
//  ObjectView.h
//  ShadowMapsTestProject
//
//  Created by easy on 25.11.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//
#ifndef SCENENODE_H__
#define SCENENODE_H__

#include "Types.h"
#include "mathLib.h"
#include "Mesh.h"
#include <vector>
#include "Material.h"
#include <cmath>

#include "BaseRenderObject.h"


class SceneNode:public BaseRenderObject {
protected:
	 XMatrix4 tmpMatrix,tmpMatrix2;
private:
   
    uint flags;
public:

    
    bool zTest;
    bool blendFunction;
    uint blendFunctionIn;
    uint blendFunctionOut;

    uint lastFrameID;
    SceneNode();
    virtual ~ SceneNode();
    
    std::vector<SceneNode*> *childsList;
    
    SceneNode* parent;
     
    double uid;
    
    bool needValidateMaterial;
    
    
    std::vector<SceneNode*> childs;
    std::vector<Object*>    lights;
    
    virtual void calculateNormalMatrix() override;
    virtual void calculateTransformMatrix() override;
    virtual SkinningFrame* getSkinAnimation(bool update) override {return NULL;};

    virtual void setVisible(bool val) override {
        for (uint i = 0; i < childs.size(); i++) {
            childs[i]->setVisible(val);
        }
        isVisible = val;
    }
    
    bool isValidMeshNode() {
        bool retVal = true;
        for (uint i = 0; i < childs.size(); i++) {
            retVal = retVal && childs[i]->isValidMeshNode();
        }
        
        if (this->getMesh() != nullptr) {
            retVal = retVal && !this->getMesh()->isDamaged;
        }
        if (this->getMaterial() != nullptr && this->needValidateMaterial) {
            retVal = retVal && this->getMaterial()->validate();
        }
        
        
        return retVal;
    }
    
    virtual void resetViewMatrixToInit() override;
    
    virtual void zoom(float value) override {
        
    };

    virtual void touchesBegan(float x,float y) override {
        
    };
    virtual void touchesMoved(float x,float y) override {
        
    };
    
    virtual void swipeEvent(uint direction) override {
        
    };
    
    virtual XMatrix4* getModelViewMatrix() override;
    virtual XMatrix3* getNormalMatrix() override;

    virtual void setModelViewMatrixF(float*data) override;
    virtual void setNormalMatrix(float*data) override;
    
    virtual void setRotationEulerinDegress(XVector3*rhs) override;
	virtual void setRotationEulerinRadians(XVector3*rhs) override;
	virtual void setQuaternion(XVector4*ref) override;

	float formatDegress(float value) ;

	virtual XVector4* getQuaternion() override;
	virtual XVector3* getEulerRadians() override;

	virtual void setTranslate(XVector3 * pos) override;

	virtual void setX(float value) override;
    virtual void setY(float value) override;
    virtual void setZ(float value) override;
    
    virtual void setScaleX(float value) override;
    virtual void setScaleY(float value) override;
    virtual void setScaleZ(float value) override;
	virtual void setScale(XVector3 *scale) override;


    void addChildsToRenderList(std::vector<SceneNode*> *list);
    
    void addLight(Object*object);
    void addChild(SceneNode*object);

	void update(uint frameID, double delta = 0.0)override {};
	void endUpdate() override {};
private:
    
};

#endif