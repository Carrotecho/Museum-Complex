//
//  ListNode.hpp
//  Test
//
//  Created by Easy Proger on 01.12.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#ifndef ListNode_hpp
#define ListNode_hpp

#include <stdio.h>

#include "DynamicEntity.h"
#include "pugixml.hpp"
#include "InteractiveObject.hpp"

#include "SoundSystem.h"

class AnimationListNode {
public:
    float startFrame;
    float endFrame;
    float currentFrame;
    float delta;
    uint direction;
    
    DynamicEntity*node;
    bool ended;
};


class AnimationListNodeParams {
public:
    float startFrame;
    float centerFrame;
    float endFrame;
    float delta;
    
    bool rotateX;
    bool rotateY;
    bool rotateZ;
    uint rotateXn;
    uint rotateYn;
    uint rotateZn;

    bool setInitialScale;
    float initialScale;
    
    float minZoom;
    float maxZoom;
    
    float rotateDelta;
    
    bool zoom;
    float zoomDelta;
    
    float deltaMoveXInZoom;
    
    DynamicEntity*node;
};


class ListNode:public InteractiveObject {
public:
    ListNode(std::map<std::string,SkinningFrame*>*_skinAnimatorCache,SoundSystem*_soundSystem);
    ~ListNode();
    
    void ceraUpdate();
    bool noList;
    bool isCera;
    void setCera(int ceralistSoundID);
    void setNoList();
    
    void translateToChildSwipeEvent(bool val);
    
    void addNodeToList(DynamicEntity*node,
                       float startFrame,
                       float centerFrame,
                       float endFrame,
                       float delta,
                       bool rotateX,
                       bool rotateY,
                       bool rotateZ,
                       uint rotateXn,
                       uint rotateYn,
                       uint rotateZn,
                       float rotateDelta,
                       bool zoom,
                       float zoomDelta,
                       bool setInitialScale,
                       float initialScale,
                       bool setMinZoom,
                       float minZoom,
                       bool setMaxZoom,
                       float maxZoom,
                       bool setInitRX,
                       float initRX,
                       bool setInitRY,
                       float initRY,
                       bool setInitRZ,
                       float initRZ,
                       bool setDeltaMoveXInZoom,
                       float deltaMoveXInZoom);
    
    
    void prevNode();
    void nextNode();
    
    
    void ceraRight();
    void ceraLeft();
    
    uint ceraPageNum;
    
    int currentNodeID;
    
    float offsetTouchesX,offsetTouchesY;
    float touchesXStart,touchesYStart;
    
    void rotateNode(AnimationListNodeParams params,float offx,float offy);
    void zoomNode(AnimationListNodeParams params,float value);
    virtual void zoom(float value) override;
    virtual void touchesBegan(float x,float y) override;
    virtual void swipeEvent(uint direction) override;
    virtual void touchesMoved(float x,float y) override;
    virtual void update(uint frameID, double delta) override;
    virtual int getInteractiveIndex() override;
    
    
    void getChangedValue(float startValue,float endValue,float *inputVal,float delta,uint * retVal);
private:
    bool isTranslateToChildSwipeEvent;
    SoundSystem*soundSystem;
    int _ceralistSoundID;
    std::vector<AnimationListNodeParams> collectionNodes;
    
    
    std::vector<AnimationListNode> animations;
    AnimationListNode ceraAnimation;
};

#endif /* ListNode_hpp */
