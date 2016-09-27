//
//  InteractiveObject.hpp
//  Novgorod Museum Complex
//
//  Created by Easy Proger on 09.12.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#ifndef InteractiveObject_hpp
#define InteractiveObject_hpp

#include <stdio.h>

#include "DynamicEntity.h"


class InteractiveObject : public DynamicEntity {
public:
    InteractiveObject(std::map<std::string,SkinningFrame*>*_skinAnimatorCache):DynamicEntity(_skinAnimatorCache){};
    ~InteractiveObject(){};
    
    virtual void zoom(float value) override = 0;
    virtual void touchesBegan(float x,float y) override = 0;
    virtual void swipeEvent(uint direction) override = 0;
    virtual void touchesMoved(float x,float y) override = 0;
    virtual void update(uint frameID, double delta) override = 0;
    
    
    virtual int getInteractiveIndex() = 0;
};


#endif /* InteractiveObject_hpp */
