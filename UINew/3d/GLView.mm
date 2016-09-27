//
//  GLView.m
//  UltraVision
//
//  Created by easy proger on 06.02.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//


#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>



#define XTECH_ENGINE_FRAMEWORK 0




#import <QCAR/Image.h>
#import <QCAR/QCAR.h>
#import <QCAR/State.h>
#import <QCAR/Tool.h>
#import <QCAR/Rectangle.h>
#import <QCAR/VirtualButton.h>
#import <QCAR/Renderer.h>
#import <QCAR/ImageTarget.h>
#import <QCAR/CameraDevice.h>
#import <QCAR/VirtualButtonResult.h>
#import <QCAR/ImageTargetResult.h>
#import <QCAR/TrackableResult.h>
#import <QCAR/VideoBackgroundConfig.h>
#import <QCAR/DataSet.h>
#import <QCAR/TrackerManager.h>
#import <QCAR/ImageTarget.h>
#import <QCAR/ObjectTracker.h>

#include "ResourceManager.h"
#include "mathLib.h"
#include "FrameBuffer.h"
#include "SkinnedMesh.h"
#include "Mesh.h"
#include "MeshCache.h"
#include "Scene.h"
#include "Renderer.h"
#include "CompressedTextureFormats.h"
#include "XRenderTypes.h"
#include "Mesh.h"
#include "FileLoader.h"
#include "SoundSystem.h"
#include "DynamicEntity.h"
#include "XAnimations.h"
#include "TextureCache.h"
#include "XGeometry.h"
#include "ParticleEmiter.hpp"
#include "XUDID.h"


#import "SceneController.h"
#import "CacheControll.h"

#import "MainController.h"

#import "GLView.h"

#include <iostream>
#include <vector>


typedef struct{
    uint meshID;
    uint soundID;
}SceneCache;


@interface GLView () {
    NSString *tileDirectory;
    EAGLContext* loadingContext;
    std::string serverPath;
    
    bool currentLoadingFile;
    XMatrix4 viewMatrix,resultM4,transM4;
    
    Camera* camera;
    Renderer* render;
    SceneGraph* mainSceneGraph;
    XAnimations*animationsCache;
    FileLoader*loader;
    SoundSystem* soundSystem;
    TextureCache* textureCache;
    MeshCache*meshCache;
    Light* defaultLight;
    
    DynamicEntity* morpNode;
    
    ParticleEmiter * testParticle;
    
    std::map<std::string,SkinningFrame*> skinAnimationsCache;
    std::map<std::string,SceneController*> sceneLibs;
    std::map<std::string,Material*> materialCache;
    std::map<std::string,Mesh*> meshsCacheMap;
    
    SceneController*currentSceneController;
    SceneController*lastSceneController;
    QCAR::DataSet * _dataSet;
}
@property (nonatomic, retain) EAGLContext *context;
@end



// for cache !
bool isDebug = true;



@interface CreateSceneData : NSObject {
    std::string markerID;
    SceneController*_scene;
    int targetID;
    unsigned char*pixels;
    FrameImageHeader*header;
    std::string path;
    
    float width,height;
}

-(void)setHeight:(float)_val;
-(float)getHeight;

-(void)setWidth:(float)_val;
-(float)getWidth;


-(void)setPath:(std::string)_path;
-(std::string)getPath;


-(void)setHeaderTex:(FrameImageHeader*)_header;
-(FrameImageHeader*)getHeaderTex;


-(void)setPixels:(unsigned char*)_pixels;
-(unsigned char*)getPixels;

-(void)setMarkerID:(std::string)_markerID;
-(std::string)getMarkerID;

-(void)setScene:(SceneController*)scene;
-(SceneController*)getScene;
@end

@implementation CreateSceneData

-(void)setHeight:(float)_val { height = _val;}
-(float)getHeight { return height; }

-(void)setWidth:(float)_val { width = _val; }
-(float)getWidth { return width; }

-(void)setPath:(std::string )_path { path = _path; }
-(std::string )getPath {return path;}

-(void)setHeaderTex:(FrameImageHeader*)_header {header = _header;}
-(FrameImageHeader*)getHeaderTex { return header;}

-(unsigned char*)getPixels { return pixels;}
-(void)setPixels:(unsigned char*)_pixels { pixels = _pixels;}


-(SceneController*)getScene {
    return _scene;
}
-(void)setScene:(SceneController*)scene {
    _scene = scene;
}
-(void)setMarkerID:(std::string)_markerID {
    markerID = _markerID;
}
-(void)setTargetID:(int)_targetID {
    targetID = _targetID;
}
-(int)getTargetID {
    return targetID;
}
-(std::string)getMarkerID {
    return markerID;
}
@end




using namespace Xengine;


@implementation GLView
@synthesize context;
@synthesize editorMode = _editorMode;
@synthesize needClearScene = _needClearScene;
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithCoder:(NSCoder*)coder{
    
    self = [super initWithCoder:coder];
    if (self) {
        
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame secondStateButton:(UIButton*)secondStateButton serverPath:(NSString*)_serverPath  {
    self = [super initWithFrame:frame];
    
    lastIndexIndexesObject = -1;
    _editorMode = 0;
    _needClearScene = false;
    if (self) {
        mSecondStateButton = secondStateButton;
        mSecondStateButton.hidden = true;
        widthTextureMarker = 512;
        serverPath = [_serverPath cStringUsingEncoding:NSUTF8StringEncoding];
        timerShowended = true;
        
     
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && 2.0 == [UIScreen mainScreen].scale) {
            eaglLayer.contentsScale = 2.0f;
        }
        
        
        
        
        
        arInited = false;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:TRUE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
        
        EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!aContext) {
            aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        }
        if (!aContext)
            NSLog(@"Failed to create ES context");
        else if (![EAGLContext setCurrentContext:aContext])
            NSLog(@"Failed to set ES context current");
        
        
        loadingContext = [[EAGLContext alloc] initWithAPI:[aContext API] sharegroup: [aContext sharegroup]];
/*
 
        
 

        UIRotationGestureRecognizer*g3 = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotation:)];
        [self addGestureRecognizer:g3];
        
  */
        
        
        UIPinchGestureRecognizer*g2 = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinch:)];
        [self addGestureRecognizer:g2];
        
        swipeRightGestureController = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeGestureRight:)];
        [swipeRightGestureController setDirection:(UISwipeGestureRecognizerDirectionRight  )];
        [self addGestureRecognizer:swipeRightGestureController];
      
        
        swipeLeftGestureController = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeGestureLeft:)];
        [swipeLeftGestureController setDirection:(UISwipeGestureRecognizerDirectionLeft  )];
        [self addGestureRecognizer:swipeLeftGestureController];
        
        swipeUpGestureController = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeGestureUp:)];
        [swipeUpGestureController setDirection:(UISwipeGestureRecognizerDirectionUp  )];
        [self addGestureRecognizer:swipeUpGestureController];
        
        
        swipeDownGestureController = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeGestureDown:)];
        [swipeDownGestureController setDirection:(UISwipeGestureRecognizerDirectionDown  )];
        [self addGestureRecognizer:swipeDownGestureController];
        
        
        swipeDownGestureController.enabled = false;
        swipeUpGestureController.enabled = false;
        
        
        [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
        [[NSNotificationCenter defaultCenter]
         addObserver:self selector:@selector(orientationChanged:)
         name:UIDeviceOrientationDidChangeNotification
         object:[UIDevice currentDevice]];
        
        
        //[[UIDevice currentDevice] orientation]
        
        
        
        self.context = aContext;
        [self initEngine];
        
        
        NSArray *subviewArray = [[NSBundle mainBundle] loadNibNamed:@"MarkerMSGs" owner:self options:nil];
        markerMsgView = (MarkerMSGs*)[subviewArray objectAtIndex:0];
        
        
        totalTouch = 0;
        
        senderRequest = [[DataSenderServer alloc] init];
        markerIDNS = @"";
        
        offetEditorScaleX = 1.0;
        offetEditorScaleY = 1.0;
        offetEditorScaleZ = 1.0;

        offetEditorX = 0.0;
        offetEditorY = 0.0;
        offetEditorZ = 0.0;
        
        offsetEditorRotationX = 0.0;
        offsetEditorRotationY = 0.0;
        offsetEditorRotationZ = 0.0;
        
        [self addSubview:markerMsgView];
        
    }
    return self;
}

-(void)reloadResource {
    
}


-(void)loadingResource {
    
}

-(void)loadingResourceEnd {
    
}
-(NSString*)createReqString:(NSString*)req data:(NSArray*)data {
    NSString * baseString = [NSString stringWithFormat:@"%s/editor.php?request=%@",serverPath.c_str(),req];
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


-(NSString*)getSFromFloat:(float)val {
    return [NSString stringWithFormat:@"%f",val];
}




-(void)editorUpdateFail:(id)sender {
    
}

-(void)clearEditorChange {
    offetEditorX = 0;
    offetEditorY = 0;
    offetEditorZ = 0;
    offetEditorScaleX = 1.0;
    offetEditorScaleY = 1.0;
    offetEditorScaleZ = 1.0;
    offsetEditorRotationX = 0.0;
    offsetEditorRotationY = 0.0;
    offsetEditorRotationZ = 0.0;
    
    XVector3 degress(offsetEditorRotationX,offsetEditorRotationY,offsetEditorRotationZ);
    degress*=DEG_TO_RAD;
    XQuaternionFromEuler(&offsetEditorQuaternion,&degress);
}

-(void)editorUpdata:(NSArray*)sender {
    
    
    NSError* error = nil;
    
    NSData *fileData = [NSData dataWithContentsOfURL:[NSURL URLWithString:markerIDNS] options: 0 error: &error];
    if (fileData == nil) {
        NSLog(@"Failed to read file, error %@", error);
    }else {
        [CacheControll cacheData:fileData fromServerPath:markerIDNS];
    }
    
    UIAlertView *successAlert = [[UIAlertView alloc] initWithTitle:@"Success"
                                                         message:@"Данные сохраннены"
                                                        delegate:nil
                                               cancelButtonTitle:@"OK"
                                               otherButtonTitles:nil];
    [successAlert show];
    
    NSLog(@"%@",[sender objectAtIndex:0]);
}


-(void)swipeGestureLeft:(UISwipeGestureRecognizer*)sender {
    if (_editorMode != 0) return;
    
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            
            [EAGLContext setCurrentContext:loadingContext];
            
            currentSceneController->swipeController(2);
            
            glFlush();
            [EAGLContext setCurrentContext: nil ];
            
            
        }
    }
}


-(void)swipeGestureRight:(UISwipeGestureRecognizer*)sender {
    if (_editorMode != 0) return;
    
        
    
    
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            [EAGLContext setCurrentContext:loadingContext];
            
            currentSceneController->swipeController(1);
            
            glFlush();
            [EAGLContext setCurrentContext: nil ];
        }
    }
}


- (void) orientationChanged:(NSNotification *)note
{}


-(void)swipeGestureDown:(UISwipeGestureRecognizer*)sender {
    
    if ([[UIDevice currentDevice] orientation] == UIDeviceOrientationLandscapeRight) {
        [self swipeGestureLeft:sender];
    }else {
        [self swipeGestureRight:sender];
    }
    
}

-(void)swipeGestureUp:(UISwipeGestureRecognizer*)sender {
    
    
    if ([[UIDevice currentDevice] orientation] == UIDeviceOrientationLandscapeRight) {
        [self swipeGestureRight:sender];
    }else {
        [self swipeGestureLeft:sender];
    }
    
    //[[UIDevice currentDevice] orientation]
    
    
    
    /*
     NSArray*dataArray = [NSArray arrayWithObjects:
     markerIDNS,
     [self getSFromFloat:offetEditorX ],
     [self getSFromFloat:offetEditorY ],
     [self getSFromFloat:offetEditorZ ],
     [self getSFromFloat:offetEditorScaleX ],
     [self getSFromFloat:offetEditorScaleY ],
     [self getSFromFloat:offetEditorScaleZ ],
     [self getSFromFloat:offsetEditorRotationX ],
     [self getSFromFloat:offsetEditorRotationY ],
     [self getSFromFloat:offsetEditorRotationZ ],nil];
     
     
     [senderRequest send:[self createReqString:@"" data:dataArray] callBackTarget:self callBackAction:@selector(editorUpdata:) callBackActionFail:@selector(editorUpdateFail:) succesParrams:[NSNumber numberWithInt:1] cookies:nil];
     */
}



-(void)initEngine {
    shaderHideMarkerID = 0;
    
    for (int i = 0;i<256;i++) {
        colorsValues[i] = (float)i/255.0;
    }
    
    QCAR::setFrameFormat(QCAR::RGB888, true);
    
    currentSceneController = NULL;
    
    XMatrix4MakeWithArray(&transM4,
                          1,  0, 0, 0,
                          0,  0, 1, 0,
                          0, -1, 0, 0,
                          0,  0, 0, 1);
    
    currentLoadingFile = false;
    tileDirectory = [[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"];
    string pathBase = [tileDirectory UTF8String];
    
    textureCache    = new TextureCache();
    soundSystem     = new SoundSystem();
    mainSceneGraph  = new SceneGraph();
    defaultLight    = new Light();
    
    
    cameraStyle = CAMERA_STYLE_VUFORIA;
    
    std::map<uint, Mesh*> damagedMeshs;
    
    meshCache       = new MeshCache(&damagedMeshs);
    animationsCache = new XAnimations(&skinAnimationsCache);
    loader          = new FileLoader(meshCache,animationsCache);
    camera          = new Camera();
    render          = new Renderer(camera,(char*)[tileDirectory  UTF8String]);
    mainSceneGraph  = new SceneGraph();
    
    camera->setPerspective(90.0, WIDTH_VIEWPORT   ,HEIGHT_VIEWPORT, 1.0, 10000.0);
    camera->resetModelViewMatrix();
    camera->lookAt(vec3(0.0f, 500.0f, 2000.0f), vec3(0.0, 0.0, 0.0), vec3(0,1,0));
    
    
    
    XMatrix4 inversed;
    
    XMatrix4 *cameraView = camera->getModelViewMatrix();
    
    XMatrix4Inverse(&inversed, cameraView);
    
    
    
    
    
    render->setDefaultSceneGraph(mainSceneGraph);
    mainSceneGraph->addLightinTree(defaultLight);
    
    defaultLight->lookAt(vec3(1000.0f, 1000.0f, 1000.0f), vec3(0.0, 0.0, 0.0), vec3(0,1,0));
    glEnable(GL_DEPTH_TEST);

    
    
    
    
    
    
    
    
    markerChecker = [[MainController sharedController] getChecker];
    
}


-(void)setEditorMode:(uint )newMode {
    _editorMode = newMode;
    if (_editorMode >0) {
        swipeLeftGestureController.enabled = false;
        swipeRightGestureController.enabled = false;
        swipeUpGestureController.enabled = false;
    }else {
        swipeLeftGestureController.enabled = true;
        swipeRightGestureController.enabled = true;
        swipeUpGestureController.enabled = true;
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
   
   
    CGPoint tappedPt = [[touches anyObject] locationInView: self];
    touchesXStart = tappedPt.x;
    touchesYStart = tappedPt.y;

    
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            currentSceneController->touchesBegan(tappedPt.x,tappedPt.y);
        }
    }
}

-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    
}

-(void)toucheEnd:(NSSet *)touches withEvent:(UIEvent *)event {
    
}

-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    
  
    
    
    CGPoint tappedPt = [[touches anyObject] locationInView: self];
    
    
    if (currentSceneController && _editorMode==0) {
        if (currentSceneController->isValid()) {
            currentSceneController->touchesMoved(tappedPt.x, tappedPt.y);
        }
    }
    
    
    if (_editorMode == 1) {
        offetEditorX+= (tappedPt.x-touchesXStart)*5.0;
        touchesXStart = tappedPt.x;
    }else if (_editorMode == 2) {
        offetEditorZ+= (tappedPt.y-touchesYStart)*5.0;
        touchesYStart = tappedPt.y;
    }else if (_editorMode == 3) {
        offetEditorY+= (tappedPt.y-touchesYStart)*5.0;
        touchesYStart = tappedPt.y;
    }else if (_editorMode == 4) {
        offetEditorX+= (tappedPt.x-touchesXStart)*5.0;
        touchesXStart = tappedPt.x;
        
        offetEditorZ+= (tappedPt.y-touchesYStart)*5.0;
        touchesYStart = tappedPt.y;
        
    }else if (_editorMode == 5) { // rotationX
        offsetEditorRotationX+= (tappedPt.x-touchesXStart)*1.0;
        touchesXStart = tappedPt.x;
        
        XVector3 degress(offsetEditorRotationX,offsetEditorRotationY,offsetEditorRotationZ);
        degress*=DEG_TO_RAD;
        
        XQuaternionFromEuler(&offsetEditorQuaternion,&degress);
    }else if (_editorMode == 6) { // rotationY
        offsetEditorRotationY+= (tappedPt.y-touchesYStart)*1.0;
        touchesYStart = tappedPt.y;
        
        XVector3 degress(offsetEditorRotationX,offsetEditorRotationY,offsetEditorRotationZ);
        degress*=DEG_TO_RAD;
        
        XQuaternionFromEuler(&offsetEditorQuaternion,&degress);
    }else if (_editorMode == 7) { // rotationZ
        offsetEditorRotationZ+= (tappedPt.y-touchesYStart)*1.0;
        touchesYStart = tappedPt.y;
        
        XVector3 degress(offsetEditorRotationX,offsetEditorRotationY,offsetEditorRotationZ);
        degress*=DEG_TO_RAD;
        
        XQuaternionFromEuler(&offsetEditorQuaternion,&degress);
    }
    
    
}

- (IBAction)pan:(UIPanGestureRecognizer *)sender
{
    
}

- (IBAction)pinch:(UIPinchGestureRecognizer *)sender
{
    
    
    
    float scale = 1.0;
    if (_editorMode == 8) {
        
        if (sender.velocity <0.0) {
            scale=-(1.0-[sender scale])/20000.0f;
            
        }else {
            scale=([sender scale])/200000.0f;
        }
    }else if (_editorMode == 9){
        
    }
    
    
    
    
    if (sender.velocity <0.0) {
        scale=-(1.0-[sender scale])/200.0f;
        
    }else {
        scale=([sender scale])/2000.0f;
    }
    
    
    
    if (currentSceneController && _editorMode==0) {
        if (currentSceneController->isValid()) {
            currentSceneController->zoom(scale);
        }
    }
    
    
    if (_editorMode != 8 && _editorMode != 9 ) return;
    
    
    offetEditorScaleX+=scale;
    offetEditorScaleY+=scale;
    offetEditorScaleZ+=scale;
    
    
    if (offetEditorScaleX < 0.0001) {
        offetEditorScaleX = 0.0001;
        offetEditorScaleY = 0.0001;
        offetEditorScaleZ = 0.0001;
    }else if (offetEditorScaleX > 5.001) {
        offetEditorScaleX = 5.0;
        offetEditorScaleY = 5.0;
        offetEditorScaleZ = 5.0;
    }
    
    printf("%f\n",scale);
    //camera->lookAt(vec3(0.0, 500.0, 2000.0f*[sender scale]), vec3(0.0, 0.0, 0.0), vec3(0,1,0));
    
}

- (IBAction)rotation:(UIRotationGestureRecognizer *)sender
{
    
}





-(void)mainThread:(NSNumber*)showSecondButton {
    [mSecondStateButton setHidden:showSecondButton.boolValue];
}

-(void)setCameraProjectionMatrix:(float*)data {
    
    XMatrix4MakeWithArray(&vuforiaperspectivematrix, data);
    
#if SHOW_AR
    camera->setPerspectivematrix(data);
    camera->resetModelViewMatrix();
#endif
    
}
float ssss = 0;



#if !SHOW_AR


float lightAngle2 = 0.0;

vec3 posAngle;
#endif

bool clearSM = false;
int idSM = 0;






-(void)showLoading {
    [self showLoadingAnimation];
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

- (BOOL) setExtendedTrackingForDataSet:(QCAR::DataSet *)theDataSet start:(BOOL) start {
    BOOL result = YES;
    for (int tIdx = 0; tIdx < theDataSet->getNumTrackables(); tIdx++) {
        QCAR::Trackable* trackable = theDataSet->getTrackable(tIdx);
        
        if (!start) {
            if (!trackable || !trackable->isExtendedTrackingStarted()) {
                continue;
            }
        }
        
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

- (BOOL)deactivateDataSet:(QCAR::DataSet *)theDataSet
{
    if (!theDataSet) {
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
    
    
    return success;
}

// Load the image tracker data set
- (void)loadImageTrackerDataSet:(NSString*)dataFile markerPrefix:(NSString*)markerPrefix
{
    _markerPrefix = markerPrefix;
    
    NSLog(@"loadImageTrackerDataSet (%@)", dataFile);
    QCAR::DataSet * dataSet = NULL;
    
    // Get the QCAR tracker manager image tracker
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    
    
    
    QCAR::ObjectTracker* imageTracker = static_cast<QCAR::ObjectTracker*>(trackerManager.getTracker(QCAR::ObjectTracker::getClassType()));
    
    if (NULL == imageTracker) {
        NSLog(@"ERROR: failed to get the ImageTracker from the tracker manager");
        return ;
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
    
    _dataSet = dataSet;
    
    imageTracker->activateDataSet(dataSet);
    
    
    for (int tIdx = 0; tIdx < dataSet->getNumTrackables(); tIdx++) {
        QCAR::Trackable* trackable = dataSet->getTrackable(tIdx);
        //trackable->startExtendedTracking();
    }
    [self doStartTrackers];
    
    
}

-(void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event {
     NSLog(@"test");
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    if (motion == UIEventSubtypeMotionShake)
    {
       
    }
}

-(void)onInitARDone {
    const QCAR::VideoBackgroundConfig& QCARBackgroundConf = QCAR::Renderer::getInstance().getVideoBackgroundConfig();
    
    _backgroundWidthFrame = QCARBackgroundConf.mSize.data[0];
    _backgroundHeightFrame = QCARBackgroundConf.mSize.data[1];
    
    _viewWidth  = self.frame.size.width;
    _viewHeight = self.frame.size.height;
    
    _xBackgroundOffset = ((int) _viewWidth - _backgroundWidthFrame) / 2.0f + QCARBackgroundConf.mPosition.data[0];
    _yBackgroundOffset = ((int) _viewHeight - _backgroundHeightFrame) / 2.0f + QCARBackgroundConf.mPosition.data[1];
    
    
    _lowSizeRectForTakePic = _viewWidth > _viewHeight ? _viewHeight: _viewWidth;
    
    [markerMsgView inizialize];
    [NSTimer scheduledTimerWithTimeInterval:1 target:self selector:@selector(updateInterface:) userInfo:nil repeats:YES];
    
    
    
    
    
    arInited = true;
    
}

uint currentFramesWithGoodFocus = 0;
uint maxNumFramesWithGoodFocus = 30;

-(bool)isGoodForTakePicture {
    videoDevice = [[AVCaptureDevice devices] objectAtIndex:0];
    if (videoDevice) {
        if (videoDevice.adjustingFocus || videoDevice.adjustingExposure) {
            if (self.stabilizationStable && [self.stabilizationStable respondsToSelector:@selector(focusAdjustiong)]) {
                [self.stabilizationStable focusAdjustiong];
            }
            currentFramesWithGoodFocus = 0;
        }else {
            currentFramesWithGoodFocus++;
            if (currentFramesWithGoodFocus > maxNumFramesWithGoodFocus) {
                
                currentFramesWithGoodFocus = 0;
                return YES;
            }else {
                return NO;
            }
        }
    }
    return NO;
}




-(void)updateInterface:(NSTimer*)timer {
    
    
    
    int wantShowID = 0;
    
    if (currentSceneController && currentSceneController->waitTextureFromFrame && currentResultTakePicForMarker != 0) {
        wantShowID = SHOW_MSG_WRONG_DEVICE_POSITION;
    }
    
    if (!showScene) {
        wantShowID = SHOW_MSG_MOVE_DEVICE_ON_MARKER;
    }
    
    // need show message
    [self showNewMsg:wantShowID];
    _lastMessageIDShowed = wantShowID;
}

-(void)showNewMsg:(int)msgID {
    [markerMsgView showMsgID:msgID indexMsg:currentResultTakePicForMarker];
}


-(void)getTranslatedVecByModel:(XMatrix4*)modelViewMatrix result:(XVector4*)result {

    XMatrix4MultiplyVector4(result, modelViewMatrix, result);
    XMatrix4MultiplyVector4(result, camera->getPerspectiveMatrix(), result);

    result->x = result->x/result->w;
    result->y = result->y/result->w;
    result->z = result->z/result->w;
    
    result->x = ((result->x+1.0) / 2.0);
    result->y = ((result->y+1.0) / 2.0);
    
}


// need combine two methods getXCoordinats and writeFrameTextureBGWidth

-(bool)getXCoordinats:(int)iW
                   iH:(int)iH
               result:(XVector4*)result
{
    XVector4 p1,p2;
    
    
    float centerMarkerW = _currentwidthMarker/2.0;
    float centerMarkerH = _currentheighMarker/2.0;
    
    p1.x = -centerMarkerW;
    p1.y =  0.0;
    p1.z =  0.0;
    p1.w =  1.0;
    
    p2.x =  centerMarkerW;
    p2.y =  0.0;
    p2.z =  0.0;
    p2.w =  1.0;
    
    [self getTranslatedVecByModel:&viewMatrix result:&p1];
    p1.x = (p1.x*_backgroundWidthFrame)+_xBackgroundOffset;
    p1.y = (ABS(p1.y-1.0)*_backgroundHeightFrame)+_yBackgroundOffset;
    
    [self getTranslatedVecByModel:&viewMatrix result:&p2];
    p2.x = (p2.x*_backgroundWidthFrame)+_xBackgroundOffset;
    p2.y = (ABS(p2.y-1.0)*_backgroundHeightFrame)+_yBackgroundOffset;
    
    
    int w = (int)(sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)));
    result->z = w;
    float scaleX = _imageCameraWidth/_viewHeight;
    float scaleY = _imageCameraHeight/_viewWidth;
    float scale = scaleX;
    
    if (scale > scaleY) {
        scale = scaleY;
    }
    
    
    float scaledWidth = _viewWidth * scale;
    float scaledHeight = _viewHeight * scale;
    
    w = w * scale;
    w = widthTextureMarker;
    const int h = w;
    
    float borderX = (_imageCameraWidth-scaledHeight)/2.0;
    float borderY = (_imageCameraHeight-scaledWidth)/2.0;

    
    float dX  = (float)iW/(float)w;
    float dx2 = (dX*_currentwidthMarker)-centerMarkerW;
    float dY  = (float)iH/(float)h;
    float dY2 = (dY*_currentheighMarker)-centerMarkerH;
    
    p1.x =  dx2;
    p1.y =  dY2;
    p1.z =  0.0;
    p1.w =  1.0;
    
    [self getTranslatedVecByModel:&viewMatrix result:&p1];
    p1.x = (p1.x*_backgroundWidthFrame)+_xBackgroundOffset;
    p1.y = (p1.y*_backgroundHeightFrame)+_yBackgroundOffset;
    
    p1.x*=scale;
    p1.y*=scale;
    
    dX   = p1.x;
    p1.x = p1.y;
    p1.y = dX;
    
    p1.x = scaledHeight-p1.x;
    p1.y = scaledWidth-p1.y;
    
    p1.x+=borderX;
    p1.y+=borderY;
    
    
    if (p1.x < 0 || p1.y < 0) {
        return false;
    }
    
    if (p1.x > _imageCameraWidth-1 || p1.y > _imageCameraHeight-1) {
        return false;
    }
    
    result->x = p1.x;
    result->y = p1.y;
    
    return true;
}



-(void)writeFrameTextureBGWidth:(unsigned char*)pixels
                           path:(std::string)path
{
    
    
    XVector4 p1,p2;
    
    
    float centerMarkerW = _currentwidthMarker/2.0;
    float centerMarkerH = _currentheighMarker/2.0;
    
    p1.x = -centerMarkerW;
    p1.y =  0.0;
    p1.z =  0.0;
    p1.w =  1.0;
    
    p2.x =  centerMarkerW;
    p2.y =  0.0;
    p2.z =  0.0;
    p2.w =  1.0;
    
    [self getTranslatedVecByModel:&viewMatrix result:&p1];
    p1.x = (p1.x*_backgroundWidthFrame)+_xBackgroundOffset;
    p1.y = (ABS(p1.y-1.0)*_backgroundHeightFrame)+_yBackgroundOffset;
    
    [self getTranslatedVecByModel:&viewMatrix result:&p2];
    p2.x = (p2.x*_backgroundWidthFrame)+_xBackgroundOffset;
    p2.y = (ABS(p2.y-1.0)*_backgroundHeightFrame)+_yBackgroundOffset;
    
    
    int w = (int)(sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)));
    
    float scaleX = _imageCameraWidth/_viewHeight;
    float scaleY = _imageCameraHeight/_viewWidth;
    float scale = scaleX;
    
    if (scale > scaleY) {
        scale = scaleY;
    }
    
    
    float scaledWidth = _viewWidth * scale;
    float scaledHeight = _viewHeight * scale;
    
    w = w * scale;
    w = widthTextureMarker;
    const int h = w;
    
    float borderX = (_imageCameraWidth-scaledHeight)/2.0;
    float borderY = (_imageCameraHeight-scaledWidth)/2.0;
    
    
    
    int bytesNum = 3;
    
    const NSInteger myDataLength = w * h * bytesNum;
    GLubyte *buffer = (GLubyte *) malloc(myDataLength);
    GLubyte *deltaBuffer =(GLubyte *) malloc(bytesNum);
    
    int iWF = w;
    
    
    
    for (int iW = 0; iW < w; iW++) {
        int iHf = h;
        iWF--;
        for (int iH = 0; iH < h; iH++) {
            iHf--;
            float dX  = (float)iW/(float)w;
            float dx2 = (dX*_currentwidthMarker)-centerMarkerW;
            float dY  = (float)iH/(float)h;
            float dY2 = (dY*_currentheighMarker)-centerMarkerH;
            
            p1.x =  dx2;
            p1.y =  dY2;
            p1.z =  0.0;
            p1.w =  1.0;
            
            [self getTranslatedVecByModel:&viewMatrix result:&p1];
            p1.x = (p1.x*_backgroundWidthFrame)+_xBackgroundOffset;
            p1.y = (p1.y*_backgroundHeightFrame)+_yBackgroundOffset;
            
            p1.x*=scale;
            p1.y*=scale;
            
            dX   = p1.x;
            p1.x = p1.y;
            p1.y = dX;
            
            p1.x = scaledHeight-p1.x;
            p1.y = scaledWidth-p1.y;
            
            p1.x+=borderX;
            p1.y+=borderY;
            
            
            if (p1.x < 0 || p1.y < 0) {
                continue;
            }
            
            if (p1.x > _imageCameraWidth-1 || p1.y > _imageCameraHeight-1) {
                continue;
            }
            
            unsigned char* pixel = pixels + (((int)p1.y*_imageCameraWidth+(int)p1.x)*bytesNum);
            memcpy( buffer + ((iHf*h+iW)*bytesNum) ,pixel,bytesNum);
           
        }
    }
    
    free(deltaBuffer);
    
    
    
    
    FrameImageHeader*header = (FrameImageHeader*)malloc(sizeof(FrameImageHeader));
    
    header->bytesNum = bytesNum;
    header->width    = w;
    header->height   = h;
    
    CreateSceneData *data = [CreateSceneData alloc];
    
    [data setPath:path];
    [data setHeaderTex:header];
    [data setPixels:buffer];
    [data setScene:currentSceneController];
    
    // debug
    [self performSelectorOnMainThread:@selector(showPic:) withObject:data waitUntilDone:YES];
    
    [self showLoading];
    
    [self performSelectorInBackground: @selector(saveFrameTexture:) withObject: data ]; // loading
}


-(void)saveFrameTexture:(CreateSceneData *)data {
    
    NSString * stringFile = [NSString stringWithFormat:@"%s",[data getPath].c_str(),nil];
    NSError*error = nil;
    [[NSFileManager defaultManager] removeItemAtPath:stringFile error:nil];
    [[NSFileManager defaultManager] createDirectoryAtPath:[stringFile stringByDeletingLastPathComponent] withIntermediateDirectories:YES attributes:nil error:&error];
    
    
    
    error = nil;
    
    [[NSData data] writeToFile:stringFile options:NSDataWritingAtomic error:&error];
    
  
    
    FILE*file = fopen([data getPath].c_str(), "wb+");
    if (file) {
        FrameImageHeader*header = [data getHeaderTex];
        unsigned char* pixels   = [data getPixels];
        
        fwrite(header, 1, sizeof(FrameImageHeader), file);
        fwrite(pixels, 1, sizeof(unsigned char)*(header->width * header->height * header->bytesNum), file);
        
        free(header);
        free(pixels);
    }
    fclose(file);
    
    
    
    Texture*tex = textureCache->getTextureByPath([data getPath].c_str());
    
    if (tex) {
        if (!tex->isDamaged()) {
            [EAGLContext setCurrentContext:loadingContext];
            
            tex->reloadTexture();
            
            glFlush();
            [EAGLContext setCurrentContext: nil ];
        }
    }
    
    [self hideLoadingAnimation];
}



-(void)showPic:(CreateSceneData*)d {
    
    int w = [d getHeaderTex]->width;
    int h = [d getHeaderTex]->height;
    int bytesNum =[d getHeaderTex]->bytesNum;
    
    NSData *data = [NSData dataWithBytes:[d getPixels] length:bytesNum*w*h];
    CGColorSpaceRef colorSpace;
    
    colorSpace = CGColorSpaceCreateDeviceRGB();
    CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    
    CGImageRef imageRef = CGImageCreate(w, h, 8, 8 * bytesNum, w*bytesNum, colorSpace,kCGImageAlphaNone|kCGBitmapByteOrderDefault, provider,NULL, false,kCGRenderingIntentDefault);
    
    UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    
    UIImageView*imageView = [[UIImageView alloc] initWithImage:finalImage];
    [imageView setFrame:CGRectMake(0, 0, 768, 768)];
    
    
    UIImageWriteToSavedPhotosAlbum(finalImage,
                                   nil,
                                   nil,
                                   nil);
    
    
   // [self addSubview:imageView];
}






-(uint)frameMarkerWork:(QCAR::State)state markerID:(string)markerID {
    
    if (!currentSceneController || !currentSceneController->waitTextureFromFrame) {
        return FRAME_MARKER_NOT_READY;
    }
    
    unsigned char* pixels;
    
    const QCAR::Image *qcarImageRGB888 = nil;
    QCAR::Frame frame = state.getFrame();
    
    for (int iQ = 0; iQ < frame.getNumImages(); iQ++)
    {
        const QCAR::Image *qcarImage = frame.getImage(iQ);
        if (qcarImage->getFormat() == QCAR::RGB888)
        {
            qcarImageRGB888 = qcarImage;
        }
    }
    
    if (qcarImageRGB888) {
        _imageCameraWidth  = qcarImageRGB888->getWidth();
        _imageCameraHeight = qcarImageRGB888->getHeight();
    }
    _currentwidthMarker = currentSceneController->widthMarker;
    _currentheighMarker = currentSceneController->heightMarker;
    
    XVector4 pointOnScreen1,pointOnScreen2,pointOnScreen3,pointOnScreen4;
    bool resultGetCoords1 = [self getXCoordinats:0                  iH:0                  result:&pointOnScreen1];
    bool resultGetCoords2 = [self getXCoordinats:widthTextureMarker iH:0                  result:&pointOnScreen2];
    bool resultGetCoords3 = [self getXCoordinats:0                  iH:widthTextureMarker result:&pointOnScreen3];
    bool resultGetCoords4 = [self getXCoordinats:widthTextureMarker iH:widthTextureMarker result:&pointOnScreen4];
    
    
    // first check rect
    if (resultGetCoords1 && resultGetCoords2 && resultGetCoords3 && resultGetCoords4) {
        // second check size - can get info about size from any point
        if (pointOnScreen1.z >= _lowSizeRectForTakePic-(_lowSizeRectForTakePic/2)) {
            // good size need check autofocus
      
            
            if ([self isGoodForTakePicture] && self.gyroAndAStable) {
                // all test done ! can take pic
                currentSceneController->waitTextureFromFrame = false;
                pixels = (unsigned char*)qcarImageRGB888->getPixels();
                std::string markerPath = serverPath+"/textures/"+[_markerPrefix UTF8String]+markerID+".frame";
                
                NSString * s = [NSString stringWithFormat:@"Library/Caches/%s",markerPath.c_str(),nil];
                NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
                std::string localFilePath = [FilePath cStringUsingEncoding:NSUTF8StringEncoding];
                
                
                [self writeFrameTextureBGWidth:pixels path:localFilePath];
            }else {
                // current autoFocusing
                return FRAME_MARKER_AUTOFOCUSING;
            }
        } else {
            return FRAME_MARKER_NEED_ZOOM_IN;
        }
    } else {
        return FRAME_MARKER_NOT_ALL_MARKER_IN_FRAME;
    }
    
    return FRAME_MARKER_TAKED;
}


uint numNotShowedFrames = 0;
uint maxNotShowFrames = 50;
uint numShowFrames = 0;
uint stableFrameCount = 50;


-(void)correctTextureCache {
    
    
    
    
    dispatch_sync(dispatch_get_main_queue(), ^{
        [EAGLContext setCurrentContext:loadingContext];
        textureCache->correctCache();
        glFlush();
        [EAGLContext setCurrentContext: nil ];
    });
    
    
    
}




-(void)changeCameraStyle:(uint)style {
    cameraStyle = style;
    if (cameraStyle == CAMERA_STYLE_VUFORIA) {
        camera->setPerspectivematrix(&vuforiaperspectivematrix.m[0]);
    }else if (cameraStyle == CAMERA_STYLE_FLIP) {
        int viewport[4];;
        glGetIntegerv(GL_VIEWPORT, viewport);
        camera->setPerspective(90.0, viewport[2]   ,viewport[3], 1.0, 1000.0);
        camera->lookAt(vec3(0.0,0.0,50), vec3(0,0,0), vec3(0,1,0));
        XMatrix4MakeWithArray(&viewMatrix, &camera->getModelViewMatrix()->m[0]);
    }
}

-(void)destroyEngine {
    arInited = false;
    XLOG("destroy engine");
    [self clearScene];
    
    delete textureCache;
    delete soundSystem;
    delete mainSceneGraph;
    delete meshCache;
    delete animationsCache;
    delete loader;
    delete render;
}






uint framesWithoutMarker = 0;
uint maxFramesWithOutMarker = 150;
int frameID = 400;
- (void)renderFrameQCAR
{
    if (!arInited) return;
    [self setFramebuffer];
    
    
    glDisable(GL_BLEND);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    QCAR::State state = QCAR::Renderer::getInstance().begin();
    QCAR::Renderer::getInstance().drawVideoBackground();
    QCAR::Renderer::getInstance().end();
    
    
    
    QCAR::setFrameFormat( QCAR::RGB888, true );

    framesWithoutMarker++;
    bool canRender = false;
    
    
    if (_needClearScene) {
        [self clearScene];
        _needClearScene = false;
    }
    
    if (isRunningThread) {
        [self presentFramebuffer];
        return;
    }
    
    
    
    if (!state.getNumTrackableResults()) {
        self.lastMarkerId = 0;
        if (currentSceneController && currentSceneController->isValid()) {
            currentSceneController->lostMarker(true);
        }
    }else {
        if (currentSceneController && currentSceneController->isValid()) {
            currentSceneController->lostMarker(false);
        }
    }
    
    for (int i = 0; i < state.getNumTrackableResults(); ++i) {
        // Get the trackable
        const QCAR::TrackableResult* result = state.getTrackableResult(i);
        const QCAR::Trackable& trackable    = result->getTrackable();
        const QCAR::Matrix34F& poseMatrix   = result->getPose();
        
        string markerID = trackable.getName();
        
       
        int targetID = trackable.getId()-1;
        
        
        
        
        if (cameraStyle == CAMERA_STYLE_VUFORIA) {
            QCAR::Matrix44F modelViewMatrix = QCAR::Tool::convertPose2GLMatrix(poseMatrix);
            modelViewMatrix.data[12]/=200.0;
            modelViewMatrix.data[13]/=200.0;
            modelViewMatrix.data[14]/=200.0;

            XMatrix4MakeWithArray(&viewMatrix, &modelViewMatrix.data[0]);
        }else if (cameraStyle == CAMERA_STYLE_FLIP) {
            camera->lookAt(vec3(0.0,0.0,50), vec3(0,0,0), vec3(0,1,0));
            XMatrix4MakeWithArray(&viewMatrix, &camera->getModelViewMatrix()->m[0]);
        }
        
     
        framesWithoutMarker = 0;
   
    
        markerID = markerID.substr(0,10);
        
        std::string info = markerID.substr(0,6);
        
        int isInfo = std::strcmp("infosd", info.c_str());
        
        
        string cuttedMarkerId = markerID.substr (7,3);
        int markerIDInt = atoi(cuttedMarkerId.c_str());
        
        if (isInfo == 0) {
            // is info marker
        }else {
            if (self.lastMarkerId != markerIDInt) {
                self.lastMarkerId = markerIDInt;
                
                dispatch_sync(dispatch_get_main_queue(), ^{
                    if (MAX_OBJECTS_IDS > markerIDInt) {
                        if (self.delegate && [self.delegate respondsToSelector:@selector(markerChanged:is3D:)]) {
                            [self.delegate markerChanged:markerIDInt is3D:markerChecker[markerIDInt]];
                        }
                    }
                    
                });
                printf("markerID:%s\n",markerID.c_str());
            }
            
            if (MAX_OBJECTS_IDS > markerIDInt) {
                if (!markerChecker[markerIDInt]) {
                    continue;
                };
            }
        }
        
        
        
        
        
        if ( sceneLibs.find(markerID) != sceneLibs.end() ) {
            currentSceneController = sceneLibs[markerID];
            
            
            if (currentSceneController && currentSceneController->needReloadScene) {
                [self clearScene];
                numShowFrames = 0;
                
                
                return;
            }
            
        }else {
            int lll = self.lastMarkerId;
            [self clearScene];
            self.lastMarkerId = lll;
            
            sceneLibs.clear();
            sceneLibs[markerID] = NULL;
            
            animationsCache->clear();
            
            
            lastIndexIndexesObject = -1;
            
            dispatch_sync(dispatch_get_main_queue(), ^{
                soundSystem->clearAllSoundData();
            });
            
            [self correctTextureCache];
            
            
            CreateSceneData *data = [CreateSceneData alloc];
            
            
            const QCAR::ImageTarget &imageTarget = (const QCAR::ImageTarget &) trackable;
            QCAR::Vec3F targetSize = imageTarget.getSize();
            targetSize.data[0]/=200.0;
            targetSize.data[1]/=200.0;
            
            [data setWidth:targetSize.data[0]];
            [data setHeight:targetSize.data[1]];
            
            
            
            [data setMarkerID: markerID];
            [data setTargetID: targetID];
            [self showLoadingAnimationProgress];
            isRunningThread = true;
            [self performSelectorInBackground: @selector(createNewScene:) withObject: data ]; // loading
        }
        
        if (currentSceneController) {
            
            if (currentSceneController->isValid()) {
                canRender = true;
            }else {
                canRender = false;
                if (currentSceneController->getSceneStatus() != LOADING) {
                    CreateSceneData *data = [CreateSceneData alloc];
                    [data setMarkerID: markerID];
                    [data setScene:currentSceneController];
                    currentSceneController->setSceneStatus(LOADING);
                    [self showLoading];
                    isRunningThread = true;
                    [self performSelectorInBackground: @selector(reloadResourceInScene:) withObject: data ]; // loading
                }
            }
        }
        
        
        if (canRender) {
            // can do it actions !
            const QCAR::ImageTargetResult* imageTargetResult = static_cast<const QCAR::ImageTargetResult*>(result);
            
            for (int vbID = 0; vbID < imageTargetResult->getNumVirtualButtons(); ++vbID) {
                const QCAR::VirtualButtonResult* buttonResult = imageTargetResult->getVirtualButtonResult(vbID);
                const QCAR::VirtualButton& button = buttonResult->getVirtualButton();
     
                currentSceneController->touchesVirtualButtonBegan(button.getName(), buttonResult->isPressed(),button.getID());
            }
        }
        
        
        
        if (currentSceneController && (currentSceneController->showMarkerHolderB || currentSceneController->waitTextureFromFrame)) {
            currentResultTakePicForMarker = [self frameMarkerWork:state markerID:markerID];
            numShowFrames = 0;
        }
        
    }

    
    XMatrix4Mult(&resultM4, &viewMatrix, &transM4);
    
    XMatrix4 test222Invers;
    XMatrix4Inverse(&test222Invers,&resultM4);
    
    camera->getInfo()->lookAtPos.x = test222Invers.m[12];
    camera->getInfo()->lookAtPos.y = test222Invers.m[13];
    camera->getInfo()->lookAtPos.z = test222Invers.m[14];
    
    camera->setModelViewMatrix(&resultM4.m[0]);
    
    
    if (currentSceneController && currentSceneController->isValid()) {
        if (currentSceneController->cameraStyle != cameraStyle) {
            [self changeCameraStyle:currentSceneController->cameraStyle];
            
            if (self.delegate && [self.delegate respondsToSelector:@selector(showHint:)]) {
                if (cameraStyle == CAMERA_STYLE_FLIP) {
                    [self.delegate showHint:currentSceneController->flipHintID];
                }else {
                    [self.delegate showHint:currentSceneController->hintID];
                }
            }
            
        }
    }
    
    if (cameraStyle == CAMERA_STYLE_FLIP && currentSceneController) {
        if (currentSceneController->isValid()) {
            canRender = true;
            
            if (lastIndexIndexesObject != currentSceneController->getCurrentIndexInClassObject()) {
                
                lastIndexIndexesObject = currentSceneController->getCurrentIndexInClassObject();
                
                
                if (self.delegate && [self.delegate respondsToSelector:@selector(indexInObjectChanged:)] && lastIndexIndexesObject != -1) {
                    [self.delegate indexInObjectChanged:lastIndexIndexesObject];
                }
                
                
            }
            
        }
        
        
        
        
        framesWithoutMarker = 0;
    }
    
    
    if (framesWithoutMarker > maxFramesWithOutMarker) {
        
        [self clearScene];
        
    }else {
        if (canRender) {
            numShowFrames++;
            
            if (numShowFrames > stableFrameCount) {
                showScene = true;
                currentSceneController->statusShow(1);
                
                if (!currentSceneController->waitTextureFromFrame && timerShowended) {
                    currentSceneController->update();
                }
                
                render->render();
                
                if (!currentSceneController->waitTextureFromFrame && timerShowended) {
                    currentSceneController->endUpdate();
                }
            }
            
            numNotShowedFrames = 0;
        }else if (currentSceneController){
            numShowFrames = 0;
            numNotShowedFrames++;
            currentSceneController->statusShow(0);
        }
        
        if (numNotShowedFrames > maxNotShowFrames) {
            showScene = false;
        }

    }
    
    [self presentFramebuffer];
}

-(void)clearScene {
    _lastMarkerId = 0;
    if (currentSceneController) {
        currentSceneController->removeScene();
        delete currentSceneController;
        currentSceneController = NULL;
        sceneLibs.clear();
        mainSceneGraph->clear();
        mainSceneGraph->createListObjects();
    }
}


-(void)reloadChilds:(SceneNode*)node {
    for (int i = 0; i < node->childs.size(); i++) {
        [self reloadChilds:node->childs[i]];
    }
    
    Mesh*mesh = node->getMesh();
    Material*mat = node->getMaterial();
    if (mat != nullptr) 
    mat->reloadTextures();
    if (mesh && mesh->isDamaged) {
        loader->reloadChunkInMesh(mesh);
    };
}



-(void)reloadResourceInScene:(CreateSceneData*)data {
    SceneController*controller = [data getScene];
    
    [EAGLContext setCurrentContext:loadingContext];
    
    for (uint i = 0; i < controller->childs.size(); i++) {
        SceneNode*node = controller->childs[i];
        
        [self reloadChilds:node];
        
        
    }
    
    controller->matValid = true;

    
    
    glFlush();
    [EAGLContext setCurrentContext: nil ];
    [self performSelectorOnMainThread:@selector(reloadEnd:) withObject:data waitUntilDone:NO];
    
}

-(void)reloadEnd:(CreateSceneData*)data {
    SceneController*controller = [data getScene];
    controller->setSceneStatus(IDLE_MESH);
    [self hideLoadingAnimation];
    isRunningThread = false;
}





-(void)createNewScene:(CreateSceneData*)data {
    [EAGLContext setCurrentContext:loadingContext];
    std::string markerID =[_markerPrefix UTF8String];
    markerID += [data getMarkerID];
    std::string markerPath = serverPath+"/markers/"+markerID+".xml";
    
    NSString* nsmarkerPath = [NSString stringWithCString:markerPath.c_str() encoding:NSUTF8StringEncoding];
    
    if (![CacheControll isCachedAtPath:nsmarkerPath]) {
        NSError* error = nil;
        
        NSData *fileData = [NSData dataWithContentsOfURL:[NSURL URLWithString:nsmarkerPath] options: 0 error: &error];
        if (fileData == nil) {
            NSLog(@"Failed to read file, error %@", error);
        }else {
            [CacheControll cacheData:fileData fromServerPath:nsmarkerPath];
        }
    }
    
    NSString * fileCachedPath   = [CacheControll getPathFromCacheAtPath:nsmarkerPath];
    if (!fileCachedPath) {
        NSLog(@"Failed to read marker file");
        return;
    }
    std::string localMarkerPath = [fileCachedPath cStringUsingEncoding:NSUTF8StringEncoding];
    
    NSString * s = [NSString stringWithFormat:@"Library/Caches/%s/",serverPath.c_str(),nil];
    NSString *FilePath = [NSHomeDirectory() stringByAppendingPathComponent:s];
    std::string localFilePath = [FilePath cStringUsingEncoding:NSUTF8StringEncoding];
    
    
    // create new scene here !
    
    SceneController*sceneController = new SceneController(localMarkerPath,loader,localFilePath,&materialCache,textureCache,&meshsCacheMap,soundSystem,animationsCache,&skinAnimationsCache);
    sceneController->widthMarker = [data getWidth];
    sceneController->heightMarker = [data getHeight];
    
    sceneController->markerID = markerID;
    sceneController->childsList = &mainSceneGraph->listObjects;
    // cache all resource !
    std::vector<std::string> resources = sceneController->getResources();
    for (uint i = 0 ; i < resources.size(); i++) {
        std::string pathForRes = serverPath+resources[i];
        NSString* nsresPath = [NSString stringWithCString:pathForRes.c_str() encoding:NSUTF8StringEncoding];
        if (![CacheControll isCachedAtPath:nsresPath]) {
            NSError* error = nil;
            
            double progress =(float) ((float)i/(float)resources.size());
            [self performSelectorOnMainThread:@selector(updateLoadingProgress:) withObject:[NSNumber numberWithDouble:progress] waitUntilDone:NO];
            
            
            NSString*pathCorrected = [nsresPath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
            
            NSData*data = [NSData dataWithContentsOfURL:[NSURL URLWithString:pathCorrected] options: 0 error: &error];
            if (data == nil) {
                NSLog(@"Failed to read file, error %@", error);
            }else {
                [CacheControll cacheData:data fromServerPath:nsresPath];
            }
        }
    }
    // load scene
    
    
    NSString*lang = [LocalizationSystem sharedLocalSystem].langID ;
    
    std::string langID = [[lang substringToIndex:2] UTF8String];
    
    
    NSLog(@"lang %s",langID.c_str());
    
    sceneController->langString = langID;
    sceneController->loadScene();
    
    
    int targetID = [data getTargetID];
    
   
    
    std::vector<XVirtualButton> vector;
    sceneController->getVirtualsButtons(&vector);
    
    if (vector.size()) {
        
        QCAR::ObjectTracker* it = reinterpret_cast<QCAR::ObjectTracker*>(QCAR::TrackerManager::getInstance().getTracker(QCAR::ObjectTracker::getClassType()));
        
        // Deactivate the data set prior to reconfiguration:
        it->deactivateDataSet(_dataSet);
        
        // we retrive the image target from the dataset
        QCAR::Trackable* trackable = _dataSet->getTrackable(targetID);
        assert(trackable);
        
        assert(trackable->getType().isOfType(QCAR::ImageTarget::getClassType()));
        QCAR::ImageTarget* imageTarget = static_cast<QCAR::ImageTarget*>(trackable);
        
        
        
        for (int i = 0; i < imageTarget->getNumVirtualButtons(); i++) {
            imageTarget->destroyVirtualButton(imageTarget->getVirtualButton(i));
        }
        
        
        for (int i = 0; i < vector.size(); i++) {
            XVirtualButton button = vector[i];
            
            QCAR::Rectangle vbRectangle(button.x1, button.y1, button.x2, button.y2);
            QCAR::VirtualButton*virtualButton = imageTarget->createVirtualButton(button.name.c_str(), vbRectangle);
            if (virtualButton) {
                
                virtualButton->setEnabled(true);
                virtualButton->setSensitivity(QCAR::VirtualButton::MEDIUM);
                
            }
            
        }
        
        it->activateDataSet(_dataSet);

    }
    
    
    [data setScene:sceneController];
    
    glFlush();
    [EAGLContext setCurrentContext: nil ];
    
    markerIDNS = nsmarkerPath;
    
    offetEditorX = sceneController->moveSceneX;
    offetEditorY = sceneController->moveSceneY;
    offetEditorZ = sceneController->moveSceneZ;
    
    offetEditorScaleX = sceneController->scaleSceneX;
    offetEditorScaleY = sceneController->scaleSceneY;
    offetEditorScaleZ = sceneController->scaleSceneZ;
    
    offsetEditorRotationX = sceneController->rotationSceneX;
    offsetEditorRotationY = sceneController->rotationSceneY;
    offsetEditorRotationZ = sceneController->rotationSceneZ;
    
    XVector3 degress(offsetEditorRotationX,offsetEditorRotationY,offsetEditorRotationZ);
    degress*=DEG_TO_RAD;
    
    XQuaternionFromEuler(&offsetEditorQuaternion,&degress);
    
    [self performSelectorOnMainThread:@selector(createNewSceneEnd:) withObject:data waitUntilDone:NO];
    
}


-(void)createNewSceneEnd:(CreateSceneData*)newScene {
    
    isRunningThread = false;
    NSLog(@"createSceneEnd:%s",[newScene getMarkerID].c_str());
    SceneController*sceneController = [newScene getScene];
    
        
    
    if (!sceneController->isValidController) {
        [self hideLoadingAnimationProgress];
        sceneController->clearScene();
        sceneLibs.clear();
        delete sceneController;
        return;
    }
    
    
    
    sceneLibs[[newScene getMarkerID]] = sceneController;
    
    
    if (sceneController->revertSwipesEvent) {
        swipeDownGestureController.enabled = true;
        swipeUpGestureController.enabled = true;
        
        XLOG("swipe is reverted");
    }else {
        XLOG("swipe is normal");
        swipeDownGestureController.enabled = false;
        swipeUpGestureController.enabled = false;
    }
    
    mainSceneGraph->clear();
    mainSceneGraph->addObjectinTree(sceneController);
    mainSceneGraph->createListObjects();
    
    
    
    if (self.delegate && [self.delegate respondsToSelector:@selector(showHint:)]) {
        [self.delegate showHint:sceneController->hintID];
    }
    
    
    [self hideLoadingAnimationProgress];
}



-(void)updateLoadingProgress:(NSNumber*)progress {
    if (self.delegate && [self.delegate respondsToSelector:@selector(updateProgress:)]) {
        [self.delegate updateProgress:[progress doubleValue]];
    }
}
- (void) showLoadingAnimationProgress {
    if (self.delegate && [self.delegate respondsToSelector:@selector(loadingResourceProgressBar)]) {
        [self.delegate loadingResourceProgressBar];
    }
}
- (void) hideLoadingAnimationProgress {
    if (self.delegate && [self.delegate respondsToSelector:@selector(loadingResourceProgressBarEnd)]) {
        [self.delegate loadingResourceProgressBarEnd];
    }
}



- (void) showLoadingAnimation {
    if (self.delegate && [self.delegate respondsToSelector:@selector(loadingResource)]) {
        [self.delegate loadingResource];
    }
}
- (void) hideLoadingAnimation {
    if (self.delegate && [self.delegate respondsToSelector:@selector(loadingResourceEnd)]) {
        [self.delegate loadingResourceEnd];
    }
}

- (void)loadingFile:(NSString*)path {
    
    NSString * fileCachedPath = [CacheControll getPathFromCacheAtPath:path];
    if (!fileCachedPath) {
        NSData*fileData = [NSData dataWithContentsOfURL:[NSURL URLWithString:path]];
        [CacheControll cacheData:fileData fromServerPath:path];
    }

    [self performSelectorOnMainThread:@selector(fileLoaded:) withObject:path waitUntilDone:NO];
}

- (void)fileLoaded:(NSString *)pathCached {
    currentLoadingFile = false;
    idSM = -1;
    [self hideLoadingAnimation];
}



-(void)loadFile:(NSString*)path {
    [self showLoadingAnimation];
    currentLoadingFile = true;
    NSOperationQueue *queue = [NSOperationQueue new];
    NSInvocationOperation *operation = [[NSInvocationOperation alloc]
                                        initWithTarget:self
                                        selector:@selector(loadingFile:)
                                        object:path];
    [queue addOperation:operation];
    
}








- (void)finishOpenGLESCommands
{
    // Called in response to applicationWillResignActive.  The render loop has
    // been stopped, so we now make sure all OpenGL ES commands complete before
    // we (potentially) go into the background
    if (context) {
        [EAGLContext setCurrentContext:context];
        glFinish();
    }
}
- (void)freeOpenGLESResources
{
    // Called in response to applicationDidEnterBackground.  Free easily
    // recreated OpenGL ES resources
    [self deleteFramebuffer];
    glFinish();
}
- (void)deleteFramebuffer
{
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer) {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer) {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
        
        if (depthRenderbuffer) {
            glDeleteRenderbuffers(1, &depthRenderbuffer);
            depthRenderbuffer = 0;
        }
    }
}
- (void)setFramebuffer
{
    // The EAGLContext must be set for each thread that wishes to use it.  Set
    // it the first time this method is called (on the render thread)
    if (context != [EAGLContext currentContext]) {
        [EAGLContext setCurrentContext:context];
    }
    
    if (!defaultFramebuffer) {
        // Perform on the main thread to ensure safe memory allocation for the
        // shared buffer.  Block until the operation is complete to prevent
        // simultaneous access to the OpenGL context
        [self performSelectorOnMainThread:@selector(createFramebuffer) withObject:self waitUntilDone:YES];
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
}

-(void)createFramebuffer {
    if (context) {
        glGenFramebuffers(1, &defaultFramebuffer);
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }
        GLint backingWidth,backingHeight;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
        
        
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        render->setDefaultBuffer(defaultFramebuffer);
    }
}

- (BOOL)presentFramebuffer
{
    // setFramebuffer must have been called before presentFramebuffer, therefore
    // we know the context is valid and has been set for this (render) thread
    
    // Bind the colour render buffer and present it
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    return [context presentRenderbuffer:GL_RENDERBUFFER];
}

/*

 */
-(UIImage *) drawableToCGImage
{
       [self setFramebuffer];
    GLint backingWidth, backingHeight;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    CGRect s = CGRectMake(0, 0, backingWidth, backingHeight);
    
    uint8_t *buffer = (uint8_t *) malloc(s.size.width * s.size.height * 4);
    glReadPixels(0, 0, s.size.width, s.size.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, buffer, s.size.width * s.size.height * 4, NULL);
    CGImageRef iref = CGImageCreate(s.size.width, s.size.height, 8, 32, s.size.width * 4, CGColorSpaceCreateDeviceRGB(),
                                    kCGBitmapByteOrderDefault, ref, NULL, true, kCGRenderingIntentDefault);
    
    size_t width = CGImageGetWidth(iref);
    size_t height = CGImageGetHeight(iref);
    size_t length = width * height * 4;
    uint32_t *pixels = (uint32_t *)malloc(length);
    
    CGContextRef cgcontext = CGBitmapContextCreate(pixels, width, height, 8, width * 4,
                                                   CGImageGetColorSpace(iref), kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Big);
    
    CGAffineTransform transform = CGAffineTransformIdentity;
    transform = CGAffineTransformMakeTranslation(0.0f, height);
    transform = CGAffineTransformScale(transform, 1.0, -1.0);
    CGContextConcatCTM(cgcontext, transform);
    CGContextDrawImage(cgcontext, CGRectMake(0.0f, 0.0f, width, height), iref);
    CGImageRef outputRef = CGBitmapContextCreateImage(cgcontext);
    
    UIImage* outputImage = [UIImage imageWithCGImage:outputRef];
    
    CGDataProviderRelease(ref);
    CGImageRelease(iref);
    CGContextRelease(cgcontext);
    CGImageRelease(outputRef);
    
    free(pixels);
    free(buffer);
    
    
    return outputImage;
  
}

@end
