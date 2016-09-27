//
//  Loader.m
//  UltraVision
//
//  Created by Easy Proger on 16.05.15.
//  Copyright (c) 2015 easy proger. All rights reserved.
//

#import "Loader.h"
#import "CacheControll.h"

@implementation Loader


-(id)init {
    self = [super init];
    if (self) {
        currentTaskID = 0;
        workers = [[NSMutableArray alloc] init];
        errorTasks = [[NSMutableDictionary alloc] init];
    }
    return self;
}

-(bool)load:(NSArray*)array {
    [[UIApplication sharedApplication] setIdleTimerDisabled: YES];
    stop = false;
    for (int i = 0; i < [workers count]; i++) {
        LoaderWorker*work = [workers objectAtIndex:i];
        [work cancel];
    }
    [workers removeAllObjects];
    
    for (int i = 0; i < [array count]; i++) {
        NSString*path = [array objectAtIndex:i];
        
        path = [path stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];
        path = [path stringByReplacingOccurrencesOfString:@"\\\\" withString:@"/"];
        path = [path stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        
        if ([CacheControll isCachedAtPath:path]) {
            
            
        }else {
            [self addTask:path];
        }
    }
    if (![workers count]) {
        [[UIApplication sharedApplication] setIdleTimerDisabled: NO];
        if (self.delegate && [self.delegate respondsToSelector:@selector(updateLoaderDelegate:sender:)]) {
            [self.delegate updateLoaderDelegate:100 sender:self];
        }
        return false;
    }
    
    currentTaskID = 0;
    [self startTask:currentTaskID];
    
    
    return true;
}

-(void)addTask:(NSString*)path {
    LoaderWorker*work = [[LoaderWorker alloc] initWithPath:path];
    work.delegate = self;
    [workers addObject:work];
}


-(long long)downloadedSize
{
    long long size = 0;
    for (int i =0;i<[workers count];i++)
    {
        size += [workers[i] getDowloadedSize];
    }
    return size;
}

-(void)startTask:(int)index {
    if (index>=[workers count]) {
        // end of task's
        return;
    }
    currentTaskID = index;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        //progress;
        LoaderWorker*work = [workers objectAtIndex:index];
        [work startTask];
    });
    
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    
    if (buttonIndex == 0) {
        [[UIApplication sharedApplication] setIdleTimerDisabled: NO];
        if (self.delegate && [self.delegate respondsToSelector:@selector(taskWithError:)]) {
            [self.delegate taskWithError:_lasterrorurl];
        }
        [self cancel];
        return;
    }
    
    
    stop = false;
    [self repeat:true];
}


-(void)cancel {
    stop = true;
    for (int i = 0; i < [workers count]; i++) {
        LoaderWorker*work = [workers objectAtIndex:i];
        [work cancel];
    }
    
    [errorTasks removeAllObjects];
    [workers removeAllObjects];
}

- (void)alertViewCancel:(UIAlertView *)alertView {
    
}


-(bool)taskWithError:(NSURL*)errorurl {
    
    bool haveAttempts = false;
    
    if ([errorTasks objectForKey:[errorurl absoluteString]]) {
        NSNumber*numberAttempts = [errorTasks objectForKey:[errorurl absoluteString]];
        if ([numberAttempts intValue] < 3) {
            int attempts = [numberAttempts intValue]+1;
            [errorTasks setValue:[NSNumber numberWithInt:attempts] forKey:[errorurl absoluteString]];
            haveAttempts = true;
        }else {
            
            _lasterrorurl = errorurl;

            dispatch_async(dispatch_get_main_queue(), ^{
                UIAlertView *successAlert = [[UIAlertView alloc] initWithTitle:AMLocalizedString(@"Error_alert", @"")
                                                                       message:AMLocalizedString(@"Error_download", @"")
                                                                      delegate:self
                                                             cancelButtonTitle:AMLocalizedString(@"Cancel", @"")
                                                             otherButtonTitles:AMLocalizedString(@"Repeat", @""),nil];
                [successAlert show];
            });
            
            
            return haveAttempts;
        }
    }else {
        if (errorurl == nil) {
        }else {
            haveAttempts = true;
            [errorTasks setValue:[NSNumber numberWithInt:0] forKey:[errorurl absoluteString]];
        }
    }
    [self addTask:[errorurl absoluteString]];
    return haveAttempts;
}

-(void)updateLoaderDelegate:(double)progress sender:(id)sender {
    double prg = [self getProgress];
    
    if (stop) {
        return;
    }
    
    if (prg>=100) {
        stop = true;
        
        if ([self repeat:true]) {
            return;
        }
        [[UIApplication sharedApplication] setIdleTimerDisabled: NO];
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        //progress;
        
        if (self.delegate && [self.delegate respondsToSelector:@selector(updateLoaderDelegate:sender:)]) {
            [self.delegate updateLoaderDelegate:prg sender:self];
        }
    });
    
    LoaderWorker*work = (LoaderWorker*)sender;
    if ([work getProgress]>=100.0) {
        // task end
        work.delegate = nil;
        currentTaskID++;
        [self startTask:currentTaskID];
        
    }
}




-(BOOL)repeat:(BOOL)cancelTask {
    
    
    
    NSMutableDictionary* dictPaths = [[NSMutableDictionary alloc] init];
    
    for (int i = 0; i < [workers count]; i++) {
        LoaderWorker*work = [workers objectAtIndex:i];
        if (![dictPaths objectForKey:[work path]] && ![CacheControll isCachedAtPath:[work path]]) {
            [dictPaths setObject:[work path] forKey:[work path]];
        }
        if (cancelTask) [work cancel];
    }
    
    [errorTasks removeAllObjects];
    [workers removeAllObjects];
    
    if ([[dictPaths allKeys] count]) {
        [self load:[dictPaths allKeys]];
        return true;
    }
    return false;
}

-(double)getProgress {
    double persent = 0;
    double maxLenght = [workers count]*100.0;
    for (int i = 0; i < [workers count]; i++) {
        LoaderWorker*work = [workers objectAtIndex:i];
        persent+=[work getProgress];
    }
    return (persent/maxLenght)*100;
}

-(long long)getTotalSize
{
    long long size = 0;
    for (int i =0;i<[workers count];i++)
    {
        size += [workers[i] getTotalSize];
    }
    return size;
}


@end


@implementation LoaderWorker

@synthesize path = _path;



- (NSURLSession *)backgroundSession{
    static NSURLSession *session = nil;
    //NSLog(@"create new session");
    NSURLSessionConfiguration *config = [NSURLSessionConfiguration defaultSessionConfiguration];
    [config setAllowsCellularAccess:YES];
    session = [NSURLSession sessionWithConfiguration:config delegate:self delegateQueue:nil];
    return session;
}

#pragma mark - NSURLSession deleagte

- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error
{
    _persent = 100.0;
    
    if (error) {
        NSURL*urlError = [[task originalRequest] URL];
        bool haveAttempts = false;
        if (self.delegate && [self.delegate respondsToSelector:@selector(taskWithError:)]) {
            haveAttempts = [self.delegate taskWithError:urlError];
        }
        if (haveAttempts) {
            _persent = 100.0;
        }else {
            _persent = 0.0;
        }
    }
    
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateLoaderDelegate:sender:)]) {
        [self.delegate updateLoaderDelegate:_persent sender:self];
    }
}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite
{
    _currentLength = (long long)totalBytesWritten;
    _contentLength = totalBytesExpectedToWrite;
    _persent = (double)totalBytesWritten / (double)totalBytesExpectedToWrite*99.0;
    //NSLog(@"download: %@ progress: %f", downloadTask, _persent);
    //progress;
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateLoaderDelegate:sender:)]) {
        [self.delegate updateLoaderDelegate:_persent sender:self];
    }}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location
{
    //NSLog(@"did finish downloading");
    
    [CacheControll cacheDataFromLocation:[location path] fromServerPath:_path];
    
    //progress;
    _persent = 100;
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateLoaderDelegate:sender:)]) {
        [self.delegate updateLoaderDelegate:_persent sender:self];
    }
}

-(void)startTask {
    
    [self.downloadTask resume];
}

-(id)initWithPath:(NSString*)path {
    self = [super init];
    if (self) {
        _persent = 0;
        _path = path;
        
        NSFileManager *fileManager = [NSFileManager defaultManager];
        
        NSError *error;
        BOOL success = [fileManager removeItemAtPath:[CacheControll getPathFromCacheAtPath:path] error:&error];
        
        NSCharacterSet *set = [NSCharacterSet URLQueryAllowedCharacterSet];
        NSString *result = [_path stringByAddingPercentEncodingWithAllowedCharacters:set];
        
        
        self.downloadTask = [[self backgroundSession] downloadTaskWithURL:[NSURL URLWithString:result]];
        
        
    }
    return self;
}



-(void)cancel {
    [_loader cancel];
}


-(double)getProgress {
    return _persent;
}
- (void)connection:(NSURLConnection* )connection didReceiveResponse:(NSURLResponse* )response {
    [_receivedData setLength:0];
    _persent = 0;
    _currentLength = 0;
    _contentLength = [response expectedContentLength];
}

- (void)connection:(NSURLConnection* )connection didReceiveData:(NSData* )data {
    _currentLength+=[data length];
    _persent = ((float)_currentLength/(float)_contentLength)*100;
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateLoaderDelegate:sender:)]) {
        [self.delegate updateLoaderDelegate:_persent sender:self];
    }
    
    [_receivedData appendData:data];
}

- (void)connection:(NSURLConnection* )connection didFailWithError:(NSError* )error {
    _persent = 100;
    NSLog(@"%@ in loader",error);
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateLoaderDelegate:sender:)]) {
        [self.delegate updateLoaderDelegate:_persent sender:self];
    }
    [_receivedData setLength:0];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
    [CacheControll cacheData:_receivedData fromServerPath:_path];
    _persent = 100;
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateLoaderDelegate:sender:)]) {
        [self.delegate updateLoaderDelegate:_persent sender:self];
    }
    [_receivedData setLength:0];
}

-(long long)getDowloadedSize
{
    return _currentLength;
}

-(long long)getTotalSize
{
    return _contentLength;
}

@end