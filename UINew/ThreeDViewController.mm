//
//  3DViewController.m
//  UINew
//
//  Created by  Roman on 04.10.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "ThreeDViewController.h"

#import "LocalizationSystem.h"

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>




#include <QuartzCore/QuartzCore.h>

#import <QCAR/QCAR.h>
#import <QCAR/Renderer.h>
#import <QCAR/State.h>
#import <QCAR/TrackerManager.h>
#import <QCAR/Trackable.h>
#import <QCAR/DataSet.h>
#import <QCAR/CameraDevice.h>
#import <QCAR/ObjectTracker.h>

#import "pugixml.hpp"

#import "SampleApplicationSession.h"
#import "CacheControll.h"


@interface ThreeDViewController ()<SampleApplicationControl> {
    GLuint _program;
    
    float _rotation;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    QCAR::DataSet* dataSetCurrent;
    QCAR::DataSet* buildingsForPresent;
    SampleApplicationSession* vapp;
}


@end

@implementation ThreeDViewController

GLuint defaultFramebuffer, colorRenderbuffer,depthRenderbuffer;
- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        [CacheControll sharedInstance].isInternetActive = [self avalibleDownloadFiles];
        
        [NSTimer scheduledTimerWithTimeInterval:5 target:self selector:@selector(checkInternet:) userInfo:nil repeats:YES];
        
        
        maxAtemptInitAR = 2;
        
        
        serverPath = @"http://f.devar.org/";
        //serverPath = @"http://192.168.200.5/ultravisionTest/";
        
        
        // Custom initialization
        
        
        SingltonForVuforia*sss = [SingltonForVuforia sharedManager];
        
        
        
        
        
        
        linksToDownload = [[NSMutableArray alloc] init];
        
        
        
        
        if (sss.sessionApp) {
            vapp = sss.sessionApp;
            vapp.delegate = self;
        }else {
            vapp = [[SampleApplicationSession alloc] initWithDelegate:self];
            sss.sessionApp = vapp;
            [self playScene:[serverPath stringByAppendingFormat:@"bases/novgorod/test_novgorod.xml"] markerPrefix:@"/novgorod/"];
        }
        
        senderRequest = [[DataSenderServer alloc] init];
        
        
        
        
        CGRect screenBounds = [[UIScreen mainScreen] bounds];
        viewFrame = screenBounds;
        
        
        dataSetCurrent = nil;
        
        tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(autofocus:)];
        
        
        imageHintsZoom = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"imageHintsZoom"]];
        imageHintsUPDown = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"imageHintsUPDown"]];
        imageHintsLeftRight = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"imageHintsLeftRight"]];
        
        imageHintsZoom.userInteractionEnabled = NO;
        imageHintsUPDown.userInteractionEnabled = NO;
        imageHintsLeftRight.userInteractionEnabled = NO;
        
        
        hintsLayer = nullptr;
        
        progressLoadedContent = nil;
        
    }
    return self;
}


-(void)gettedContentLinks:(NSArray*)data {
    [linksToDownload removeAllObjects];
    
    NSString*paths = [data objectAtIndex:0];
    
    NSArray *myWords = [paths componentsSeparatedByCharactersInSet:
                        [NSCharacterSet characterSetWithCharactersInString:@":"]
                        ];
    
    for (int i = 0; i < [myWords count]; i++) {
        
        NSString*realPath = [serverPath stringByAppendingString:[myWords objectAtIndex:i]];
        NSLog(@"%@", realPath);
        if (![CacheControll isCachedAtPath:realPath]) {
            [linksToDownload addObject:realPath];
        }
    }
    /*
    if ([linksToDownload count] > 0) {
        
        UIAlertView *successAlert = [[UIAlertView alloc] initWithTitle:AMLocalizedString(@"Message", @"")
                                                               message:AMLocalizedString(@"load_all_content", @"")
                                                              delegate:self
                                                     cancelButtonTitle:AMLocalizedString(@"Partial", @"")
                                                     otherButtonTitles:AMLocalizedString(@"Full", @""),nil];
        [successAlert show];
        
        
        
    }*/
    
    
}


// loader delegate
-(void)updateLoaderDelegate:(double)progress sender:(id)sender {
    // content loaded;
    if (progress >= 100.0) {
        [backgroundProgress removeFromSuperview];
        [progressLoadedContent removeFromSuperview];
    }else {
        float progressF = progress/100.0;
        [progressLoadedContent setProgress:progressF];
    }
    
    
    
}

-(bool)taskWithError:(NSURL*)errorurl {
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [backgroundProgress removeFromSuperview];
        [progressLoadedContent removeFromSuperview];
        [self.navigationController popViewControllerAnimated:YES];
    });
    return false;
}

-(void)failGettedContentLinks:(id)data {
    
}

-(NSString*)createReqString:(NSString*)req data:(NSArray*)data {
    NSString * baseString = [NSString stringWithFormat:@"%@/contentGetter.php?request=%@",serverPath,req];
    if (data) {
        NSMutableString *s = [[NSMutableString alloc] initWithString:baseString] ;
        for (int i = 0; i < [data count]; i++) {
            
            NSString*param = [data objectAtIndex:i];
            
            NSCharacterSet *set = [NSCharacterSet URLQueryAllowedCharacterSet];
            NSString *result = [param stringByAddingPercentEncodingWithAllowedCharacters:set];
            
            
            [s appendFormat:@"&d%d=%@",i,result];
        }
        
        return s;
    }
    return baseString;
}



-(void)checkInternet:(NSTimer*)timer {
    [CacheControll sharedInstance].isInternetActive = [self avalibleDownloadFiles];
}


-(bool)avalibleDownloadFiles {
    
    NSArray *subviews = [[[[UIApplication sharedApplication] valueForKey:@"statusBar"] valueForKey:@"foregroundView"]subviews];
    NSNumber *dataNetworkItemView = nil;
    
    for (id subview in subviews) {
        if([subview isKindOfClass:[NSClassFromString(@"UIStatusBarDataNetworkItemView") class]]) {
            dataNetworkItemView = subview;
            break;
        }
    }
    
    switch ([[dataNetworkItemView valueForKey:@"dataNetworkType"]integerValue]) {
        case 0:
            return false;
            break;
        case 1:
            return true;
            break;
        case 2:
            return true;
            break;
        case 3:
            return true;
            break;
        case 4:
            return true;
            break;
        case 5:
            return true;
            break;
        default:
            break;
    }
    return false;
}





-(void)changeEditMode:(UISegmentedControl *)paramSender {
    //get index position for the selected control
    NSInteger selectedIndex = [paramSender selectedSegmentIndex];
    
    
    glView.editorMode = selectedIndex;
    
    
}


-(void)clearEditorChange:(id)sender {
    [glView clearEditorChange];
}



-(void)focusAdjustiong {
    self.currentStableFrames = 0;
}




-(void)reloadResource {
    if (glView && [glView respondsToSelector:@selector(reloadResource)]) {
        [glView reloadResource];
    }
}


-(void)loadingResource {
    [self showLoadingAnimationEngine];
}

-(void)loadingResourceEnd {
    [self hideLoadingAnimationEngine];
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    
    [glView toucheEnd:touches withEvent:event];
    
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    printf("begin\n");
}
-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    printf("moved\n");
}
-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    printf("end\n");
    [glView toucheEnd:touches withEvent:event];
}


- (void)viewWillDisappear:(BOOL)animated {
  //   glView.needClearScene = true;
    MainController *myController = [MainController sharedController];
     self->marker2dDetectionAllowed = false;
    _vwMenu.hidden = YES;
    
    [vapp pauseAR:nil];
    
    [[NSNotificationCenter defaultCenter] removeObserver:backgroundObserver];
    [[NSNotificationCenter defaultCenter] removeObserver:activeObserver];
}


-(void)viewDidAppear:(BOOL)animated {
    
    self.title = @"3DViewCntrlr";
     self->marker2dDetectionAllowed = true;
     glView.needClearScene = true;
    
    if (isARInited) {
        [vapp resumeAR:nil];
    }else {
        currentAtempInitAR = 0;
        [NSTimer timerWithTimeInterval:5 target:self selector:@selector(checkInitAR:) userInfo:nil repeats:YES];
        [vapp initAR:QCAR::GL_20 ARViewBoundsSize:viewFrame.size orientation:UIInterfaceOrientationPortrait];
    }
}




-(void)checkInitAR:(NSTimer *)timer {
    if (isARInited) {
        [timer invalidate];
    }else {
        if (currentAtempInitAR > maxAtemptInitAR) {
            [timer invalidate];
            [CrashlyticsKit logEvent:@"error" attributes:@{@"errorCode": @"1",@"errorMsg": @"max atempt to initAR reached"}];
        }else {
            currentAtempInitAR++;
        }
    }
}







#pragma VUFORIA zone


- (bool) doInitTrackers {
    // Initialize the image or marker tracker
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    
    // Image Tracker...
    QCAR::Tracker* trackerBase = trackerManager.initTracker(QCAR::ObjectTracker::getClassType());
    if (trackerBase == NULL)
    {
        NSLog(@"Failed to initialize ImageTracker.");
        return false;
    }
    NSLog(@"Successfully initialized ImageTracker.");
    return true;
}



-(void)loadResources:(NSArray*)objects {
    Loader*loader = [[Loader alloc] init];
    loader.delegate = self;
    [loader load:objects];
}
-(void)updateEnd {
    [self updateLoaderDelegate:100 sender:self];
}



-(void)sendLoadTrackers:(NSArray*)data {
    
    
}



-(void)playScene:(NSString *)dataPath markerPrefix:(NSString*)markerPrefix{
    
    
    if (isARInited) {
        
        
        [glView loadImageTrackerDataSet:[CacheControll getPathFromCacheAtPath:dataPath] markerPrefix:markerPrefix];
    }else {
        _lastDataPath = dataPath;
        _lastmarkerPrefix = markerPrefix;
        
    }
}






- (bool) doLoadTrackersData {
    
    NSString*pathBaseCache = [serverPath stringByAppendingString:basePath];
    if (![CacheControll isCachedAtPath:pathBaseCache]) {
        NSError* error = nil;
        NSData*data = [NSData dataWithContentsOfURL:[NSURL URLWithString:pathBaseCache] options: 0 error: &error];
        if (data == nil ) {
            NSLog(@"Failed to read file, error %@ ", error);
        }else {
            [CacheControll cacheData:data fromServerPath:pathBaseCache];
        }
    }
    
    NSString*pathBaseCache2 = [pathBaseCache stringByReplacingOccurrencesOfString:@".xml" withString:@".dat"];
    
    if (![CacheControll isCachedAtPath:pathBaseCache2]) {
        NSError* error2 = nil;
        NSData*data2 = [NSData dataWithContentsOfURL:[NSURL URLWithString:pathBaseCache2] options: 0 error: &error2];
        if (data2 == nil) {
            NSLog(@"Failed to read file, error %@",error2);
        }else {
            [CacheControll cacheData:data2 fromServerPath:pathBaseCache2];
        }
    }
    
    
    
    return YES;
    
}


- (bool) doStartTrackers {
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::Tracker* tracker = trackerManager.getTracker(QCAR::ObjectTracker::getClassType());
    if(tracker == 0) {
        return NO;
    }
    
    tracker->start();
    return YES;
}

// callback: the AR initialization is done
- (void) onInitARDone:(NSError *)initError {
    [self hideLoadingAnimation];
    
    if (initError == nil) {
        NSError * error = nil;
        [vapp startAR:QCAR::CameraDevice::CAMERA_BACK error:&error];
        
        [glView setCameraProjectionMatrix:&vapp.projectionMatrix.data[0]];
        // by default, we try to set the continuous auto focus mode
        // and we update menu to reflect the state of continuous auto-focus
        QCAR::CameraDevice::getInstance().setFocusMode(QCAR::CameraDevice::FOCUS_MODE_CONTINUOUSAUTO);
        isARInited = true;
        [glView onInitARDone]; // fill vars, very important
        
        // load current dataset -
        [glView loadImageTrackerDataSet:[CacheControll getPathFromCacheAtPath:_lastDataPath] markerPrefix:_lastmarkerPrefix];
    } else {
        isARInited = false;
        NSLog(@"Error initializing AR:%@", [initError description]);
    }
}



- (void) onQCARUpdate: (QCAR::State *) state {
    
}

// Load the image tracker data set
- (QCAR::DataSet *)loadImageTrackerDataSet:(NSString*)dataFile
{
    NSLog(@"loadImageTrackerDataSet (%@)", dataFile);
    QCAR::DataSet * dataSet = NULL;
    
    // Get the QCAR tracker manager image tracker
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::ObjectTracker* imageTracker = static_cast<QCAR::ObjectTracker*>(trackerManager.getTracker(QCAR::ObjectTracker::getClassType()));
    
    if (NULL == imageTracker) {
        NSLog(@"ERROR: failed to get the ImageTracker from the tracker manager");
        return NULL;
    } else {
        dataSet = imageTracker->createDataSet();
        
        if (NULL != dataSet) {
            NSLog(@"INFO: successfully loaded data set");
            
            // Load the data set from the app's resources location
            if (!dataSet->load([dataFile cStringUsingEncoding:NSASCIIStringEncoding], QCAR::DataSet::STORAGE_ABSOLUTE)) {
                NSLog(@"ERROR: failed to load data set");
                imageTracker->destroyDataSet(dataSet);
                dataSet = NULL;
            }
        }
        else {
            NSLog(@"ERROR: failed to create data set");
        }
    }
    
    
    
    //[glView setDataSet:dataSet];
    return dataSet;
}


- (bool) doStopTrackers {
    // Stop the tracker
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::Tracker* tracker = trackerManager.getTracker(QCAR::ObjectTracker::getClassType());
    
    if (NULL != tracker) {
        tracker->stop();
        NSLog(@"INFO: successfully stopped tracker");
        return YES;
    }
    else {
        NSLog(@"ERROR: failed to get the tracker from the tracker manager");
        return NO;
    }
}

- (bool) doUnloadTrackersData {
    [self deactivateDataSet: dataSetCurrent];
    dataSetCurrent = nil;
    return YES;
}

- (BOOL)activateDataSet:(QCAR::DataSet *)theDataSet
{
    // if we've previously recorded an activation, deactivate it
    if (dataSetCurrent != nil)
    {
        [self deactivateDataSet:dataSetCurrent];
    }
    BOOL success = NO;
    
    // Get the image tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::ObjectTracker* imageTracker = static_cast<QCAR::ObjectTracker*>(trackerManager.getTracker(QCAR::ObjectTracker::getClassType()));
    
    if (imageTracker == NULL) {
        NSLog(@"Failed to load tracking data set because the ImageTracker has not been initialized.");
    }
    else
    {
        // Activate the data set:
        if (!imageTracker->activateDataSet(theDataSet))
        {
            NSLog(@"Failed to activate data set.");
        }
        else
        {
            NSLog(@"Successfully activated data set.");
            dataSetCurrent = theDataSet;
            success = YES;
        }
    }
    
    // we set the off target tracking mode to the current state
    if (success) {
        [self setExtendedTrackingForDataSet:dataSetCurrent start:YES];
    }
    
    return success;
}

- (BOOL)deactivateDataSet:(QCAR::DataSet *)theDataSet
{
    if ((dataSetCurrent == nil) || (theDataSet != dataSetCurrent))
    {
        NSLog(@"Invalid request to deactivate data set.");
        return NO;
    }
    
    BOOL success = NO;
    
    // we deactivate the enhanced tracking
    [self setExtendedTrackingForDataSet:theDataSet start:NO];
    
    // Get the image tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::ObjectTracker* imageTracker = static_cast<QCAR::ObjectTracker*>(trackerManager.getTracker(QCAR::ObjectTracker::getClassType()));
    
    if (imageTracker == NULL)
    {
        NSLog(@"Failed to unload tracking data set because the ImageTracker has not been initialized.");
    }
    else
    {
        // Activate the data set:
        if (!imageTracker->deactivateDataSet(theDataSet))
        {
            NSLog(@"Failed to deactivate data set.");
        }
        else
        {
            success = YES;
        }
    }
    
    dataSetCurrent = nil;
    
    return success;
}

- (BOOL) setExtendedTrackingForDataSet:(QCAR::DataSet *)theDataSet start:(BOOL) start {
    BOOL result = YES;
    for (int tIdx = 0; tIdx < theDataSet->getNumTrackables(); tIdx++) {
        QCAR::Trackable* trackable = theDataSet->getTrackable(tIdx);
        if (start) {
            if (!trackable->startExtendedTracking())
            {
                NSLog(@"Failed to start extended tracking on: %s", trackable->getName());
                result = false;
            }
        } else {
            if (!trackable->stopExtendedTracking())
            {
                NSLog(@"Failed to stop extended tracking on: %s", trackable->getName());
                result = false;
            }
        }
    }
    return result;
}

- (bool) doDeinitTrackers {
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    trackerManager.deinitTracker(QCAR::ObjectTracker::getClassType());
    return YES;
}

- (void)autofocus:(UITapGestureRecognizer *)sender
{
    //[self performSelector:@selector(cameraPerformAutoFocus) withObject:nil afterDelay:.4];
}

- (void)cameraPerformAutoFocus
{
    //QCAR::CameraDevice::getInstance().setFocusMode(QCAR::CameraDevice::FOCUS_MODE_TRIGGERAUTO);
}





-(void)loadingResourceProgressBar {
    [self performSelectorOnMainThread:@selector(showAnimationEngineProgress) withObject:nil waitUntilDone:NO];
}

-(void)loadingResourceProgressBarEnd {
    [self performSelectorOnMainThread:@selector(hideAnimationEngineProgress) withObject:nil waitUntilDone:NO];
    
}

-(void)updateProgress:(double)progress {
    
    [self performSelectorOnMainThread:@selector(updateAnimationEngineProgress:) withObject:[NSNumber numberWithDouble:progress] waitUntilDone:NO];
}



-(void)showAnimationEngineProgress {
    UIProgressView*loadingIndicator = [[UIProgressView alloc] initWithProgressViewStyle:UIProgressViewStyleDefault];
    loadingIndicator.progress = 0;
    [loadingIndicator setFrame:CGRectMake(self.view.frame.size.width/2 - 100, self.view.frame.size.height/2-25, 200, 50)];
    loadingIndicator.tag  = 222;
    [glView addSubview:loadingIndicator];
}

- (void) hideAnimationEngineProgress {
    UIProgressView *loadingIndicator = (UIProgressView *)[self.view viewWithTag:222];
    [loadingIndicator removeFromSuperview];
}



-(void)updateAnimationEngineProgress:(NSNumber*)progress {
    UIProgressView *loadingIndicator = (UIProgressView *)[self.view viewWithTag:222];
    loadingIndicator.progress = [progress doubleValue];
}



-(void)showAnimationEngine {
    CGRect mainBounds = [[UIScreen mainScreen] bounds];
    CGRect indicatorBounds = CGRectMake(mainBounds.size.width / 2 - 12,
                                        mainBounds.size.height / 2 - 12, 24, 24);
    UIActivityIndicatorView *loadingIndicator = [[UIActivityIndicatorView alloc]
                                                 initWithFrame:indicatorBounds];
    
    loadingIndicator.tag  = 111;
    loadingIndicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyleWhiteLarge;
    
    
    [glView addSubview:loadingIndicator];
    [loadingIndicator startAnimating];
}

-(void)hideAnimationEngine {
    UIActivityIndicatorView *loadingIndicator = (UIActivityIndicatorView *)[glView viewWithTag:111];
    [loadingIndicator removeFromSuperview];
}

- (void)showLoadingAnimationEngine {
    [self performSelectorOnMainThread:@selector(showAnimationEngine) withObject:nil waitUntilDone:NO];
}
-(void)hideLoadingAnimationEngine {
    [self performSelectorOnMainThread:@selector(hideAnimationEngine) withObject:nil waitUntilDone:NO];
    
}

- (void) showLoadingAnimation {
    CGRect mainBounds = [[UIScreen mainScreen] bounds];
    CGRect indicatorBounds = CGRectMake(mainBounds.size.width / 2 - 12,
                                        mainBounds.size.height / 2 - 12, 24, 24);
    UIActivityIndicatorView *loadingIndicator = [[UIActivityIndicatorView alloc]
                                                 initWithFrame:indicatorBounds];
    
    loadingIndicator.tag  = 112;
    loadingIndicator.activityIndicatorViewStyle = UIActivityIndicatorViewStyleWhiteLarge;
    
    
    self.view.backgroundColor = [UIColor blackColor];
    
    [self.view addSubview:loadingIndicator];
    [loadingIndicator startAnimating];
}

- (void) hideLoadingAnimation {
    UIActivityIndicatorView *loadingIndicator = (UIActivityIndicatorView *)[self.view viewWithTag:112];
    [loadingIndicator removeFromSuperview];
}



- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    
    
    // Do any additional setup after loading the view.
    MainController *myController = [MainController sharedController];
    switch (myController.currentLang)
    {
        case 0:
            LocalizationSetLanguage(@"ru");
            break;
        case 1:
            LocalizationSetLanguage(@"en");
            break;
    }
    
    [self UpdateButtons];
    
    SingltonForVuforia*sss = [SingltonForVuforia sharedManager];
    
    if (sss.glView) {
        glView = sss.glView;
        isARInited = true;
    }else {
        glView = [[GLView alloc] initWithFrame:viewFrame secondStateButton:secondViewButton serverPath:serverPath];
        sss.glView = glView;
        isARInited = false;
        [self showLoadingAnimation];
    }
    

    
    
    
  
    [self.view insertSubview:glView atIndex:0];
    
    glView.lastMarkerId = 0;
    
    glView.delegate = self;
    
    [self.view addGestureRecognizer:tapGestureRecognizer];
    
    
    
    [glView addSubview:secondViewButton];
    glView.stabilizationStable = self;
    
    // show loading animation while AR is being initialized
    
    
    
    backgroundObserver = [[NSNotificationCenter defaultCenter]
                          addObserverForName:UIApplicationWillResignActiveNotification
                          object:nil
                          queue:nil
                          usingBlock:^(NSNotification *note) {
                              dispatch_async(dispatch_get_main_queue(), ^{
                                  
                                  [self popoverPresentationController];
                                  
                                  NSError * error = nil;
                                  //if (![vapp pauseAR:&error]) {
                                  //    NSLog(@"Error pausing AR:%@", [error description]);
                                  //}
                              });
                              
                          } ];
    
    activeObserver = [[NSNotificationCenter defaultCenter]
                      addObserverForName:UIApplicationDidBecomeActiveNotification
                      object:nil
                      queue:nil
                      usingBlock:^(NSNotification *note) {
                          dispatch_async(dispatch_get_main_queue(), ^{
                              NSError * error = nil;
                              
                              
                              if(! [vapp resumeAR:&error]) {
                                  NSLog(@"Error resuming AR:%@", [error description]);
                              }
                              // on resume, we reset the flash and the associated menu item
                              QCAR::CameraDevice::getInstance().setFlashTorchMode(false);
                          });
                          
                      } ];
    
    
    
    /*
    
    UIImageView*titleBG = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"title-main"]];
    [self.view addSubview:titleBG];
    
    
    UIButton* menuTitleButton = [UIButton buttonWithType:UIButtonTypeCustom];
    menuTitleButton.showsTouchWhenHighlighted = YES;
    menuTitleButton.adjustsImageWhenHighlighted = YES;
    
    
    CGRect frame ;
    
    [self.view addSubview:menuTitleButton];
    
    
    UIButton* clearEditorChange = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    clearEditorChange.showsTouchWhenHighlighted = YES;
    clearEditorChange.adjustsImageWhenHighlighted = YES;
    [clearEditorChange setTitle:@"clear" forState:UIControlStateNormal];
    
    
    [clearEditorChange addTarget:self action:@selector(clearEditorChange:) forControlEvents:UIControlEventTouchUpInside];
    
    frame = clearEditorChange.frame;
    frame.size.width = 50;
    frame.size.height = 50;
    frame.origin.y = 0;
    frame.origin.x = 0;
    [clearEditorChange setFrame:frame];
    
    [self.view addSubview:clearEditorChange];
    
    
    UISegmentedControl* editorChoise = [[UISegmentedControl alloc] initWithItems:[[NSArray alloc] initWithObjects: @"off",@"mX",@"mZ",@"mY",@"mF",@"rX",@"rY",@"rZ",@"scale",@"scaleF",nil]];
    editorChoise.frame = CGRectMake(self.view.frame.size.width-300, 50.0f, 300.0f, 40.0f);
    [editorChoise setSelectedSegmentIndex:0];
    editorChoise.backgroundColor = [UIColor whiteColor];
    [editorChoise addTarget:self action:@selector(changeEditMode:)
           forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:editorChoise];
    
    
    

[senderRequest send:[self createReqString:@"" data:nil] callBackTarget:self callBackAction:@selector(gettedContentLinks:) callBackActionFail:@selector(failGettedContentLinks:) succesParrams:[NSNumber numberWithInt:1] cookies:nil];
*/
    
    if (hintsLayer == nullptr) {
        hintsLayer = [[UIView alloc] initWithFrame:self.view.frame];
        hintsLayer.userInteractionEnabled = NO;
    }
    
    
    float delta = self.view.frame.size.width / 1536.0;
    
    
    CGRect frame = imageHintsZoom.frame;
    frame.size.height*=delta;
    frame.size.width*=delta;
    
    [imageHintsZoom setFrame:frame];
    
    frame = imageHintsUPDown.frame;
    frame.size.height*=delta;
    frame.size.width*=delta;
    
    [imageHintsUPDown setFrame:frame];
    
    frame = imageHintsLeftRight.frame;
    frame.size.height*=delta;
    frame.size.width*=delta;
    
    [imageHintsLeftRight setFrame:frame];
    
    
    [self.view addSubview:hintsLayer];
    
    
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
    
    if (buttonIndex == 0) {
        
        return;
    }
    
    
    
    backgroundProgress = [[UIView alloc] initWithFrame:self.view.frame];
    backgroundProgress.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.3];
    [self.view addSubview:backgroundProgress];
    
    progressLoadedContent = [[UIProgressView alloc] initWithProgressViewStyle:UIProgressViewStyleDefault];
    progressLoadedContent.progress = 0;
    [progressLoadedContent setFrame:CGRectMake(self.view.frame.size.width/2 - 100, self.view.frame.size.height/2-25, 200, 50)];
    [self.view addSubview:progressLoadedContent];
    
    Loader*loader = [[Loader alloc] init];
    loader.delegate = self;
    [loader load:linksToDownload];
    
    [linksToDownload removeAllObjects];

}


-(void)hideHint {
    dispatch_async(dispatch_get_main_queue(), ^{
        [imageHintsLeftRight removeFromSuperview];
        [imageHintsUPDown removeFromSuperview];
        [imageHintsZoom removeFromSuperview];
    });
}

-(void)showHint:(uint)idHint {
    
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [imageHintsLeftRight removeFromSuperview];
        [imageHintsUPDown removeFromSuperview];
        [imageHintsZoom removeFromSuperview];
        
        float degrees = 0; //the value in degrees
        imageHintsZoom.transform = CGAffineTransformMakeRotation(degrees * M_PI/180);
        imageHintsLeftRight.transform = CGAffineTransformMakeRotation(degrees * M_PI/180);
        
        CGRect frame;
        
        if (idHint == 1) {
            
            
            float degrees = 90; //the value in degrees
            imageHintsZoom.transform = CGAffineTransformMakeRotation(degrees * M_PI/180);
            imageHintsLeftRight.transform = CGAffineTransformMakeRotation(degrees * M_PI/180);

            
            
            [hintsLayer addSubview:imageHintsLeftRight];
            [hintsLayer addSubview:imageHintsZoom];
            
            //self.view.frame
            
            frame = _btnBack.frame;
            
            
            frame = imageHintsZoom.frame;
            
            frame.origin.y = self.view.frame.size.height/2 - (frame.size.height*2);
            frame.origin.x = imageHintsZoom.frame.size.width/3 ;
            
            [imageHintsZoom setFrame:frame];
            
            
            
            frame = imageHintsLeftRight.frame;
            frame.origin.y = self.view.frame.size.height/2  + imageHintsZoom.frame.size.height;
            frame.origin.x = imageHintsZoom.frame.size.width/3 ;
            
            [imageHintsLeftRight setFrame:frame];
            
            
        }else if (idHint == 2) {
            
            
            [hintsLayer addSubview:imageHintsUPDown];
            [hintsLayer addSubview:imageHintsZoom];
            
            //self.view.frame
            
            frame = _btnBack.frame;
            
            
            frame = imageHintsZoom.frame;
            
            frame.origin.x = self.view.frame.size.width/2 - (frame.size.width*2);
            frame.origin.y = _btnBack.frame.origin.y - frame.size.height - (_btnBack.frame.size.height/3);
            
            [imageHintsZoom setFrame:frame];
            
            
            
            frame = imageHintsUPDown.frame;
            frame.origin.x = self.view.frame.size.width/2  + imageHintsZoom.frame.size.width;
            frame.origin.y = _btnBack.frame.origin.y - frame.size.height - (_btnBack.frame.size.height/3);
            
            [imageHintsUPDown setFrame:frame];
            
        }else if (idHint == 3) {
            
            [hintsLayer addSubview:imageHintsLeftRight];
            [hintsLayer addSubview:imageHintsUPDown];
            [hintsLayer addSubview:imageHintsZoom];
            
            //self.view.frame
            
            frame = _btnBack.frame;
            
            
            frame = imageHintsLeftRight.frame;
            
            frame.origin.x = self.view.frame.size.width/2 - (imageHintsZoom.frame.size.width/2 + frame.size.width*2);
            frame.origin.y = _btnBack.frame.origin.y - frame.size.height - (_btnBack.frame.size.height/3);
            
            [imageHintsLeftRight setFrame:frame];
            
            
            
            frame = imageHintsZoom.frame;
            frame.origin.x = self.view.frame.size.width/2 - imageHintsZoom.frame.size.width/2;
            frame.origin.y = _btnBack.frame.origin.y - frame.size.height - (_btnBack.frame.size.height/3);
            
            [imageHintsZoom setFrame:frame];
            
            
            frame = imageHintsUPDown.frame;
            frame.origin.x = self.view.frame.size.width/2 + (imageHintsZoom.frame.size.width/2) + imageHintsLeftRight.frame.size.width;
            frame.origin.y = _btnBack.frame.origin.y - frame.size.height - (_btnBack.frame.size.height/3);
            
            [imageHintsUPDown setFrame:frame];
            
            
        }
    });
    
    
    
    
    
    
}



-(void)markerChanged:(uint)newMarkerID is3D:(bool)is3D {
    if (!is3D) {
        
    }
     MainController *myController = [MainController sharedController];
    _activeIndex = 0;
    
    NSString *value = [myController.markerIdObjectId valueForKey:[NSString stringWithFormat:@"object_%d",newMarkerID]];
    if (value)
    {
        self.detectedObjectID = value;
    }
    else
    {
        self.detectedObjectID = [NSString stringWithFormat:@"%d",newMarkerID];
    }
    myController.objectId = self.detectedObjectID;
    myController.detectedId = self.detectedObjectID;
    
    [myController UpdateObjectStatus];
    
    if (!is3D && self->marker2dDetectionAllowed)
    {
        [myController ObjectIdUpdated:self];
        self->marker2dDetectionAllowed = false;
        //[myController PushToStack:myController.objectId];
    }else {
        [myController ObjectIdUpdated:nil];
        //[myController PushToStack:myController.objectId];
    }   
}

-(void)loadEnd {
    MainController *myController = [MainController sharedController];
   // [myController PushToStack:myController.objectId];
    if (myController.hasNestedObjects)
    {
        Exhibit *exh = myController.objectList[_activeIndex];
        myController.objectId = exh->Id;
        self.detectedObjectID = myController.objectId;
        [myController ObjectIdUpdated:self];
        return;
    }
    self->marker2dDetectionAllowed = true;
    [self performSegueWithIdentifier:@"pushToInfo" sender:self];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

-(IBAction)btnBackPressed:(id)sender
{
    
    [self hideHint];
    
    MainController *myController = [MainController sharedController];
    
    myController.objectId = [myController.objectStack lastObject];
    if (myController.objectStack.count > 1)
    {
        [myController.objectStack removeLastObject];
    }
    [myController ObjectIdUpdated:nil];
    
    NSMutableArray *navigationArray = [[NSMutableArray alloc] initWithArray: self.navigationController.viewControllers];
    
    // [navigationArray removeAllObjects];
    for (int k = 0; k < [navigationArray count]; k++)
    {
        NSLog(@"navigation array %i %@", k, ((UIViewController*)navigationArray[k]).title);
    }
    
    int i = 0;
    for (; i < [navigationArray count]; i++)
    {
        NSLog(@"navigation array %i %@", i, ((UIViewController*)navigationArray[i]).title);
        NSString *title = ((UIViewController*)navigationArray[i]).title;
        if ([title isEqualToString:@"3DViewCntrlr"] || [title isEqual:nil])
            break;
    }
    
    [navigationArray removeObjectsAtIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(i, navigationArray.count-i)]];

    for (int k = 0; k < [navigationArray count]; k++)
    {
        NSLog(@"navigation array %i %@", k, ((UIViewController*)navigationArray[k]).title);
    }
    self.navigationController.viewControllers = navigationArray;
    
	[self.navigationController popViewControllerAnimated:YES];
}

-(IBAction)btnInfoPressed:(id)sender
{
	//need to set objectId to detected one

	MainController *myController = [MainController sharedController];
	myController.objectId = self.detectedObjectID;
	[myController ObjectIdUpdated:self];
}

-(IBAction)btnInstagramPressed:(id)sender
{
	_vwSocials.hidden = YES;
	
	MainController* controller = [MainController sharedController];
	UIImage *screengrab = [glView drawableToCGImage];
	[controller.socials PostAdvertising:SOCIALS_INSTAGRAM withScreenshot: screengrab];
}

-(IBAction)btnVKPressed:(id)sender
{
	_vwSocials.hidden = YES;
	
	MainController* controller = [MainController sharedController];
	UIImage *screengrab = [glView drawableToCGImage];
	[controller.socials PostAdvertising:SOCIALS_VK withScreenshot: screengrab];
}

-(IBAction)btnFBPressed:(id)sender
{
	_vwSocials.hidden = YES;
	
	MainController* controller = [MainController sharedController];
	UIImage *screengrab = [glView drawableToCGImage];
	[controller.socials PostAdvertising:SOCIALS_FACEBOOK withScreenshot: screengrab];
}

-(IBAction)btnTwitterPressed:(id)sender
{
	_vwSocials.hidden = YES;
	
	MainController* controller = [MainController sharedController];
	UIImage *screengrab = [glView drawableToCGImage];
	[controller.socials PostAdvertising:SOCIALS_TWITTER withScreenshot: screengrab];
}


- (IBAction)btnMenuPressed:(id)sender {
    _vwMenu.hidden = !_vwMenu.hidden;
}


-(void)UpdateButtons
{
    dispatch_async(dispatch_get_main_queue(), ^{
        
        [[_btnMap imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnMap setImage:[UIImage imageNamed:@"Map.png"] forState:UIControlStateNormal];
        
        [[_btnContacts imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnContacts setImage:[UIImage imageNamed:@"Contacts.png"] forState:UIControlStateNormal];
        
        [[_btnRemind imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnRemind setImage:[UIImage imageNamed:@"Remind.png"] forState:UIControlStateNormal];
        
        [[_btnToMainMenu imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnToMainMenu setImage:[UIImage imageNamed:@"ToMainMenu.png"] forState:UIControlStateNormal];
        
        [[_btnMenu imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnMenu setImage:[UIImage imageNamed:@"MainMenuButton"] forState:UIControlStateNormal];
        
        [[_btnPhoto imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnPhoto setImage:[UIImage imageNamed:@"camera"] forState:UIControlStateNormal];
        
        [[_btnShare imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnShare setImage:[UIImage imageNamed:@"ShareIco"] forState:UIControlStateNormal];
        
        [[_btnVK imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnVK setImage:[UIImage imageNamed:@"VK"] forState:UIControlStateNormal];
        
        [[_btnFB imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnFB setImage:[UIImage imageNamed:@"FB"] forState:UIControlStateNormal];
        
        [[_btnTwitter imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnTwitter setImage:[UIImage imageNamed:@"Twitter"] forState:UIControlStateNormal];
        
        [[_btnInstagram imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnInstagram setImage:[UIImage imageNamed:@"Instagram"] forState:UIControlStateNormal];
        
        [_btnContacts setTitle:AMLocalizedString(@"Contacts", @"") forState:UIControlStateNormal];
        [_btnRemind setTitle:AMLocalizedString(@"Remind", @"") forState:UIControlStateNormal];
        [_btnMap setTitle:AMLocalizedString(@"Map", @"") forState:UIControlStateNormal];
        [_btnToMainMenu setTitle:AMLocalizedString(@"MainMenu", @"") forState:UIControlStateNormal];
        
        [_btnContacts setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnToMainMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnInstagram setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnVK setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        
        [_btnFB setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnShare setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnTwitter setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnPhoto setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnRemind setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        
        [_lblNovgorod setText:AMLocalizedString(@"Novgorod", @"")];
        [_lblStateMuseum setText:AMLocalizedString(@"StateMuseum", @"")];
        
        [_btnBack setImage:[UIImage imageNamed:@"back"] forState:UIControlStateNormal];
        [_btnBack setTitle:AMLocalizedString(@"Back",@"") forState:UIControlStateNormal];
        [_btnInfo setTitle:AMLocalizedString(@"Information",@"") forState:UIControlStateNormal];
        

    });
}


- (IBAction)btnPhotoPressed:(id)sender {

    UIImage *screengrab = [glView drawableToCGImage];
    
    UIImageWriteToSavedPhotosAlbum(screengrab, nil, nil, nil);
}
- (IBAction)btnSharePressed:(id)sender {
    _vwSocials.hidden = !_vwSocials.hidden;
}

-(void)indexInObjectChanged:(int)index {
    NSLog(@"swithching to the next object: %i", index);
    _activeIndex = index - 1;
    
}
@end
