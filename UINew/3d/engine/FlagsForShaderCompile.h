//
//  FlagsForShaderCompile.h
//  XTech
//
//  Created by easy proger on 20.02.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef XTech_FlagsForShaderCompile_h
#define XTech_FlagsForShaderCompile_h


#define NORMAL_MESH                  1   // 2^0, bit 0
#define SKINING_MESH                 2   // 2^1, bit 1


#define NORMAL_MATERIAL               1   // 2^2, bit 2
#define COLOR_MATERIAL                2   // 2^2, bit 2
#define DYNAMIC_PER_PIXEL_BASE_LIGHT  3   // 2^3, bit 3
#define RESAVE_SHADOW_SM              4  // 2^4, bit 4
#define SPECULAR_MATERIAL             5 // and so on...
#define NORMAL_MAP_MATERIAL           6 // and so on...
#define DYNAMIC_PER_VERTEX_BASE_LIGHT 7 // and so on...
#define TEXTURE_AND_COLOR             8 // and so on...
#define BASE_MATERIAL_COLOR           9 // and so on...

#define MATERIAL_COLOR_FROM_MAT       10 // and so on...
#define PARTICLE_MATERIAL             11 // and so on...
#define MATERIAL_PROJECT              12
#define MATERIAL_DISCARD_Y            13
#define MATERIAL_OBJECT_POSITION      14

#define MAX_MATERIALS 512


#endif
