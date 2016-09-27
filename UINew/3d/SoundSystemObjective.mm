//
//  SoundSystemObjective.m
//  UltraVision
//
//  Created by Easy Proger on 18.05.15.
//  Copyright (c) 2015 easy proger. All rights reserved.
//

#import "SoundSystemObjective.h"
#import <string>
@implementation SoundSystemObjective
// C "trampoline" function to invoke Objective-C method

@synthesize audioPlayer = _audioPlayer;


int playSoundFromCPP (std::string aParameter,bool loop)
{
    void* self;
    self = (void*)CFBridgingRetain([SoundSystemObjective sharedInstance]);
    // Call the Objective-C method using Objective-C syntax
    return [ (id) CFBridgingRelease(self) playSound:aParameter loop:loop];
}

void stopSoundFromCPP (std::string aParameter)
{
    void* self;
    self = (void*)CFBridgingRetain([SoundSystemObjective sharedInstance]);
    // Call the Objective-C method using Objective-C syntax
    [ (id) CFBridgingRelease(self) stopSound:aParameter];
}

+ (instancetype)sharedInstance
{
    static SoundSystemObjective *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[SoundSystemObjective alloc] init];
        // Do any other initialisation stuff here
    });
    return sharedInstance;
}


-(instancetype)init {
    self = [super init];
    if (self) {
        _audioPlayer = nil;
    }
    return self;
}

- (void) stopSound:(std::string) aParameter {
    if (_audioPlayer!=nil) {
        [_audioPlayer stop];
    }
}

- (int) playSound:(std::string) aParameter loop:(BOOL)loop
{
    // The Objective-C function you wanted to call from C++.
    // do work here..
    NSError*error = nil;
    NSString*pathForSound = [NSString stringWithUTF8String:aParameter.c_str()];
    
    if (_audioPlayer!=nil) {
        [_audioPlayer stop];
    }
    
    NSData *songFile = [[NSData alloc] initWithContentsOfFile:pathForSound options:NSDataReadingMappedIfSafe error:&error ];
    error = nil;
    _audioPlayer = [[AVAudioPlayer alloc] initWithData:songFile error:&error];
    
    if (loop) {
        _audioPlayer.numberOfLoops = -1;
    }
    
    
    [_audioPlayer prepareToPlay];
    [_audioPlayer play];
    
    return 21 ; // half of 42
}
@end
