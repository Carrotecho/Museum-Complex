//
//  Loader.h
//  UltraVision
//
//  Created by Easy Proger on 16.05.15.
//  Copyright (c) 2015 easy proger. All rights reserved.
//
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

#import "LocalizationSystem.h"

@protocol LoaderDelegate <NSObject>
-(bool)taskWithError:(NSURL*)errorurl;
-(void)updateLoaderDelegate:(double)progress sender:(id)sender;

@end


@interface LoaderWorker : NSObject<NSURLSessionDownloadDelegate> {
    long long _contentLength;
    long long _currentLength;
    NSMutableData* _receivedData;
   
    NSURLRequest* _urlRequest;
    NSURLConnection* _loader;
    double _persent;
}
@property (nonatomic, strong) NSURLSessionDownloadTask *downloadTask;
@property (nonatomic, strong) NSString *path;

@property (nonatomic, retain) id<LoaderDelegate> delegate;
-(long long)getDowloadedSize;
-(void)cancel;
-(void)startTask;
-(double)getProgress;
-(id)initWithPath:(NSString*)path;
-(long long)getTotalSize;
@end

@interface Loader : NSObject<LoaderDelegate,UIAlertViewDelegate> {
    NSMutableArray* workers;
    NSMutableDictionary*errorTasks;
    id  _targetHandler;
    SEL _selectorHandler;
    int currentTaskID;
    bool stop;
    
    NSURL *_lasterrorurl;
}

@property (nonatomic, retain) id<LoaderDelegate> delegate;

-(long long)downloadedSize;
-(void)addTask:(NSString*)path;
-(void)startTask:(int)index;
-(double)getProgress;
-(bool)load:(NSArray*)array;
-(long long)getTotalSize;
-(void)cancel;
@end

