//
//  ResourceManager.h
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 15.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//
#ifndef RESOURCEMANAGER_H__
#define RESOURCEMANAGER_H__




#ifdef __ANDROID__

#include <android/log.h>

#define  LOG_TAG    "GLViewA"
#define  XLOG_w(tag,text)  __android_log_write(ANDROID_LOG_DEBUG,tag,text)
#define  XLOG(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#else
#define  XLOG_w(tag,text) printf("%s %s\n",tag,text)
#define  XLOG(...) printf(__VA_ARGS__)
#endif


class ResourcesManager {
private:

public:
    

    ResourcesManager();
    ~ResourcesManager(){};
};


#endif