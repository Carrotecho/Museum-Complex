//
//  XObject.h
//  XTech
//
//  Created by easy proger on 14.02.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef __XObject__
#define __XObject__

#include <iostream>


class XObject {
public:
    XObject* linkClass;
    XObject() {
        countUsed = 1;
        linkClass = NULL;
    };
    ~XObject() {};
    
    XObject* retain() {
        if(countUsed<=0) return NULL;
        countUsed++;
        return this;
    };
    void release(){
        if(countUsed<=0) return ;
        countUsed--;
        if(countUsed<=0) {
            delete linkClass;
        };
    };
protected:
    int countUsed;
};



#endif /* defined(__XObject__) */
