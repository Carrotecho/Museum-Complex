//
//  BookNode.cpp
//  UltraVision
//
//  Created by Easy Proger on 01.10.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#include "BookNode.hpp"

#include "ResourceManager.h"
#include "XTECH-C-Interface.h"

BookNode::BookNode(
                   Mesh*_pagesMesh,
                   std::map<std::string,
                   SkinningFrame*>*_skinAnimatorCache,
                   std::vector<std::string> _pagetexturesPaths,
                   std::string _globalPath,
                   XAnimation*pagesAnim,
                   float bookOpenS,
                   float bookOpenE,
                   float pageS,
                   float pageE,
                   float bookCloseS,
                   float bookCloseE,
                   SoundSystem*soundSystem,
                   int _listSoundID,
                   int _endSoundID,
                   int _openSoundID):InteractiveObject(_skinAnimatorCache){
    
    _soundSystem  = soundSystem;
    
    openSoundID = _openSoundID;
    listSoundID = _listSoundID;
    endSoundID  = _endSoundID;
    
    
    pagesoneNode  = new DynamicEntity(_skinAnimatorCache);
    pagestwoNode  = new DynamicEntity(_skinAnimatorCache);
    pagesfreeNode = new DynamicEntity(_skinAnimatorCache);
    
    
    pagesoneNode->needValidateMaterial = false;
    pagestwoNode->needValidateMaterial = false;
    pagesfreeNode->needValidateMaterial = false;
    
    
    if (pagesAnim !=nullptr) {
        pagesoneNode->setAnimation(pagesAnim);
        pagestwoNode->setAnimation(pagesAnim);
        pagesfreeNode->setAnimation(pagesAnim);
        
        pagesoneNode->update(0);
        pagestwoNode->update(0);
        pagesfreeNode->update(0);
        
    }
    
    pagetexturesPaths = _pagetexturesPaths;
    
    
    
    _bookOpenS  = bookOpenS;
    _bookOpenE  = bookOpenE;
    _pageS      = pageS;
    _pageE      = pageE;
    _bookCloseS = bookCloseS;
    _bookCloseE = bookCloseE;
    
    
    
    
    animationsCollection.resize(pagetexturesPaths.size()+2);
    
    
    animationsCollection[0].startFrameBook       = bookOpenS;
    animationsCollection[0].endFrameBook         = bookOpenE;
    
    animationsCollection[0].startFrameActivePage = bookOpenS;
    animationsCollection[0].endFrameActivePage   = bookOpenE;
    
    for (uint i = 0; i < pagetexturesPaths.size(); i++) {
        animationsCollection[i+1].startFrameBook       = bookOpenE;
        animationsCollection[i+1].endFrameBook         = bookOpenE;
        
        animationsCollection[i+1].startFrameActivePage = pageS;
        animationsCollection[i+1].endFrameActivePage   = pageE;
    }
    
    uint lastId = pagetexturesPaths.size()+1;
    
    animationsCollection[lastId].startFrameBook       = bookCloseS;
    animationsCollection[lastId].endFrameBook         = bookCloseE;
    
    animationsCollection[lastId].startFrameActivePage = bookCloseS;
    animationsCollection[lastId].endFrameActivePage   = bookCloseE;
    
    
    
    globalPath = _globalPath;
    
    pagesoneNode->setMaterial(getMatForpages(0));
    pagestwoNode->setMaterial(getMatForpages(1));
    pagesfreeNode->setMaterial(getMatForpages(0));

    pagesoneNode->setMesh(_pagesMesh);
    pagestwoNode->setMesh(_pagesMesh);
    pagesfreeNode->setMesh(_pagesMesh);
    
    skinAnimatorCache = _skinAnimatorCache;
    
    positionBook = 0;
    
    currentAnimation = nullptr;
    
    this->addChild(pagesoneNode);
    this->addChild(pagestwoNode);
    this->addChild(pagesfreeNode);
}




void BookNode::getChangedValue(uint startValue,uint endValue,uint *inputVal,uint * retVal) {
    if (startValue < endValue) {
        // +
        if (*inputVal>=endValue) {
            *retVal=1;
            return;
        }
        
        *inputVal+=1;
    }else {
        // -
        
        if (*inputVal<=endValue) {
            *retVal=1;
            return;
        }
        
        *inputVal-=1;
    }
    
    *retVal=0;
}

void BookNode::swipeEvent(uint direction) {
    
    if (direction == 2) {
        swipeLeft();
    }else if (direction == 1) {
        swipeRight();
    }
    
    
}

void BookNode::swipeRight() {
    
    if (positionBook<=0) return;
    
    if (currentAnimation != nullptr) return;
    
    if (textureCache.size()>3) {
        for(auto const &ent1 : textureCache) {
            Texture*tex = ent1.second;
            tex->deleteTexture();
            delete tex;
        }
        textureCache.clear();
    }
    
    pagesoneNode->isVisible  = true;
    pagestwoNode->isVisible  = true;
    pagesfreeNode->isVisible = true;
    
    
    
    if (positionBook == 1) {
        pagesoneNode->update(_bookOpenS);
        pagestwoNode->update(_bookOpenS);
        pagesfreeNode->update(_bookOpenS);
        
        pagestwoNode->isVisible = false;
        pagesfreeNode->isVisible = false;
    }else if (positionBook == 2) {
        pagesoneNode->update(_pageE);
        pagestwoNode->update(_pageS);
        pagesfreeNode->update(_pageS);
    }else if (positionBook-1 == pagetexturesPaths.size()){
        pagesoneNode->update(_pageE);
        pagesfreeNode->update(_pageE);
        pagestwoNode->isVisible = false;
    }else if (positionBook-2 == pagetexturesPaths.size()){
        pagesoneNode->update(_bookCloseE);
        pagesfreeNode->update(_bookCloseE);
        pagestwoNode->isVisible = false;
    }else if (positionBook > 2) {
        pagesoneNode->update(_pageE);
        pagestwoNode->update(_pageS);
        pagesfreeNode->update(_pageE);
    }
    
    
    
    
    positionBook--;
    
    
    pagesoneNode->getMaterial()->diffuseTexture  = getTexture(positionBook-1);
    pagestwoNode->getMaterial()->diffuseTexture  = getTexture(positionBook);
    pagesfreeNode->getMaterial()->diffuseTexture = getTexture(positionBook-2);
    
    AnimationBook* animation = &animationsCollection[positionBook];
    
    animation->currentFrameBook       = animation->endFrameBook;
    animation->currentFrameActivePage = animation->endFrameActivePage;
    animation->direction = 2;
    
    currentAnimation = animation;
    
    
    if (positionBook == 0) {
        _soundSystem->playSound(endSoundID,false);
    }else {
        _soundSystem->playSound(listSoundID,false);
    }
    
    
}

void BookNode::swipeLeft() {
    if (positionBook >= animationsCollection.size()) return;
    
    if (currentAnimation != nullptr) return;
    
    AnimationBook* animation = &animationsCollection[positionBook];
    
    animation->currentFrameBook       = animation->startFrameBook;
    animation->currentFrameActivePage = animation->startFrameActivePage;
    animation->direction = 1;
    
    //printf("positionBook %d\n",positionBook);
    
    
    if (textureCache.size()>3) {
        for(auto const &ent1 : textureCache) {
            Texture*tex = ent1.second;
            tex->deleteTexture();
            delete tex;
        }
        textureCache.clear();
    }
    
    pagesoneNode->isVisible  = true;
    pagestwoNode->isVisible  = true;
    pagesfreeNode->isVisible = true;
    
    
    if (positionBook == 0) {
        pagesoneNode->update(_bookOpenS);
        pagestwoNode->update(_bookOpenS);
        pagesfreeNode->update(_bookOpenS);
        
        pagestwoNode->isVisible  = false;
        pagesfreeNode->isVisible  = false;
    }else if (positionBook == 1) {
        pagesoneNode->update(_pageS);
        pagestwoNode->update(_pageS);
        pagesfreeNode->update(_pageS);
        pagesfreeNode->isVisible  = false;
    }else if (positionBook == pagetexturesPaths.size()) {
        pagesoneNode->update(_pageS);
        pagestwoNode->update(_pageE);
        pagesfreeNode->update(_pageE);
        pagestwoNode->isVisible = false;
    }else if (positionBook-1 == pagetexturesPaths.size()) {
        printf("end");
        pagesoneNode->update(_pageE);
        pagestwoNode->isVisible = false;
        pagesfreeNode->isVisible = false;
    }else if (positionBook > 1) {
        pagesoneNode->update(_pageS);
        pagestwoNode->update(_pageS);
        pagesfreeNode->update(_pageE);
    }
    
    pagesoneNode->getMaterial()->diffuseTexture  = getTexture(positionBook-1);
    pagestwoNode->getMaterial()->diffuseTexture  = getTexture(positionBook);
    pagesfreeNode->getMaterial()->diffuseTexture = getTexture(positionBook-2);
    
    
    currentAnimation = animation;
    
    positionBook++;
    
    
    if (positionBook-2 == pagetexturesPaths.size()) {
        _soundSystem->playSound(endSoundID,false);
    }else {
        _soundSystem->playSound(listSoundID,false);
    }
    
    
    
    // if cicled book animation
    //if (positionBook-2 == pagetexturesPaths.size()) {
    //    positionBook = 0;
    //}
}



void BookNode::update(uint frameID, double delta) {

    
    if (currentAnimation == nullptr) return;
    
    uint retValue = 0;
    uint retValuePage = 0;
    if (currentAnimation->direction == 1) {
        getChangedValue(currentAnimation->startFrameBook,currentAnimation->endFrameBook,&currentAnimation->currentFrameBook,&retValue);
        getChangedValue(currentAnimation->startFrameActivePage,currentAnimation->endFrameActivePage,&currentAnimation->currentFrameActivePage,&retValuePage);
    }else if (currentAnimation->direction == 2) {
        getChangedValue(currentAnimation->endFrameBook,currentAnimation->startFrameBook,&currentAnimation->currentFrameBook,&retValue);
        getChangedValue(currentAnimation->endFrameActivePage,currentAnimation->startFrameActivePage,&currentAnimation->currentFrameActivePage,&retValuePage);
    }
    
    DynamicEntity::update(currentAnimation->currentFrameBook, 1);
    pagesoneNode->update(currentAnimation->currentFrameActivePage);
    
    if (retValue && retValuePage) {
        currentAnimation = nullptr; // end
    }
}



Texture* BookNode::getTexture(int indexPage) {
    Texture* tex = nullptr;
    if (indexPage < 0 ) {
        indexPage = 0;
    }
    if (indexPage >= pagetexturesPaths.size()) {
        indexPage = pagetexturesPaths.size()-1;
    }
    if (pagetexturesPaths.size() > indexPage) {
        
        std::string pathForTexture = globalPath+pagetexturesPaths[indexPage];
        
        if (textureCache.find(pathForTexture) != textureCache.end()) {
            tex = textureCache[pathForTexture];
            return tex;
        }else {
            tex = new Texture();
            tex->createTexture(pathForTexture);
            textureCache[pathForTexture] = tex;
            return tex;
        }
    }
    
    return nullptr;
}


Material* BookNode::getMatForpages(uint indexPage) {
    Material* mat = new Material();
    
    
    mat->diffuseTexture = getTexture(indexPage);
    mat->setFlag(DYNAMIC_PER_VERTEX_BASE_LIGHT, true);
    
    if (mat->diffuseTexture  != nullptr) {
        mat->setFlag(NORMAL_MATERIAL, true);
    }else {
        mat->setFlag(BASE_MATERIAL_COLOR, true);
    }
    
    return mat;
}


BookNode::~BookNode() {
    XLOG("~BookNode dealoc");
    for(auto const &ent1 : textureCache) {
        Texture*tex = ent1.second;
        tex->deleteTexture();
        delete tex;
    }
    textureCache.clear();
    
    delete pagesoneNode->getMaterial();
    delete pagestwoNode->getMaterial();
    delete pagesfreeNode->getMaterial();
    
}










