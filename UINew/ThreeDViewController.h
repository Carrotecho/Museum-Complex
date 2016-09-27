//
//  3DViewController.h
//  UINew
//
//  Created by  Roman on 04.10.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "GLView.h"
#import <Crashlytics/Crashlytics.h>

#import "Loader.h"

#import "SingltonForVuforia.h"
#include <Social/Social.h>

#import "DataSenderServer.h"
#import "MainController.h"

@interface ThreeDViewController : UIViewController<UIAlertViewDelegate,StabilizationDelegate,UIPopoverControllerDelegate,GLViewDelegate,LoaderDelegate,GLViewDelegate,MainControllerDelegate> {
    GLView* glView;
    
    DataSenderServer*senderRequest;
    
    IBOutlet UIButton * menuButton;
    IBOutlet UIButton* secondViewButton;
    NSString*_lastDataPath;
    NSString*_lastmarkerPrefix;
    
    CGRect viewFrame;
    uint currentAtempInitAR,maxAtemptInitAR;
    
    id backgroundObserver;
    id activeObserver;
    UITapGestureRecognizer * tapGestureRecognizer;
    
    NSMutableArray*linksToDownload;
    
    // vuforia only
    UIView*backgroundProgress;
    
    UIProgressView*progressLoadedContent;
    NSString*serverPath;
    NSString*appPath;
    NSString*basePath;
    bool isARInited;
    
    UIImageView*imageHintsZoom;
    UIImageView*imageHintsUPDown;
    UIImageView*imageHintsLeftRight;
    
    UIView*hintsLayer;
    
    bool marker2dDetectionAllowed;
    
}
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;
- (IBAction)btnMenuPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UIButton *btnContacts;
@property (weak, nonatomic) IBOutlet UIButton *btnRemind;
@property (weak, nonatomic) IBOutlet UIButton *btnMap;
@property (weak, nonatomic) IBOutlet UIButton *btnToMainMenu;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;
@property (weak, nonatomic) IBOutlet UIView *vwMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnPhoto;

- (IBAction)btnPhotoPressed:(id)sender;
@property(nonatomic,readwrite) float aX;
@property(nonatomic,readwrite) float aY;
@property(nonatomic,readwrite) float aZ;
@property (weak, nonatomic) IBOutlet UIButton *btnInstagram;
@property (weak, nonatomic) IBOutlet UIButton *btnVK;
@property (weak, nonatomic) IBOutlet UIButton *btnFB;
@property (weak, nonatomic) IBOutlet UIButton *btnTwitter;

@property(nonatomic,readwrite) float gX;
@property(nonatomic,readwrite) float gY;
@property(nonatomic,readwrite) float gZ;

@property(nonatomic,readwrite) float spaceAcc;
@property(nonatomic,readwrite) float spaceGyro;

@property(nonatomic,readwrite) float currentStableFrames;
@property(nonatomic,readwrite) float maxStableFrames;

@property (weak, nonatomic) IBOutlet UIButton *btnShare;

- (IBAction)btnSharePressed:(id)sender;
@property (nonatomic, strong) CMMotionManager *motionManager;
@property(nonatomic,readwrite) NSString* detectedObjectID;
@property (weak, nonatomic) IBOutlet UIView *vwSocials;

@property int activeIndex;

@property (weak, nonatomic) IBOutlet UIButton *btnBack;
- (IBAction)btnBackPressed:(id)sender;
- (IBAction)btnInfoPressed:(id)sender;
- (IBAction)btnInstagramPressed:(id)sender;
- (IBAction)btnVKPressed:(id)sender;
- (IBAction)btnFBPressed:(id)sender;
- (IBAction)btnTwitterPressed:(id)sender;
-(void)UpdateButtons;
-(void)indexInObjectChanged:(int)index;
@property (weak, nonatomic) IBOutlet UIButton *btnInfo;


@end
