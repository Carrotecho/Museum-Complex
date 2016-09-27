//
//  SingltonForVuforia.m
//  UINew
//
//  Created by Easy Proger on 07.10.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import "SingltonForVuforia.h"

@implementation SingltonForVuforia


@synthesize sessionApp = _sessionApp;
@synthesize glView = _glView;


#pragma mark Singleton Methods

+ (id)sharedManager {
    static SingltonForVuforia *sharedMyManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedMyManager = [[self alloc] init];
    });
    return sharedMyManager;
}

- (id)init {
    if (self = [super init]) {
        _glView = NULL;
        _sessionApp = NULL;
    }
    return self;
}

- (void)dealloc {
    // Should never be called, but just here for clarity really.
}

@end
