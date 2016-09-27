//
//  BookNode.hpp
//  UltraVision
//
//  Created by Easy Proger on 01.10.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#ifndef BookNode_hpp
#define BookNode_hpp

#include "DynamicEntity.h"
#include "InteractiveObject.hpp"
#include "SoundSystem.h"
#include <stdio.h>


class AnimationBook {
public:
    uint startFrameBook;
    uint startFrameActivePage;
    
    uint endFrameBook;
    uint endFrameActivePage;
    
    uint currentFrameBook;
    uint currentFrameActivePage;
    
    uint direction;
};



class BookNode:public InteractiveObject {
public:
    BookNode(Mesh*_pagesMesh,std::map<std::string,SkinningFrame*>*_skinAnimatorCache,std::vector<std::string> _pagetexturesPaths,std::string _globalPath,XAnimation*pagesAnim,
             float bookOpenS,
             float bookOpenE,
             float pageS,
             float pageE,
             float bookCloseS,
             float bookCloseE,
             SoundSystem*soundSystem,
             int _listSoundID,
             int _endSoundID,
             int _openSoundID);
    virtual ~ BookNode();
    
    
    void update(uint frameID, double delta = 0.0) override;
    
    
private:
    int openSoundID;
    int listSoundID;
    int endSoundID;

    void getChangedValue(uint startValue,uint endValue,uint *inputVal,uint * retVal);
    DynamicEntity* pagesoneNode;
    DynamicEntity* pagestwoNode;
    DynamicEntity* pagesfreeNode;
    
    float _bookOpenS;
    float _bookOpenE;
    float _pageS;
    float _pageE;
    float _bookCloseS;
    float _bookCloseE;
    
    DynamicEntity* bookNode;
    
    Material* getMatForpages(uint indexPage);
    std::map<std::string,SkinningFrame*>*skinAnimatorCache;
    
    std::vector<std::string> pagetexturesPaths;
    std::string globalPath;
    
    Texture* getTexture(int indexPage);
    virtual void swipeEvent(uint direction) override;
    
    
    virtual void zoom(float value) override {
        
    };
    
    virtual void touchesBegan(float x,float y) override {
        
    };
    
    virtual void touchesMoved(float x,float y) override{
        
    };
    
    
    virtual int getInteractiveIndex() override {
        return -1;
    };
    
    void swipeRight();
    void swipeLeft();
    int positionBook;
    std::map<std::string,Texture*> textureCache;
    
    
    AnimationBook*currentAnimation;
    std::vector<AnimationBook> animationsCollection;
    SoundSystem*_soundSystem;
};





#endif /* BookNode_hpp */
