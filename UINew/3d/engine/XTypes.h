//
//  XTypes.h
//  XTech
//
//  Created by easy proger on 09.02.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef XTypes_h
#define XTypes_h
// hi here

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#ifndef __ANDROID__
#define M_PI 3.14159265358979323846264338327950288
#endif

#define TARGET_iOS 1

#define WIDTH_SHADOW_MAP 1024
#define HEIGHT_SHADOW_MAP 1024

#define WIDTH_VIEWPORT 768
#define HEIGHT_VIEWPORT 1024


#define PRINT_COMPRESSED_TEXTURE_FORMATS 0
#define X_DEBUG 1
#define X_DEBUG_SHADERS 1


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define GL_DEPTH_COMPONENT24_OES GL_DEPTH_COMPONENT24 
    #define GL_DEPTH24_STENCIL8_OES GL_DEPTH24_STENCIL8 

#else
#if TARGET_iOS
    #define GL_DEPTH_COMPONENT24 GL_DEPTH_COMPONENT24_OES
    #define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#endif
#endif



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <gl/glew.h>

#define glMapBufferRangeEXT glMapBufferRange
#define GL_MAP_WRITE_BIT_EXT GL_MAP_WRITE_BIT
#define GL_MAP_FLUSH_EXPLICIT_BIT_EXT GL_MAP_FLUSH_EXPLICIT_BIT
#define GL_MAP_UNSYNCHRONIZED_BIT_EXT GL_MAP_UNSYNCHRONIZED_BIT


#define glClientWaitSyncAPPLE glClientWaitSyncNop
#define glFenceSyncAPPLE glFenceSyncNop



#include <stdio.h>

typedef int GLsyncNop;
#define GLsync GLsyncNop

inline GLsync glFenceSyncNop(GLenum condition,
	GLbitfield flags) {
	return 0;
}

inline void glClientWaitSyncNop(GLsync sync,
	GLbitfield flags,
	GLuint64 timeout) {

}

//#define glFenceSyncAPPLE glFenceSync
//#define glClientWaitSyncAPPLE glClientWaitSync
#define GL_SYNC_FLUSH_COMMANDS_BIT_APPLE GL_SYNC_FLUSH_COMMANDS_BIT
#define GL_TIMEOUT_IGNORED_APPLE GL_TIMEOUT_IGNORED
#define glUnmapBufferOES glUnmapBuffer
#define glFlushMappedBufferRangeEXT glFlushMappedBufferRange

#define GL_SYNC_GPU_COMMANDS_COMPLETE_APPLE GL_SYNC_GPU_COMMANDS_COMPLETE
    
#else

#ifdef __ANDROID__

#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2.h>

#define GL_MAP_WRITE_BIT_EXT              0x0002
#define GL_MAP_FLUSH_EXPLICIT_BIT_EXT     0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT_EXT     0x0020
#define GL_SYNC_FLUSH_COMMANDS_BIT_APPLE  0x00000001
#define GL_TIMEOUT_IGNORED_APPLE          0xFFFFFFFFFFFFFFFFull
#define GL_SYNC_GPU_COMMANDS_COMPLETE_APPLE 0x9117

#define GL_APIENTRYP GL_APIENTRY*
typedef struct __GLsync *GLsync;
typedef khronos_uint64_t GLuint64;

typedef void *(GL_APIENTRYP PFNGLMAPBUFFERRANGEEXTPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GL_APIENTRYP PFNGLFLUSHMAPPEDBUFFERRANGEEXTPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef GLenum (GL_APIENTRYP PFNGLCLIENTWAITSYNCAPPLEPROC) (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef GLsync (GL_APIENTRYP PFNGLFENCESYNCAPPLEPROC) (GLenum condition, GLbitfield flags);


#else

#include <OpenGLES/ES2/gl.h>
#endif
#endif



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define AGRESIVE_INLINE static 
#define X_INLINE_X  inline
#else
#define AGRESIVE_INLINE inline __attribute__((always_inline))
#define X_INLINE_X  inline
#endif





#ifndef X_REGCALL
#ifdef __INTEL_COMPILER
#define X_REGCALL __regcall
#else
#define X_REGCALL __fastcall
#endif
#endif

#define X_ALIGN16 __attribute__((aligned(16)))


typedef unsigned long DWORD;


#define DW_AS_FLT(DW) (*(FLOAT*)&(DW))
#define FLT_AS_DW(F) (*(DWORD*)&(F))
#define FLT_SIGN(F) ((FLT_AS_DW(F) & 0x80000000L))
#define ALMOST_ZERO(F) ((FLT_AS_DW(F) & 0x7f800000L)==0)


AGRESIVE_INLINE  
const char * xOpenGLStrError(GLenum code)
{
    switch(code)
    {
        case GL_NO_ERROR:
            return "No error";
            break;
        case GL_INVALID_ENUM:
            return "Invalid enum";
            break;
        case GL_INVALID_VALUE:
            return "Invalid value";
            break;
        case GL_INVALID_OPERATION:
            return "Invalid operation";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "invalid frame buffer";
            break;
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
            break;
    }
    return "Unknown error";
}


#define GET_GLERROR()                                                                                 \
{                                                                                                     \
    GLenum code = glGetError();                                                                       \
    while (code!=GL_NO_ERROR) {                                                                       \
        fprintf(stderr, "%s [line %d]  GL Error: %s\n",__FUNCTION__,__LINE__,xOpenGLStrError(code));  \
        code = glGetError();                                                                          \
    }                                                                                                 \
}                                                                                                     \


typedef struct {
	unsigned int id;
	char name[50];
}UintNameStruct;


#include <stdlib.h>

typedef struct {
    unsigned long width;
    unsigned long height;
    unsigned int bytePerPixel;
    
    void*data;
    
    
}TextureInfo;




typedef unsigned int facesType;

#define BASE_TYPE_INDEXES GL_UNSIGNED_INT



#endif


