//
//  Textures.h
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 13.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//

#ifndef TEXTURE_H_DEF
#define TEXTURE_H_DEF

#include <iostream>

#include "Types.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <GL/glew.h>
#else
#ifdef __ANDROID__
#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cstring>
#else
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif
#endif

#include "PVRLoader.h"

// need create Image library headers and formats and push to 

typedef struct {
    uint width;
    uint height;
    uint bytesNum;
}FrameImageHeader;


class Texture {
    uint formatTexture;
public:
    Texture(){
        texID = 0;
        _isDamaged = true;
        countUsed = 0;
        pathForTexture = "";
    };
    ~Texture();
    
    GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
	GLuint	texID;											// Texture ID Used To Select A Texture
	GLuint	type;
    void setTextureID(uint formatTexture,uint texID);
    uint getTextureID();
    uint getTextureFormat();
    uint createClearTexture(int width,int height,uint formatTexture,uint formatBitsAndChanel,uint formatChanel,uint formatBits);
    uint createFrombufferAndUploadTexture(uint formatTexture);
    
	
    void createTextureSimple();
    void createTexture();
    int createTexture(const std::string &path);
    
    bool isDamaged(){
        
        return _isDamaged;
    };
    void releaseTexture();
    void reloadTexture();

	void deleteTexture();

	std::string pathForTexture;
private:
    int read_png_file(const char * file_name, int * width, int * height);
    
   
    bool _isDamaged;
    int countUsed;
};



#endif