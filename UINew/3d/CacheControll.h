//
//  CacheControll.h
//  Mousson Atelier
//
//  Created by easy proger on 08.05.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>




@interface CacheControll : NSObject<NSURLConnectionDelegate> {
    long long _contentLength;
    NSMutableData* _receivedData;
    NSURLRequest* _urlRequest;
    NSURLConnection* _loader;
    NSString * _path;
    
    
}
@property(nonatomic,readwrite) bool isInternetActive;
+(void)cacheData:(NSData*)data fromServerPath:(NSString*)path;
+(BOOL)isCachedAtPath:(NSString*)path;

+(NSString*)getLocalPathFromLocation:(NSString*)path;
+(NSData*)getDataFromCacheAtPath:(NSString*)path;
+(NSString*)getPathFromCacheAtPath:(NSString*)path;

+(id)cacheDatafromServerPath:(NSString*)path;
-(id)initWithPath:(NSString*)path;

+(void)setCheckCacheActual:(bool) v;
+(bool)getCheckCacheActual;

+(void)setCacheActualString:(NSString*)v;
+(NSString*)getCacheActualString;
+(BOOL)isCachedAtPathExist:(NSString*)path;
+ (instancetype)sharedInstance;
+(void)cacheDataFromLocation:(NSString*)pathLocation fromServerPath:(NSString*)path;
@end
