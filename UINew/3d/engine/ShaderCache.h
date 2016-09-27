//
//  ShaderCache.h
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 15.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//
#ifndef SHADER_CACHE_H__
#define SHADER_CACHE_H__

#define UNIFORM_PERSPECTIVE_MATRIX               1
#define UNIFORM_CAMERA_VIEW_MATRIX               2
#define UNIFORM_WORLD_OBJECT_POSITION            3
#define UNIFORM_FAR_PLANE                        4
#define UNIFORM_MODEL_NORMAL_MATRIX              5
#define UNIFORM_CAMERA_VIEW_INVERSE3             6
#define UNIFORM_CAMERA_POSITION                  7
#define UNIFORM_LIGHT_POSITION                   8
#define UNIFORM_DEPTH_TEXTURE                    9
#define UNIFORM_LIGHT_TEXTURE                    10
#define UNIFORM_OBJECT_DIFFUSE_TEXTURE           11
#define UNIFORM_PERSPECTIVE_VIEW_INVERSE_MATRIX  12
#define UNIFORM_NORMAL_MATERIAL_TEXTURE          13
#define UNIFORM_ALBEDO_TEXTURE                   14
#define UNIFORM_PERSPECTIVE_LIGHT_MATRIX         15
#define UNIFORM_LIGHT_VIEW_MATRIX                16
#define UNIFORM_SHADOW_MAPS                      17
#define UNIFORM_LIGHT_PROJ_MODEL_VIEW_MATRIX     18
#define UNIFORM_LIGHT_PROJ_MODEL_VIEW_ST_MATRIX  19
#define UNIFORM_PROJ_CAMERA_MODEL_VIEW           20
#define UNIFORM_BONES_TRANSLATIONS               21
#define UNIFORM_BONES_ROTATIONS                  22
#define UNIFORM_NORMAL_MAP                       23
#define UNIFORM_SPECULAR_MAP                     24
#define UNIFORM_INDEX_MESH                       25
#define UNIFORM_COLOR_MATERIAL                   26
#define UNIFORM_CAMERA_RIGHT_WS                  27
#define UNIFORM_CAMERA_UP_WS                     28
#define UNIFORM_SPECULAR_POWER                   29

#define MAX_UNIFORMS   40
#define MAX_ATTRIBUTES 40

#include <iostream>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#else
#ifdef __ANDROID__
#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2ext.h>
#else
#import <OpenGLES/ES2/gl.h>
#endif
#endif

#include "stdio.h"
#include "stdlib.h"
#include "XSingleton.h"
#include "Types.h"
#include <vector>
#include <map>
#include <string>
#include "pugixml.hpp"
#include "Mesh.h"
#include "ShaderParamsSetter.h"





using namespace Xengine;
using namespace std;


struct AttributeShader {
    uint idAttributeConstant;
    uint idAttribute;
    uint size;
    uint type;
    bool normalized;
};
struct UniformShader {
    uint idUniform;
    uint idUniformConstant;
    void (ShaderParamsSetter::*action)(uint idUniform);
    ShaderParamsSetter*target;
    
};
struct Shader {
    uint idProgram;
    AttributeShader*attributes;
    UniformShader*uniforms;
    uint uniformsLenght;
    uint attributesLenght;
    void deleteShader() {
        free(attributes);
        free(uniforms);
        glDeleteProgram(idProgram);
        uniformsLenght = 0;
        attributesLenght = 0;
    };
};




class ShaderCache {
private:
    void getShadersName(Material*mat ,Mesh*model, uint pass,vector<string> *outList);
    
    void (ShaderParamsSetter::*action[MAX_UNIFORMS])(uint idUniform);
    
    uint attributesTableLenght;
    
    uint uniforms;
    uint attributes;
    
    int attributesLenght;
    
    
    bool positionSeted;
    
    std::string codeHeader;
    std::string positionBlock;
    std::string varyingsBlock;
    std::string attributesBlock;
    std::string uniformsBlock;
    std::string extensionBlock;
    
    std::string vertexShaderSource;
    std::string pixelShaderSource;
    
    Shader* createShaderProgram(const char*vertexSource,const char*pixelSource,Mesh*model);
    void fillExtensions(pugi::xml_node *shader);
    void fillUniforms(pugi::xml_node *shader);
    void fillAttributes(pugi::xml_node *shader);
    void fillVaryings(pugi::xml_node *shader);
    void createLocalVarForAttribute(pugi::xml_node &attributeInfo,pugi::xml_node &attribute,string &prefix);
    void fillLocalVars(pugi::xml_node *shader);
    
    pugi::xml_document shaderXML;
    pugi::xml_document declorationTableXML;
    
    
    std::map<std::string, Shader*> shadersCache;
    std::string uid;
    std::string resourcePath;
public:
    Shader* createShader(Material*mat ,Mesh*model, uint pass);
    ShaderCache(std::string _resourcePath);
    ~ShaderCache();
    char* itoa(int val, int base);
    Shader* get(Material*mat ,Mesh*model, uint pass);
    
};

class CodeMap {
public:
    uint id;
    string code;
};


#endif