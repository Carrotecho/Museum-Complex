//
//  CacheControll.m
//  Mousson Atelier
//
//  Created by easy proger on 08.05.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "CacheControll.h"
#import "Reachability.h"
#include "Loader.h"

@implementation CacheControll
@synthesize isInternetActive = _isInternetActive;


bool checkCacheActual;
NSString * cacheString;


+(BOOL)isCachedAtPathExist:(NSString*)path {
    NSString * s = [NSString stringWithFormat:@"Library/Caches/%@",path,nil];
    NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
    
    
    bool isActual = [[NSFileManager defaultManager] fileExistsAtPath:FilePath];
    
    return isActual;
}


+(BOOL)isCachedAtPath:(NSString*)path {
    
    path = [path stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    
    
    NSString * s = [NSString stringWithFormat:@"Library/Caches/%@",path,nil];
    NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
    
    
    bool isActual = [[NSFileManager defaultManager] fileExistsAtPath:FilePath];
    
    if (isActual) {
        isActual = [CacheControll isCachedActualAtPath:path];
    }
    
    return isActual;
}



#define CacheControlInst [CacheControll sharedInstance]


+ (instancetype)sharedInstance
{
    static CacheControll *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[CacheControll alloc] init];
        sharedInstance.isInternetActive = false;
        // Do any other initialisation stuff here
    });
    return sharedInstance;
}

-(instancetype)init {
    self = [super init];
    if (self) {
        
    }
    return self;
    
    
    
}

-(void)backgroundLoad:(NSString*)path {
    
    if (_isInternetActive) {
        NSString * s = [NSString stringWithFormat:@"Library/Caches/%@",path,nil];
        NSString *localPath = [NSHomeDirectory() stringByAppendingPathComponent:s];
        
        NSError * error = nil;
        NSString * pathToDataModifiedLocaleFile = [NSString stringWithFormat:@"%@.dataModified",localPath];
        NSString * localDataModifiedDate = [[NSString alloc] initWithContentsOfFile:pathToDataModifiedLocaleFile encoding:NSUTF8StringEncoding error:&error];
        if (!localDataModifiedDate) {
            
        }
        
        NSString * pathToDataModifiedOnlineFile = [NSString stringWithFormat:@"%@.dataModified",path];
        NSString * onlineDataModifiedDate = [[NSString alloc] initWithContentsOfURL:[NSURL URLWithString:pathToDataModifiedOnlineFile] encoding:NSUTF8StringEncoding error:&error];
        
        if (onlineDataModifiedDate) {
            if ([localDataModifiedDate isEqualToString:onlineDataModifiedDate]) {
            }else {
                // need reload
                Loader* loader  = [[Loader alloc] init];
                [loader addTask:path];
                [loader startTask:0];
            }
        }
    }
    
}

-(void)defferedLoading:(NSString*)path {
    //if ([CacheControll isInternetActive]) {
    [self performSelectorInBackground:@selector(backgroundLoad:) withObject:path];
    //}
}


+(BOOL)isCachedActualAtPath:(NSString*)path {
    [[CacheControll sharedInstance] performSelectorOnMainThread:@selector(defferedLoading:) withObject:path waitUntilDone:NO];
    return true;
}



+(NSString*)getLocalPathFromLocation:(NSString*)path {
    NSString * s = [NSString stringWithFormat:@"Library/Caches/%@",path,nil];
    NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
    FilePath = [FilePath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
    FilePath = [FilePath stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
    return FilePath;
}

+(void)cacheDataFromLocation:(NSString*)pathLocation fromServerPath:(NSString*)path {
    
    
    
    NSString *FilePath = [CacheControll getLocalPathFromLocation:path];// [NSHomeDirectory() stringByAppendingPathComponent:s];
    NSError * error = NULL;
    
    
   
    
    
    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:FilePath];
    if (fileExists) {
        NSFileManager *fileManager = [NSFileManager defaultManager];
        [fileManager removeItemAtPath:[NSString stringWithFormat:@"%@.dataModified",path] error:&error];
        [fileManager removeItemAtPath:path error:&error];
        
    }
    
    
    NSString * dirPath = [FilePath stringByDeletingLastPathComponent];
    
    [[NSFileManager defaultManager] createDirectoryAtPath:dirPath
                              withIntermediateDirectories:YES
                                               attributes:nil
                                                    error:&error];
    
    error = NULL;
    
    
    
    // We've successfully finished the download. Let's save the file
    NSFileManager *fileManager = [NSFileManager defaultManager];
    

    BOOL success = [fileManager copyItemAtPath:pathLocation toPath:FilePath error:&error];
    // Make sure we overwrite anything that's already there
    
    
    
    //[data writeToFile:[FilePath stringByExpandingTildeInPath] options:NSDataWritingAtomic error:&error];
    
    
    
    NSString * pathToDataModifiedOnlineFile = [NSString stringWithFormat:@"%@.dataModified",path];
    NSString * onlineDataModifiedDate = [[NSString alloc] initWithContentsOfURL:[NSURL URLWithString:pathToDataModifiedOnlineFile] encoding:NSUTF8StringEncoding error:&error];
    
    if (onlineDataModifiedDate) {
        [onlineDataModifiedDate writeToFile:[NSString stringWithFormat:@"%@.dataModified",FilePath] atomically:YES encoding:NSUTF8StringEncoding error:&error];
    }else {
        [@"cache" writeToFile:[NSString stringWithFormat:@"%@.dataModified",FilePath] atomically:YES encoding:NSUTF8StringEncoding error:&error];
    }
}


+(void)cacheData:(NSData*)data fromServerPath:(NSString*)path {
    NSString * s = [NSString stringWithFormat:@"Library/Caches/%@",path,nil];
    
    NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
    
    
    NSError * error = NULL;
    
    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:FilePath];
    if (fileExists) {
        NSFileManager *fileManager = [NSFileManager defaultManager];
        [fileManager removeItemAtPath:[NSString stringWithFormat:@"%@.dataModified",path] error:&error];
        [fileManager removeItemAtPath:path error:&error];
        
    }
    
    
    NSString * dirPath = [FilePath stringByDeletingLastPathComponent];
    
    [[NSFileManager defaultManager] createDirectoryAtPath:dirPath
                              withIntermediateDirectories:YES 
                                               attributes:nil
                                                    error:&error];
    
    error = NULL;
    
    
    [data writeToFile:[FilePath stringByExpandingTildeInPath] options:NSDataWritingAtomic error:&error];
    
    
    
    NSString * pathToDataModifiedOnlineFile = [NSString stringWithFormat:@"%@.dataModified",path];
    NSString * onlineDataModifiedDate = [[NSString alloc] initWithContentsOfURL:[NSURL URLWithString:pathToDataModifiedOnlineFile] encoding:NSUTF8StringEncoding error:&error];
    
    if (onlineDataModifiedDate) {
        [onlineDataModifiedDate writeToFile:[NSString stringWithFormat:@"%@.dataModified",FilePath] atomically:YES encoding:NSUTF8StringEncoding error:&error];
    }else {
        [@"cache" writeToFile:[NSString stringWithFormat:@"%@.dataModified",FilePath] atomically:YES encoding:NSUTF8StringEncoding error:&error];
    }
}


+(id)cacheDatafromServerPath:(NSString*)path {
    return [[[self alloc] initWithPath:path] autorelease];
}

- (id)initWithPath:(NSString*)path
{
    self = [super init];
    if (self) {
        _path = [path retain];
        _receivedData = [[NSMutableData alloc] init];
        _urlRequest = [NSURLRequest requestWithURL:[NSURL URLWithString:path]];    
        _loader = [[NSURLConnection alloc] initWithRequest:_urlRequest delegate:self];
    }
    return self;
}

- (void)dealloc
{
    [_path release];
    [super dealloc];
}

- (void)connection:(NSURLConnection* )connection didReceiveResponse:(NSURLResponse* )response {
    [_receivedData setLength:0];
	_contentLength = [response expectedContentLength];
}

- (void)connection:(NSURLConnection* )connection didReceiveData:(NSData* )data {
    [_receivedData appendData:data];
}

- (void)connection:(NSURLConnection* )connection didFailWithError:(NSError* )error {
    [connection release];
    [_receivedData setLength:0];
    
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
    
    [CacheControll cacheData:_receivedData fromServerPath:_path];
    
    [connection release];
    [_receivedData setLength:0];
}



+(NSString*)getPathFromCacheAtPath:(NSString*)path{
    if ([CacheControll isCachedAtPath:path]) {
        if ([CacheControll getCheckCacheActual]) {
            if ([CacheControll isCachedActualAtPath:path]) {
                NSString * s = [NSString stringWithFormat:@"Library/Caches/%@",path,nil];
                NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
                return FilePath;
            }else {
                return nil;
            }
        }else {
            NSString * s = [NSString stringWithFormat:@"Library/Caches/%@",path,nil];
            NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
            return FilePath;
        }
        
    }else {
        NSLog(@"not cached at url path:%@",path);
    }
    return nil;
}

+(NSData*)getDataFromCacheAtPath:(NSString*)path{

    if ([CacheControll isCachedAtPath:path]) {
        if ([CacheControll getCheckCacheActual]) {
            if ([CacheControll isCachedActualAtPath:path]) {
                NSString * s = [NSString stringWithFormat:@"Library/Caches/%@",path,nil];
                NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
                return [NSData dataWithContentsOfFile:FilePath];
            }else {
                return nil;
            } 
        }else {
            NSString * s = [NSString stringWithFormat:@"Library/Caches/%@",path,nil];
            NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
            return [NSData dataWithContentsOfFile:FilePath]; 
        }
        
    }else {
        NSLog(@"not cached at url path:%@",path);
    }
    return nil;
}

+(void)setCacheActualString:(NSString*) v {
    if (cacheString) {
        [cacheString release];
    }
    cacheString = [v retain];
}
+(NSString*)getCacheActualString{ return cacheString;}

+(void)setCheckCacheActual:(bool) v { checkCacheActual = v;}
+(bool)getCheckCacheActual{ return checkCacheActual;}
@end
