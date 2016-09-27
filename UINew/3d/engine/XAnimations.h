//
//  XAnimations.h
//  
//
//  Created by easy proger on 02.09.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef ____XAnimations__
#define ____XAnimations__

#include <iostream>
#include "XAnimation.h"
#include <map>


class XAnimations {
public:
    XAnimations(std::map<std::string,SkinningFrame*>*skinAnimatorCache){
		_skinCache = skinAnimatorCache;
        reloading = false;
    };
    ~XAnimations(){
        clear();
    };
    
    std::vector<XAnimation*> animationsRequest;
    std::vector<XAnimation*> animationsLoading;
    
    void clear();
    void correctCache();
    void reload();

    XAnimation* getAnimation(const char*pathForAnim);
    XAnimation* getAnimation(uint id);

	int getAnimationID(const char * pathForAnim);

	
    int addAnimation(const char*cacheString,XAnimation*anim) ;
    bool reloading;
private:
    std::map<std::string,SkinningFrame*>*_skinCache;
    std::map<std::string, int> cacheIDS;
    std::vector<XAnimation*> animationsCache;
    
};


#endif /* defined(____XAnimations__) */
