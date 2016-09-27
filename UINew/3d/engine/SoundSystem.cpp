//
//  SoundSystem.cpp
//  
//
//  Created by easy proger on 10.07.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#include "SoundSystem.h"
#include "XTECH-C-Interface.h"

SoundSystem::SoundSystem() {
    lastPlayedTrack = 0;
    currentIndxForCache = -1;
    initializeOpenAL();
    
    
}

SoundSystem::~SoundSystem() {
    clear();
}

bool SoundSystem::initializeOpenAL()
{

    return true;
}
std::string GetFileExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}



int SoundSystem::addFile(char*pathToFile) {
    
    
    SoundData *data = new SoundData();
    data->loaded = false;
    data->filePath = pathToFile;
    fileLibs.push_back(data);
    
    return ((uint)fileLibs.size())-1;
}


void SoundSystem::stopSound() {
    std::string path;
    stopSoundFromCPP(path);
}

void SoundSystem::playSound(uint id,bool loop) {
    if (id >= fileLibs.size()) return;
    
    SoundData *data = fileLibs[id];
    
    errors.clear();
    
    playSoundFromCPP(data->filePath,loop);
}

void SoundSystem::destroySound(SoundData*d) {
    

    printf("removed buffer ID:%d %d\n",d->bufferID,d->sourceID);
    
    d->loaded = false;
}

void SoundSystem::clear() {
    clearAllSoundData();
}

void SoundSystem::clearAllSoundData() {
    for (int i = 0; i < fileLibs.size(); i++) {
        delete fileLibs[i];
    }
    fileLibs.clear();
}

int SoundSystem::loadFile(char*pathToFile) {

    FILE* file = fopen(pathToFile, "rb");
	if (!file) {
		return 0;
	}

    fclose(file);
    return 1;
}


void SoundSystem::destroyOpenAL()
{
    // Очищаем все буффера
    //for (TBuf::iterator i = Buffers.begin(); i != Buffers.end(); i++)
    //    alDeleteBuffers(1, &i->second.ID);
    
    for (uint i = 0; i < loadedTracks.size(); i++) {
        uint id = loadedTracks[i];
        SoundData *d = fileLibs[id];
        if (d->loaded) {
            destroySound(d);
        }
		delete d;
    }

    
}