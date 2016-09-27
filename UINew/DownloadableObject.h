//
//  DownloadableObject.h
//  dwldDemo
//
//  Created by  Roman on 04.12.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Loader.h"

enum downloadableStatus
{
    DOWNLOADED,
    NOT_DOWNLOADED,
    STOPPED,
    DOWNLOADING
};
@protocol DownloadableObjectDelegate <NSObject>

-(void)loadEnd:(id)sender;
-(void)update:(id)sender;

@end

@interface DownloadableObject : NSObject<LoaderDelegate>
{
    Loader *loader;
    double alreadyDownloaded;
}

@property NSString *name;
@property float mbytesDowloaded;
@property double size;
@property enum downloadableStatus status;
@property NSMutableArray *paths;
@property (nonatomic, retain) id<DownloadableObjectDelegate> senderForCallbackLoad;

@property  (retain, strong) UITableView* table;
@property int section;

-(id)initWithData:(NSMutableArray*)links;
-(void)StartDownloadWithSender:(id<DownloadableObjectDelegate>)sender;
-(void)CancelDownload;
-(double)GetApproxSize;
-(void)RefreshWithData:(NSArray*)_links;


@end
