//
//  XAnimTypes.h
//  
//
//  Created by easy proger on 01.09.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef _XAnimTypes_h
#define _XAnimTypes_h


#include "XMathTypes.h"


typedef struct{
    XVector3*deltas;
}MorphChannelInfo;

typedef struct{
    int numAffectedVertex;
    int numChannels;
    MorphChannelInfo*channels;
}MorphInfo;



typedef struct{
    int numChannels;
    float* weights;
}MorphAnimationFrame;

typedef struct{
	XMatrix4 mat;
}AnimationFrame;

typedef struct{
    XVector3 *pos;
    XVector4 *rot;
    int *parentID;
    XMatrix4*mat;
    void init() {
        pos = NULL;
        rot = NULL;
        parentID = NULL;
        mat = NULL;
    }
	void remove(){
        printf("SkinningFrame remove \n");
		if (pos)     free(pos);
    	if (rot)     free(rot);
    	if (parentID)free(parentID);
		if (mat)     free(mat);
	}
}SkinningFrame;


#endif
