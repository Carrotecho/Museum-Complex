//
//  XAnimations.cpp
//  
//
//  Created by easy proger on 02.09.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#include "XAnimations.h"
#include "SkinningMath.h"

XAnimation* XAnimations::getAnimation(uint id) {
    if (animationsCache.size() > id ) {
        
        if (animationsCache[id]->isValid) {
            // here need request reload animation !
        }
        
        return animationsCache[id];
    }
    return nullptr;
}

int XAnimations::getAnimationID(const char*pathForAnim) {
	if (cacheIDS.find(pathForAnim) != cacheIDS.end()) {
		int indxInCache = cacheIDS[pathForAnim];
		return indxInCache;
	}
	return -1;
}

XAnimation* XAnimations::getAnimation(const char*pathForAnim) {
    if (cacheIDS.find(pathForAnim) != cacheIDS.end()) {
        int indxInCache = cacheIDS[pathForAnim];
        return animationsCache[indxInCache];
    }
    return nullptr;
}



void XAnimations::clear() {
    for (uint i = 0; i < animationsCache.size(); i++) {
        XAnimation*anim = animationsCache[i];
        if (anim) {
            anim->removeData();
        }
    }
    
    animationsCache.resize(0);
    cacheIDS.erase(cacheIDS.begin(),cacheIDS.end());


	std::map<std::string, SkinningFrame*>::iterator it=_skinCache->begin();
	while(it!=_skinCache->end()) {
		removeSkinningFrame(it->second);
        it = _skinCache->erase(it);
	}
}



void XAnimations::correctCache() {
    for (uint i = 0; i < animationsCache.size(); i++) {
        XAnimation*anim = animationsCache[i];
        if (anim) {
            anim->release();
        }
    }
}


int XAnimations::addAnimation(const char*cacheString,XAnimation*anim) {

    if (cacheIDS.find(cacheString) != cacheIDS.end()) {
        return cacheIDS[cacheString];
    }
    
    if (anim) {
        anim->idInCache = (uint)animationsCache.size();
        cacheIDS[cacheString] = anim->idInCache;
        animationsCache.push_back(anim);
		return animationsCache.size()-1;
    }
	return -1;
}
