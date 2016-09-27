//
//  XMathTypes.cpp
//  UltraVision
//
//  Created by Easy Proger on 17.08.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#include "XMathTypes.h"
#include <algorithm>
XPlane::XPlane(const XVector3 &n, const XVector3 &o) {
    normal = n;
    setOrigin(o);
}

//--------------------------------------------------------------------------

void XPlane::setOrigin(const XVector3 &o) {
    dist = -normal.dot(o);
}

//--------------------------------------------------------------------------

void XPlane::normalize() {
    float len = normal.length();
    
    if (fabsf(len) > EPSILON ) {
        normal /= len;
        dist /= len;
    }
}

//--------------------------------------------------------------------------

XPlane normalize(const XPlane &p) {
    float len = p.normal.length();
    
    if (fabsf(len) > EPSILON ) {
        float ilen = 1.0f / len;
        return XPlane(p.normal * ilen, p.dist * ilen);
    }
    return p;
}

//--------------------------------------------------------------------------

bool XPlane::trace(const XLine &l, XTracePoint &i, bool fixSrc, bool fixDst) const {
    
    float cosang = 0;
    float d = 0;
    float lamda = 0;
    
    XVector3 dir = l.getDirection();
    cosang = dir.dot(normal);
    
   // if (cosang > -EPSILON && cosang < EPSILON) return false;
    
    d = l.src.dot(normal);
    
    lamda = (-dist - d) / cosang;
   // if (lamda > 100.0) lamda = 100.0;
   // if (lamda < -100.0) lamda = -100.0;
    
    //if ((fixSrc && lamda < 0) || (fixDst && lamda > 1)) return false;
    
    i.point = l.src + dir * lamda;
    i.normal = normal;
    
    return true;
}
