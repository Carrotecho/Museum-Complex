//
//  MainController.h
//  UINew
//
//  Created by  Roman on 24.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#define EXHIBITS_TOTAL_NUM 21

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import "DataHolder.h"
#import "Socials.h"
#include "pugixml.hpp"

#import "LocalizationSystem.h"
#import "CacheControll.h"
#import "Loader.h"
#import "DataSenderServer.h"
#import "DownloadableObject.h"

#import <Crashlytics/Crashlytics.h>



#define MAX_OBJECTS_IDS 2000

@protocol MainControllerDelegate <NSObject>

-(void)loadEnd;

@end


enum exhibitState
{
	NOT_VISITED,
	VISITED,
	LAST_VISITED
};

@interface Exhibit: NSObject
{
	@public
		NSString *Id;
		enum exhibitState state;
		CGVector coord;
		NSInteger floor;
		NSString *caption;
		NSString *parentId;
        NSString *icon;
        BOOL hasNestedObjects;
        BOOL hasMarker;
        BOOL isUnesco;
        NSString *markerId;
        NSString *details;
}

@end

@interface MainController : NSObject<LoaderDelegate>
{
    DataSenderServer*senderRequest;
	NSString *objectId;
	NSString *audio;
	NSString *excursionMode;//main, audio, 3D, 3DAudio
	NSString *caption;
	NSString *text;
	NSArray *descriptions;
	NSArray *images;
	NSArray *objectList;
	NSInteger index;
	NSInteger currentLang;
	NSInteger currentFloor;
	NSArray *mapImages;
	CGVector mapCoords;
	NSArray *globalObjectList;
    NSMutableArray *objectStack;
	BOOL hasNestedObjects;
	DataHolder*		holder;
	Socials*		socials;
    NSString        *details;
    bool isXMLContentLoaded;
    
    UIView*backgroundProgress;
    UIProgressView* progressLoadedContent;
    pugi::xml_document contentXMLObj;
    NSMutableArray* linksToDownload;
    
    bool fastMarkerObject[MAX_OBJECTS_IDS];
    
}

@property (nonatomic, retain) id<MainControllerDelegate> senderForCallbackLoad;
@property (nonatomic, retain) NSString *serverPath;
@property (nonatomic, retain) UINavigationController* mainNavController;

@property (nonatomic, retain) NSString *objectId;
@property (nonatomic, retain) NSString *prevObjectId;
@property (nonatomic, retain) NSString *parentId;
@property (nonatomic) BOOL isUnesco;
@property (nonatomic, retain) NSString *audio;
@property (nonatomic, retain) NSString *excursionMode;//main, audio, 3D, 3DAudio
@property (nonatomic, retain) NSArray *descriptions;
@property (nonatomic, retain) NSString *caption;
@property (nonatomic, retain) NSString *text;
@property (nonatomic, retain) NSString *details;
@property (nonatomic, retain) NSArray *imagesO;
@property (nonatomic, retain) NSArray *images;
@property (nonatomic, retain) NSArray *mapImages;
@property (nonatomic, retain) NSArray *objectList;
@property (nonatomic, retain) NSArray *globalObjectList;
@property (nonatomic) NSInteger lastDetectedIndex; // id of object detected by Vuforia
@property (nonatomic, retain) NSString *detectedId;
@property (nonatomic) CGVector mapCoords;
@property (nonatomic) NSInteger index;
@property (nonatomic) NSInteger currentFloor;
@property (nonatomic) NSInteger currentLang;
@property (nonatomic) NSInteger tempIndex;
@property (nonatomic) NSMutableArray *objectStack;
@property (nonatomic) BOOL hasNestedObjects;
@property (nonatomic, retain) Socials* socials;
@property (nonatomic, retain) NSMutableDictionary *dataSize;
@property (nonatomic, retain) NSMutableDictionary *markerIdObjectId;
@property (atomic, retain) NSMutableArray *downloadObjects;

-(void)retrieve3Dlinks;
-(void)loadFiles:(NSArray*)paths sender:(id<MainControllerDelegate>)sender;
-(bool*)getChecker;
-(void)createXMlContent:(NSString*)textXML;
-(void)ObjectIdUpdated:(id<MainControllerDelegate>)sender; //TBD
-(void)FillGlobalObjectList;
-(void)UpdateObjectStatus;
-(void)FillObjectList;
-(void)PushToStack: (NSString*) objId;
-(BOOL) checkIsChildObject: (NSInteger)Id;
-(NSMutableArray*)getAllResourcesForID:(NSString*)idNeedle;
-(void)retrieveMarkerId;
-(BOOL)Determine3DStatus;
+(id)sharedController;

@end
