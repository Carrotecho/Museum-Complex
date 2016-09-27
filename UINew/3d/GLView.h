//
//  GLView.h
//  UltraVision
//
//  Created by easy proger on 06.02.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QCAR/UIGLViewProtocol.h>
#import <AVFoundation/AVFoundation.h>
#import "MarkerMSGs.h"

#import "XMathTypes.h"

#import "DataSenderServer.h"
#import <CoreMotion/CoreMotion.h> 
#import "NSString+URLEncoding.h"
#import "NSString+MD5.h"


#include "LocalizationSystem.h"

@protocol GLViewDelegate <NSObject>
-(void)markerChanged:(uint)newMarkerID is3D:(bool)is3D;
-(void)loadingResource;
-(void)loadingResourceEnd;
-(void)indexInObjectChanged:(int)index;

-(void)loadingResourceProgressBar;
-(void)loadingResourceProgressBarEnd;
-(void)updateProgress:(double)progress;

-(void)showHint:(uint)idHint;
-(void)hideHint;

-(void)reloadResource;
@end

@protocol StabilizationDelegate <NSObject>

-(void)focusAdjustiong;

@end





@interface GLView : UIView<UIGLViewProtocol,GLViewDelegate> {
    
    bool isRunningThread;
    long totalTouch;
    UIButton * mSecondStateButton;
    XMatrix4 vuforiaperspectivematrix;
    uint cameraStyle;
    GLuint defaultFramebuffer, colorRenderbuffer,depthRenderbuffer;
    DataSenderServer*senderRequest;
    GLuint vertexHandle;
    GLuint planeColorsHandle;
    GLuint planeIndexColorsHandle;
    GLuint pMatrixHandle,vMatrixHandle;
    
    GLuint shaderHideMarkerID;
    NSString* markerIDNS;
    NSString*_markerPrefix;
    float colorsValues[256];
    AVCaptureDevice*videoDevice;
    
    float _backgroundWidthFrame;
    float _backgroundHeightFrame;
    float _viewWidth;
    float _viewHeight;
    float _xBackgroundOffset;
    float _yBackgroundOffset;
    int _imageCameraWidth;
    int _imageCameraHeight;

    float _currentwidthMarker;
    float _currentheighMarker;
    
    float _lowSizeRectForTakePic;
    MarkerMSGs *markerMsgView;
    bool showScene;
    int _lastMessageIDShowed;
    uint widthTextureMarker;
    bool arInited;
    int currentResultTakePicForMarker,lastResultTakePicForMarker;
    
    bool*markerChecker;
    
    float offetEditorX;
    float offetEditorY;
    float offetEditorZ;
    
    float offetEditorScaleX;
    float offetEditorScaleY;
    float offetEditorScaleZ;
    
    float offsetEditorRotationX;
    float offsetEditorRotationY;
    float offsetEditorRotationZ;
    
    XVector4 offsetEditorQuaternion;
    
    float lastScale;
    
    int lastIndexIndexesObject;
    
    UISwipeGestureRecognizer*swipeRightGestureController;
    UISwipeGestureRecognizer*swipeLeftGestureController;
    UISwipeGestureRecognizer*swipeUpGestureController;
    UISwipeGestureRecognizer*swipeDownGestureController;

    
    
    float touchesXStart,touchesYStart;
    
    bool timerShowended;
}
-(void)destroyEngine;
-(void)clearEditorChange;
-(void)setEditorMode:(uint )newMode;
-(void)onInitARDone;
-(void)finishOpenGLESCommands;
-(void)freeOpenGLESResources;
-(void)deleteFramebuffer;
-(void)setFramebuffer;
-(void)createFramebuffer;
-(BOOL)presentFramebuffer;
- (void)loadImageTrackerDataSet:(NSString*)dataFile markerPrefix:(NSString*)markerPrefix;
-(id)initWithFrame:(CGRect)frame secondStateButton:(UIButton*)secondStateButton serverPath:(NSString*)_serverPath ;
-(void)setCameraProjectionMatrix:(float*)data;
-(void)toucheEnd:(NSSet *)touches withEvent:(UIEvent *)event;
-(UIImage *) drawableToCGImage;
@property(nonatomic,readwrite) bool needClearScene;
@property(nonatomic,readwrite) uint lastMarkerId;
@property(nonatomic,readwrite) uint editorMode;
@property(nonatomic,readwrite) bool gyroAndAStable;

@property (nonatomic, assign) id<StabilizationDelegate> stabilizationStable;
@property (nonatomic, assign) id<GLViewDelegate> delegate;
@end


