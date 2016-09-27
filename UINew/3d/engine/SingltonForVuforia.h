//
//  SingltonForVuforia.h
//  UINew
//
//  Created by Easy Proger on 07.10.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "GLView.h"
#include "SampleApplicationSession.h"

@interface SingltonForVuforia : NSObject {
}

@property (nonatomic, retain) GLView *glView;
@property (nonatomic, retain) SampleApplicationSession *sessionApp;


+ (id)sharedManager;

@end