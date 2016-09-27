//
//  DynamicEntity.h
//
//  Created by easy proger on 01.09.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef ____DynamicEntity__
#define ____DynamicEntity__

#include <iostream>
#include <map>

#include "SceneNode.h"
#include "XAnimation.h"
#include <string.h>
#include "GLES20ExtAndroid.h"

class DynamicEntity:public SceneNode {
public:
    DynamicEntity(std::map<std::string,SkinningFrame*>*skinAnimatorCache):SceneNode() {
        skinCacheCollections = skinAnimatorCache;
		zeroPos = XVector3(0, 0, 0);
        calculatedSkinFrame = NULL;
        haveAnimation = false;
        lastFrameID = 0;
        waitEndUpdate = false;
        GLSyncExtEnabled = true;
#ifdef __ANDROID__
        
        glMapBufferRangeEXT         = NULL;
        glFlushMappedBufferRangeEXT = NULL;
        glClientWaitSyncAPPLE       = NULL;
        glFenceSyncAPPLE            = NULL;
        
        initialiseFunctionsGL20ExtFunctions();
        
        if (glMapBufferRangeEXT         != NULL &&
            glFlushMappedBufferRangeEXT != NULL &&
            glClientWaitSyncAPPLE       != NULL &&
            glFenceSyncAPPLE            != NULL) {
        }else {
            GLSyncExtEnabled = false;
        }
#endif
        
    };
    virtual ~DynamicEntity(){
        
        
        for (unsigned int i = 0; i < skinningAnimations.size(); i++) {
            free(skinningAnimations[i]);
        }
        for (unsigned int i = 0; i < frameAnimations.size(); i++) {
            free(frameAnimations[i]);
        }
        for (unsigned int i = 0; i < morphAnimations.size(); i++) {
            free(morphAnimations[i]);
        }
        
        skinningAnimations.clear();
        morphAnimations.clear();
        frameAnimations.clear();
        printf("DynamicEntity delete\n");
    };
    
    void updateSkinAnimation(int frameID);
    void updateFrameAnimation(int frameID);
    void updateMorph(int frameID);
    
	virtual void calculateTransformMatrix() override;
    void update(uint frameID, double delta = 0.0) override;
	void endUpdate() override;
    virtual void swipeEvent(uint direction) override{
        
    };
	
    virtual void zoom(float value) override {
        
    };
    virtual void touchesBegan(float x,float y) override {
        
    };
    
    virtual void touchesMoved(float x,float y) override{
        
    };
    
    
    
	void setAnimation(XAnimation*anim);
    bool haveAnimation;
    


	virtual XVector3* getManipulatorPosition() override;
    virtual SkinningFrame* getSkinAnimation(bool update) override; // for shader !
private:
    bool GLSyncExtEnabled;
	XMatrix4 frameMatrix;
    SkinningFrame*calculatedSkinFrame;
    GLsync fence;

    std::map<std::string,SkinningFrame*> *skinCacheCollections;


    char* itoa(int val, int base);
    std::vector<SkinAnimationInfo *> skinningAnimations;
	std::vector<FrameAnimationInfo *> frameAnimations;
    std::vector<MorphAnimationInfo *> morphAnimations;


	XVector3 zeroPos;
};



#endif /* defined(____DynamicEntity__) */
