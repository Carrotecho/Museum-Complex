//
//  GLES20ExtAndroid.h
//  UINew
//
//  Created by Easy Proger on 14.11.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#ifndef GLES20ExtAndroid_h
#define GLES20ExtAndroid_h

#ifdef __ANDROID__

#include <dlfcn.h>
#include "XTypes.h"



#ifndef DglMapBufferRangeEXT
#define DglMapBufferRangeEXT 1
static PFNGLMAPBUFFERRANGEEXTPROC glMapBufferRangeEXT;
#endif

#ifndef DglFlushMappedBufferRangeEXT
#define DglFlushMappedBufferRangeEXT 1
static PFNGLFLUSHMAPPEDBUFFERRANGEEXTPROC glFlushMappedBufferRangeEXT;
#endif

#ifndef DglClientWaitSyncAPPLE
#define DglClientWaitSyncAPPLE 1
static PFNGLCLIENTWAITSYNCAPPLEPROC glClientWaitSyncAPPLE;
#endif

#ifndef DglFenceSyncAPPLE
#define DglFenceSyncAPPLE 1
static PFNGLFENCESYNCAPPLEPROC glFenceSyncAPPLE;
#endif



inline void initialiseFunctionsGL20ExtFunctions () {
    void *libhandle = dlopen("libGLESv2.so", RTLD_LAZY);
    glMapBufferRangeEXT         = (PFNGLMAPBUFFERRANGEEXTPROC)         dlsym(libhandle, "glMapBufferRangeEXT");
    glFlushMappedBufferRangeEXT = (PFNGLFLUSHMAPPEDBUFFERRANGEEXTPROC) dlsym(libhandle, "glFlushMappedBufferRangeEXT");
    glClientWaitSyncAPPLE       = (PFNGLCLIENTWAITSYNCAPPLEPROC)       dlsym(libhandle, "glClientWaitSyncAPPLE");
    glFenceSyncAPPLE            = (PFNGLFENCESYNCAPPLEPROC)            dlsym(libhandle, "glFenceSyncAPPLE");
}


#else




#endif


#endif /* GLES20ExtAndroid_h */
