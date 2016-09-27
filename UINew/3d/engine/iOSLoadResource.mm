//
//  iOSLoadResource.m
//  UltraVision
//
//  Created by Easy Proger on 03.07.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#import "iOSLoadResource.h"

@implementation iOSLoadResource




TextureInfo* loadTextureCPP ( char *aParameter)
{
    return [ (id) CFBridgingRelease(CFBridgingRetain([iOSLoadResource sharedInstance])) loadTexture:aParameter];
}
+ (instancetype)sharedInstance
{
    static iOSLoadResource *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[iOSLoadResource alloc] init];
        // Do any other initialisation stuff here
    });
    return sharedInstance;
}

- (TextureInfo*) loadTexture:(char *) aParameter {

    
    NSString*pathS = [NSString stringWithUTF8String:aParameter];
    
    
    NSData *texData = [[NSData alloc] initWithContentsOfFile:pathS];
    
    UIImage *image = [[UIImage alloc] initWithData:texData];
    if (image == nil)
        return nullptr;
    
    
    TextureInfo*info = (TextureInfo*)malloc(sizeof(TextureInfo));
    
    info->width = CGImageGetWidth(image.CGImage);
    info->height = CGImageGetHeight(image.CGImage);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    void *imageData = malloc( info->height * info->width * 4 );
    CGContextRef context = CGBitmapContextCreate( imageData, info->width, info->height, 8, 4 * info->width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect( context, CGRectMake( 0, 0, info->width, info->height ) );
    CGContextTranslateCTM( context, 0, info->height - info->height );
    CGContextDrawImage( context, CGRectMake( 0, 0, info->width, info->height ), image.CGImage );
    
    info->data = imageData;

    
    CGContextRelease(context);
    
    return info;
};

@end
