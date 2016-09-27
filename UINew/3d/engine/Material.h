//
//  Material.h
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 14.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//
#ifndef MATERIAL_H_DEF
#define MATERIAL_H_DEF
#include <iostream>
#include "Types.h"
#include "FlagsForShaderCompile.h"
#include "Texture.h"
#include "XMathTypes.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <GL/glew.h>
#else
#ifdef __ANDROID__
#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif
#endif


class Material {
private:
public:
    void setSpecularPower(float val);
    float getSpecularPower();
    
    
	std::string matFlagsString;
	XVector3 color;

    Texture* diffuseTexture;
    Texture* normalMapTexture;
    Texture* specularTexture;
    
    Material(bool useBaseTexture = true);
	void setFlag(uint flagID, bool val);
	bool getFlagVal(uint flagID);
	void updateUDIDMaterial();
	~Material() {};
    uint shader_id;
    
    bool validate();
    void reloadTextures();
private:
    float specularPower;
	bool flags[MAX_MATERIALS];
};

#endif