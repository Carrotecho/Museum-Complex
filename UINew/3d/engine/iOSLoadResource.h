//
//  iOSLoadResource.h
//  UltraVision
//
//  Created by Easy Proger on 03.07.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "XTypes.h"



@interface iOSLoadResource : NSObject




+(id)sharedInstance;
- (TextureInfo*) loadTexture:(char *) aParameter;

@end
