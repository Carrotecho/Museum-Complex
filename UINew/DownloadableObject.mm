//
//  DownloadableObject.m
//  dwldDemo
//
//  Created by  Roman on 04.12.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import "DownloadableObject.h"
#import "MainController.h"
#import "CacheControll.h"

@implementation DownloadableObject:NSObject

-(id)initWithData:(NSMutableArray*)links
{
    if ( self = [super init])
    {
        _paths = links;
        [self RefreshWithData:links];
        _size = [self GetApproxSize];
        if ( alreadyDownloaded < _size )
        {
            _status = NOT_DOWNLOADED;
        }
        else
        {
            _status = DOWNLOADED;
        }
        
    }
    return self;
}

-(bool)taskWithError:(NSURL *)errorurl {
    return false;
}


-(void)updateLoaderDelegate:(double)progress sender:(id)sender
{
    if (progress >= 100.0)
    {
        if (self.senderForCallbackLoad) {
            [self.senderForCallbackLoad loadEnd:self];
        }
    }
    else
    {
        
        double tmpSize = alreadyDownloaded + [self->loader downloadedSize]/1024.0/1024.0;
        _mbytesDowloaded = tmpSize;
        //[_table reloadData];
      //  NSLog(@"downloaded: %f", _mbytesDowloaded);
       if (self.senderForCallbackLoad)
           [self.senderForCallbackLoad update:self];
     //  if ( ![_table isHidden] )
       //  [_table reloadSections:[NSIndexSet indexSetWithIndex:_section] withRowAnimation:UITableViewRowAnimationNone];

    }
}

-(void)StartDownloadWithSender:(id<DownloadableObjectDelegate>)sender
{
   if (!self->loader)
   {
       self->loader = [[Loader alloc]init];
   }
    _senderForCallbackLoad = sender;
   self->loader.delegate = self;
   [self->loader load:_paths];
};

-(void)CancelDownload
{
    if (loader)
    {
        [loader cancel];
        [self RefreshWithData:_paths];
    }
    
};

-(double)GetApproxSize
{
    long long byteSize = 0;
    int count = 0;
    MainController *myController = [MainController sharedController];
    
    for (int i = 0; i < [_paths count]; i++)
    {
        if ([[myController.dataSize valueForKey:_paths[i]] integerValue] > 0)
        {
            count++;
        }
        else
        {
            NSLog(@"path %@ not found", _paths[i]);
        }
        byteSize += [[myController.dataSize valueForKey:_paths[i]] integerValue];
    }
    
    double size = (double)byteSize/1024.0/1024.0;
    NSLog(@"SIZE: %i %lu", count, (unsigned long)[_paths count]);
    return  size;
}

-(void)RefreshWithData:(NSMutableArray*)links
{
    _mbytesDowloaded = 0.0;
     _paths = links;
    long long byteSize = 0;
    MainController *myController = [MainController sharedController];
    for (int i = 0; i < [_paths count]; i++)
    {
        if ([CacheControll isCachedAtPath:_paths[i]])
        {
            byteSize += [[myController.dataSize valueForKey:_paths[i]] integerValue];
        }
            
    }
    alreadyDownloaded = (double)byteSize/1024.0/1024.0;
    _size = [self GetApproxSize];
    if (loader)
    {
        _mbytesDowloaded = alreadyDownloaded - [self->loader downloadedSize]/1024.0/1024.0;
    }
    else
    {
        _mbytesDowloaded = alreadyDownloaded;
    }
}

@end
