//
//  ListNode.cpp
//  Test
//
//  Created by Easy Proger on 01.12.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#include "ListNode.hpp"

ListNode::ListNode(std::map<std::string,SkinningFrame*>*_skinAnimatorCache,SoundSystem*_soundSystem):InteractiveObject(_skinAnimatorCache) {
    
    soundSystem = _soundSystem;
    offsetTouchesX = 0;
    offsetTouchesY = 0;
    
    currentNodeID = 0;
    isTranslateToChildSwipeEvent = false;
    
    isCera = false;
    noList = false;
    
    
};



ListNode::~ListNode() {
  
}

int ListNode::getInteractiveIndex() {
    return currentNodeID;
}

void ListNode::getChangedValue(float startValue,float endValue,float *inputVal,float delta,uint * retVal) {
    if (startValue < endValue) {
        // +
        if (*inputVal>=endValue) {
            *retVal=1;
            return;
        }
        
        *inputVal+=delta;
    }else {
        // -
        
        if (*inputVal<=endValue) {
            *retVal=1;
            return;
        }
        
        *inputVal-=delta;
    }
    
    *retVal=0;
}


void ListNode::ceraUpdate() {
    
    if (ceraAnimation.ended) {
        return;
    }
    
    uint retValue = 0;
    if (ceraAnimation.direction == 1) {
        getChangedValue(ceraAnimation.startFrame,ceraAnimation.endFrame,&ceraAnimation.currentFrame,ceraAnimation.delta,&retValue);
    }else if (ceraAnimation.direction == 2) {
        getChangedValue(ceraAnimation.endFrame,ceraAnimation.startFrame,&ceraAnimation.currentFrame,ceraAnimation.delta,&retValue);
    }
    
    for (int i = 0; i < childs.size(); i++) {
        DynamicEntity*obj = (DynamicEntity*)childs[i];
        if (obj) {
            obj->update(ceraAnimation.currentFrame);
        }
    }
    if (retValue) {
        ceraAnimation.ended = true;
    }
}


void ListNode::ceraLeft() {
    if (!ceraAnimation.ended) {
        return;
    }
   
    if (ceraPageNum >= 4) {
        return;
    }
    
    ceraAnimation.ended = false;
    
    if (ceraPageNum == 0) {
        ceraAnimation.direction = 1;
        ceraAnimation.currentFrame = 0;
        ceraAnimation.startFrame = 0;
        ceraAnimation.endFrame = 0;
        ceraAnimation.delta = 1;
    }else if (ceraPageNum == 1) {
        ceraAnimation.direction = 1;
        ceraAnimation.currentFrame = 0;
        ceraAnimation.startFrame = 0;
        ceraAnimation.endFrame = 30;
        ceraAnimation.delta = 1;
        
    }else if (ceraPageNum == 2) {
        ceraAnimation.direction = 1;
        ceraAnimation.currentFrame = 30;
        ceraAnimation.startFrame = 30;
        ceraAnimation.endFrame = 60;
        ceraAnimation.delta = 1;
        
    }else if (ceraPageNum == 3) {
        ceraAnimation.direction = 1;
        ceraAnimation.currentFrame = 60;
        ceraAnimation.startFrame = 60;
        ceraAnimation.endFrame = 90;
        ceraAnimation.delta = 1;
        
    }
    ceraPageNum++;
    soundSystem->playSound(_ceralistSoundID,false);
}
void ListNode::ceraRight() {
    if (!ceraAnimation.ended) {
        return;
    }
    
    if (ceraPageNum <= 1) {
        return;
    }
    ceraPageNum--;
    
    ceraAnimation.ended = false;
    
    if (ceraPageNum == 0) {
        ceraAnimation.direction = 1;
        ceraAnimation.currentFrame = 0;
        ceraAnimation.startFrame = 0;
        ceraAnimation.endFrame = 0;
        ceraAnimation.delta = 1;
    }else if (ceraPageNum == 1) {
        ceraAnimation.direction = 1;
        ceraAnimation.currentFrame = 30;
        ceraAnimation.startFrame = 30;
        ceraAnimation.endFrame = 0;
        ceraAnimation.delta = 1;
        
    }else if (ceraPageNum == 2) {
        ceraAnimation.direction = 1;
        ceraAnimation.currentFrame = 60;
        ceraAnimation.startFrame = 60;
        ceraAnimation.endFrame = 30;
        ceraAnimation.delta = 1;
        
    }else if (ceraPageNum == 3) {
        ceraAnimation.direction = 1;
        ceraAnimation.currentFrame = 90;
        ceraAnimation.startFrame = 90;
        ceraAnimation.endFrame = 60;
        ceraAnimation.delta = 1;
        
    }
    soundSystem->playSound(_ceralistSoundID,false);
    
}

void ListNode::update(uint frameID, double delta) {

    
    
    
    
    if (isCera) {
        ceraUpdate();
        return;
    }
    
    for (int i = 0; i < childs.size(); i++) {
        DynamicEntity*obj = (DynamicEntity*)childs[i];
        if (obj) {
            int frameID =  obj->lastFrameID;
            frameID++;
            obj->update(frameID);
            
            //XVector3 position = obj->getPosition();
            //position.x+=1.1;
            //obj->setTranslate(&position);
            //1400 1830
        }
    }
    
    this->isTransformMatrixDerty = true;
    this->isNormalMatrixDerty = true;
    this->calculateTransformMatrix();
    
    if (!animations.size()) return;
    
    
    for (std::vector<AnimationListNode>::iterator it=animations.begin();it!=animations.end();) {
        
        uint retValue = 0;
        if (it->direction == 1) {
            getChangedValue(it->startFrame,it->endFrame,&it->currentFrame,it->delta,&retValue);
        }else if (it->direction == 2) {
            getChangedValue(it->endFrame,it->startFrame,&it->currentFrame,it->delta,&retValue);
        }
  
        
        
        it->node->setX(it->currentFrame);
        
        if(retValue) {
            it = animations.erase(it);
        }else {
            ++it;
        }
    }
    
    
}


void ListNode::addNodeToList(DynamicEntity*node,
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
                             float deltaMoveXInZoom) {
    
    AnimationListNodeParams params;
    params.node            = node;
    params.startFrame      = startFrame;
    params.endFrame        = endFrame;
    params.centerFrame     = centerFrame;
    params.delta           = delta;
    params.rotateX         = rotateX;
    params.rotateY         = rotateY;
    params.rotateZ         = rotateZ;
    params.rotateXn        = rotateXn;
    params.rotateYn        = rotateYn;
    params.rotateZn        = rotateZn;
    params.setInitialScale = setInitialScale;
    params.initialScale    = initialScale;
    params.rotateDelta     = rotateDelta;
    params.zoom            = zoom;
    params.zoomDelta       = zoomDelta;
    
    params.minZoom         = setMinZoom ? minZoom : 0.5;
    params.maxZoom         = setMaxZoom ? maxZoom : 2.5;
    
    params.deltaMoveXInZoom = setDeltaMoveXInZoom ? deltaMoveXInZoom : 0.0;
    
    collectionNodes.push_back(params);
    
    if (!noList) {
        node->setVisible(false);
    }
    
    
    
    node->resetTransformMatrix();
    node->setX(startFrame); // initial position;
    
    if (params.initialScale) {
        XVector3 initialScaleVector(params.initialScale,params.initialScale,params.initialScale);
        node->getMesh()->initScale = initialScaleVector;
        node->setScale(&initialScaleVector);
    }
    
    XVector3 rotation = node->getRotationDegress();
    if (setInitRX) {
        rotation.x = initRX;
    }
    
    if (setInitRY) {
        rotation.y = initRY;
    }
    
    if (setInitRZ) {
        rotation.z = initRZ;
    }
    
    node->setRotationEulerinDegress(&rotation);
    
    rotation*=DEG_TO_RAD;
    
    XVector4 rotateQuat;
    XQuaternionFromEuler(&rotateQuat, &rotation);
    node->getMesh()->initQuaternion = rotateQuat;
    
    
    
    
    this->addChild(node);
}

void ListNode::translateToChildSwipeEvent(bool val) {
    isTranslateToChildSwipeEvent = val;
}

void ListNode::setNoList() {
    noList = true;
    currentNodeID = 1;
}

void ListNode::setCera(int ceralistSoundID) {
    isCera = true;
    ceraPageNum = 0;
    ceraAnimation.ended = true;
    _ceralistSoundID = ceralistSoundID;
}

void ListNode::zoomNode(AnimationListNodeParams params,float value) {
    if (!params.zoom) return;
    
    DynamicEntity*node = params.node;
    XVector3 scale = node->getScale();
    
    scale.x+=value * params.zoomDelta;
    scale.y+=value * params.zoomDelta;
    scale.z+=value * params.zoomDelta;
    
    if (scale.x < params.minZoom) scale.x = params.minZoom;
    if (scale.y < params.minZoom) scale.y = params.minZoom;
    if (scale.z < params.minZoom) scale.z = params.minZoom;
    
    if (scale.x > params.maxZoom) scale.x = params.maxZoom;
    if (scale.y > params.maxZoom) scale.y = params.maxZoom;
    if (scale.z > params.maxZoom) scale.z = params.maxZoom;
    
    if (params.deltaMoveXInZoom) {
        XVector3 position = node->getMesh()->initTranslation;
        position.x+=params.deltaMoveXInZoom*(scale.x-1.0);
        node->setTranslate(&position);
    }
    
    
    node->setScale(&scale);
}

void ListNode::zoom(float value) {
    if (animations.size()) return;
    if (!collectionNodes.size()) return;
    if (currentNodeID-1 < 0 || currentNodeID-1 >= collectionNodes.size()) return;
    
    
    if (noList) {
        for (uint i = 0; i < collectionNodes.size(); i++) {
            AnimationListNodeParams params = collectionNodes[i]; // get current
            zoomNode(params,value);
        }
    }else {
        AnimationListNodeParams params = collectionNodes[currentNodeID-1];
        zoomNode(params,value);
    }
    
    
    
    
    
}

void ListNode::touchesBegan(float x,float y) {
    touchesXStart = x;
    touchesYStart = y;
    
    
}

void ListNode::rotateNode(AnimationListNodeParams params,float offx,float offy) {
    DynamicEntity*node = params.node;
    XVector3 currentRotation = node->getRotationDegress();
    
    if (params.rotateX) currentRotation.x = params.rotateXn == 0 ? offx : offy;
    if (params.rotateY) currentRotation.y = params.rotateYn == 0 ? offx : offy;
    if (params.rotateZ) currentRotation.z = params.rotateZn == 0 ? offx : offy;
    
    node->setRotationEulerinDegress(&currentRotation);
    
}

void ListNode::touchesMoved(float x,float y) {
    
    
    if (animations.size()) return;
    if (!collectionNodes.size()) return;
    if (currentNodeID-1 < 0 || currentNodeID-1 >= collectionNodes.size()) return;
    
    
    if (noList) {
        for (uint i = 0; i < collectionNodes.size(); i++) {
            AnimationListNodeParams params = collectionNodes[i]; // get current
            
            
            offsetTouchesX += (x-touchesXStart)*params.rotateDelta;
            touchesXStart = x;
            
            offsetTouchesY += (y-touchesYStart)*params.rotateDelta;
            touchesYStart = y;
            
            rotateNode(params,offsetTouchesX,offsetTouchesY);
        }
    }else {
        AnimationListNodeParams params = collectionNodes[currentNodeID-1]; // get current
        
        
        offsetTouchesX += (x-touchesXStart)*params.rotateDelta;
        touchesXStart = x;
        
        offsetTouchesY += (y-touchesYStart)*params.rotateDelta;
        touchesYStart = y;
        
        rotateNode(params,offsetTouchesX,offsetTouchesY);
    }
    
    
    
}

void ListNode::swipeEvent(uint direction) {
    
    if (isTranslateToChildSwipeEvent) {
        for (int i = 0; i < childs.size(); i++) {
            SceneNode*node = childs[i];
            node->swipeEvent(direction);
        }
    }
    
    if (direction == 2) {
        nextNode();
    }else if (direction == 1) {
        prevNode();
    }
}

void ListNode::nextNode() {
    
    if (isCera) {
        ceraLeft();
    }
    
    if (noList) return;
    if (animations.size()) return;
    if (!collectionNodes.size()) return;
    
    if (currentNodeID < 0 || currentNodeID >= collectionNodes.size()) return;
    
    for (int i = 0; i < collectionNodes.size(); i++) {
        collectionNodes[i].node->setVisible(false); // hide all
    }
    
    if (currentNodeID>0) {
        AnimationListNodeParams params = collectionNodes[currentNodeID-1];
        DynamicEntity*nodePrevToAnimate = params.node;
        nodePrevToAnimate->setVisible(true); // show prev
        AnimationListNode animation;
        
        animation.currentFrame = params.centerFrame;
        animation.startFrame   = params.centerFrame;
        animation.endFrame     = params.endFrame;
        animation.delta        = params.delta;
        animation.direction    = 1;
        animation.node         = nodePrevToAnimate;
        
        nodePrevToAnimate->setScale(&nodePrevToAnimate->getMesh()->initScale);
        nodePrevToAnimate->setQuaternion(&nodePrevToAnimate->getMesh()->initQuaternion);
        
        animations.push_back(animation);
    }
    AnimationListNodeParams params = collectionNodes[currentNodeID];
    
    DynamicEntity*nodeToAnimate = params.node;
    nodeToAnimate->setVisible(true); // show new
    AnimationListNode animation;
    
    animation.currentFrame = params.startFrame;
    animation.startFrame   = params.startFrame;
    animation.endFrame     = params.centerFrame;
    animation.delta        = params.delta;
    animation.direction    = 1;
    animation.node         = nodeToAnimate;
    
    nodeToAnimate->setScale(&nodeToAnimate->getMesh()->initScale);
    nodeToAnimate->setQuaternion(&nodeToAnimate->getMesh()->initQuaternion);
    
    animations.push_back(animation);

    currentNodeID++;
    
}

void ListNode::prevNode() {
    if (isCera) {
        ceraRight();
        return;
    }
    
    if (noList) return;
    if (animations.size()) return;
    if (!collectionNodes.size()) return;
    
    if (currentNodeID-2 < 0 || currentNodeID-1 >= collectionNodes.size()) return;
    
    currentNodeID--;
    
    
    
    for (int i = 0; i < collectionNodes.size(); i++) {
        collectionNodes[i].node->setVisible(false); // hide all
    }
    
    
    
    AnimationListNodeParams paramsBack = collectionNodes[currentNodeID];
    DynamicEntity*nodePrevToAnimate = paramsBack.node;
    nodePrevToAnimate->setVisible(true); // show prev
    AnimationListNode animationBack;
    
    animationBack.currentFrame = paramsBack.centerFrame;
    animationBack.startFrame   = paramsBack.centerFrame;
    animationBack.endFrame     = paramsBack.startFrame;
    animationBack.delta        = paramsBack.delta;
    animationBack.direction    = 1;
    animationBack.node         = nodePrevToAnimate;
    
    animations.push_back(animationBack);
    
    nodePrevToAnimate->setScale(&nodePrevToAnimate->getMesh()->initScale);
    nodePrevToAnimate->setQuaternion(&nodePrevToAnimate->getMesh()->initQuaternion);
    
    
    AnimationListNodeParams params = collectionNodes[currentNodeID-1];
    
    DynamicEntity*nodeToAnimate = params.node;
    nodeToAnimate->setVisible(true); // show new
    AnimationListNode animation;
    
    animation.currentFrame = params.endFrame;
    animation.startFrame   = params.endFrame;
    animation.endFrame     = params.centerFrame;
    animation.delta        = params.delta;
    animation.direction    = 1;
    animation.node         = nodeToAnimate;
    
    nodeToAnimate->setScale(&nodeToAnimate->getMesh()->initScale);
    nodeToAnimate->setQuaternion(&nodeToAnimate->getMesh()->initQuaternion);
    
    
    animations.push_back(animation);
    
    
    
}

