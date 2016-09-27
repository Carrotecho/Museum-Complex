//
//  SoundSystem.h
//  
//
//  Created by easy proger on 10.07.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef ____SoundSystem__
#define ____SoundSystem__

#include <iostream>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#include <al.h>
#include <alc.h>
#include "XMathBase.h"
#else
#ifdef __ANDROID__
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AudioToolbox/AudioToolbox.h>
#endif
#endif





#include <vector>

#define MAX_SOUNDS_LOAD 1


class SoundData{
public:
    std::string filePath;
    bool destroyed;
    bool loaded;
    uint bufferID;
    uint sourceID;
};

class SoundSystem {
public:
    SoundSystem();
    ~SoundSystem();

    void clear();
    void stopSound();
    void playSound(uint id,bool loop) ;
    int addFile(char*pathToFile);
    std::vector<unsigned long> errors;
    void clearAllSoundData();
private:
    

    
    bool initializeOpenAL();
    void destroyOpenAL();
    
    uint currentIndxForCache;
    std::vector<SoundData*> fileLibs;
    int loadFile(char*pathToFile);
    std::vector<int> loadedTracks;
    
    void destroySound(SoundData*d);
    
    int lastPlayedTrack;
};




class Sound {
    
};


#endif /* defined(____SoundSystem__) */
