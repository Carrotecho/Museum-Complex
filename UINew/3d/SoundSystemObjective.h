//
//  SoundSystemObjective.h
//  UltraVision
//
//  Created by Easy Proger on 18.05.15.
//  Copyright (c) 2015 easy proger. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#include <string>

@interface SoundSystemObjective : NSObject

+(id)sharedInstance;
- (void) stopSound:(std::string) aParameter;
- (int) playSound:(std::string) aParameter loop:(BOOL)loop;
@property(nonatomic,readonly) AVAudioPlayer *audioPlayer;
@end
