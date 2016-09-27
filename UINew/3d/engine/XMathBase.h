//
//  XMathBase.h
//  
//
//  Created by easy proger on 02.09.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef ____XMathBase__
#define ____XMathBase__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "Vectors.h"
#include "XMathTypes.h"
#include "XTypes.h"

void itostr(int n, char* c);
void itostr(unsigned val, char* c);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

struct BoneInfo
{
    int boneID;
    int parentID;
    std::vector<BoneInfo*> childs;
};

typedef unsigned int uint;




AGRESIVE_INLINE vec3 normalize_(const vec3 &v);
AGRESIVE_INLINE float dot_(const vec3 &a, const vec3 &b);
AGRESIVE_INLINE vec3 cross_(const vec3 &a, const vec3 &b);
AGRESIVE_INLINE float arccos_(float x);

AGRESIVE_INLINE vec3 normalize_(const vec3 &v) {

		float len = v.length();
		if (fabsf(len) > EPSILON) {
			float inv = 1.0f / len;
			return vec3(v.x * inv, v.y * inv, v.z * inv);
		} else {
			return v;
		}
	}

	AGRESIVE_INLINE float dot_(const vec3 &a, const vec3 &b) {
		return
			a.x * b.x +
			a.y * b.y +
			a.z * b.z;
	}

	AGRESIVE_INLINE vec3 cross_(const vec3 &a, const vec3 &b) {

		return vec3(
			a.y * b.z - b.y * a.z,
			a.z * b.x - b.z * a.x,
			a.x * b.y - b.x * a.y
		);
	}
	AGRESIVE_INLINE float arccos_(float x) {
			double omxx = 1.0 - double(x) * double(x);
			if (omxx < 0.0) omxx = 0.0;
			return atan2f((float)sqrt(omxx), x);
		}
	



#endif /* defined(____XMathBase__) */
