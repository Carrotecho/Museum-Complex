/*==============================================================================
 Copyright (c) 2012-2013 Qualcomm Connected Experiences, Inc.
 All Rights Reserved.
 ==============================================================================*/

#import "SampleApplicationSession.h"
#import <QCAR/QCAR.h>
#import <QCAR/QCAR_iOS.h>
#import <QCAR/Tool.h>
#import <QCAR/Renderer.h>
#import <QCAR/CameraDevice.h>
#import <QCAR/VideoBackgroundConfig.h>
#import <QCAR/UpdateCallback.h>

namespace {
    // --- Data private to this unit ---
    
    // instance of the seesion
    // used to support the QCAR callback
    // there should be only one instance of a session
    // at any given point of time
    SampleApplicationSession* instance = nil;
    
    // QCAR initialisation flags (passed to QCAR before initialising)
    int mQCARInitFlags;
    
    // camera to use for the session
    QCAR::CameraDevice::CAMERA mCamera = QCAR::CameraDevice::CAMERA_DEFAULT;
    
    // class used to support the QCAR callback mechanism
    class VuforiaApplication_UpdateCallback : public QCAR::UpdateCallback {
        virtual void QCAR_onUpdate(QCAR::State& state);
    } qcarUpdate;

    // NSerror domain for errors coming from the Sample application template classes
    NSString * SAMPLE_APPLICATION_ERROR_DOMAIN = @"vuforia_sample_application";
}

@interface SampleApplicationSession ()

@property (nonatomic, readwrite) CGSize mARViewBoundsSize;
@property (nonatomic, readwrite) UIInterfaceOrientation mARViewOrientation;
@property (nonatomic, readwrite) BOOL mIsActivityInPortraitMode;
@property (nonatomic, readwrite) BOOL cameraIsActive;

// SampleApplicationControl delegate (receives callbacks in response to particular
// events, such as completion of Vuforia initialisation)


@end


@implementation SampleApplicationSession
@synthesize viewport;

- (id)initWithDelegate:(id<SampleApplicationControl>) delegate
{
    self = [super init];
    if (self) {
        self.delegate = delegate;
        
        // we keep a reference of the instance in order to implemet the QCAR callback
        instance = self;
        QCAR::registerCallback(&qcarUpdate);
    }
    return self;
}

- (void)dealloc
{
    instance = nil;
    [self setDelegate:nil];
    [super dealloc];
}

// build a NSError
- (NSError *) NSErrorWithCode:(int) code {
    return [NSError errorWithDomain:SAMPLE_APPLICATION_ERROR_DOMAIN code:code userInfo:nil];
}

- (void) NSErrorWithCode:(int) code error:(NSError **) error{
    if (error != NULL) {
        *error = [self NSErrorWithCode:code];
    }
}

// Determine whether the device has a retina display
- (BOOL)isRetinaDisplay
{
    // If UIScreen mainScreen responds to selector
    // displayLinkWithTarget:selector: and the scale property is 2.0, then this
    // is a retina display
    return ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && 2.0 == [UIScreen mainScreen].scale);
}

// Initialize the Vuforia SDK
- (void) initAR:(int) QCARInitFlags ARViewBoundsSize:(CGSize) ARViewBoundsSize orientation:(UIInterfaceOrientation) ARViewOrientation {
    self.cameraIsActive = NO;
    self.cameraIsStarted = NO;
    mQCARInitFlags = QCARInitFlags;
    self.isRetinaDisplay = [self isRetinaDisplay];
    self.mARViewOrientation = ARViewOrientation;

    // If this device has a retina display, we expect the view bounds to
    // have been scaled up by a factor of 2; this allows it to calculate the size and position of
    // the viewport correctly when rendering the video background
    // The ARViewBoundsSize is the dimension of the AR view as seen in portrait, even if the orientation is landscape
    
    CGSize viewBoundsSize = [self getCurrentARViewBoundsSize];
    int smallerSize = MIN(viewBoundsSize.width, viewBoundsSize.height);
    int largerSize = MAX(viewBoundsSize.width, viewBoundsSize.height);
    
    
    self.mARViewBoundsSize = CGSizeMake(smallerSize, largerSize);
    
    // Initialising QCAR is a potentially lengthy operation, so perform it on a
    // background thread
    [self performSelectorInBackground:@selector(initQCARInBackground) withObject:nil];
}

// Initialise QCAR
// (Performed on a background thread)
- (void)initQCARInBackground
{
    // Background thread must have its own autorelease pool
    @autoreleasepool {
        QCAR::setInitParameters(mQCARInitFlags,"ARVP75P/////AAAAAf0CQMd0PEiAsTReuEKHX1+LFKbB3+rd3c2V+DsSlMKzMsbw4RRMPFbw58GXfwiLqMcNh7s9DWVoz7r0O8yfgVjYG0BBw6I6Jksve9d7VXUhXTVhjCggXJjYIEUJEP3Wq1dez4BSlTWxwJiXeuw6RNLjom8mhdbJcNB6pE8Zsrgv1Rzejy0jtYQVCjQ+cL2ty3/HhkigV4u6bKALiUyyAnYERSexAzIdDACCICJjnpY/4jWFRXw4T8+sAxF72ieO5pkCRekabSDRC/jeTju0wy/eEMrdrbJ2WnnaTFeH0PkK2D8ysoxLzRl+CkR/qxyAvfkhP/QO+h8/RJdgnL07w0Ao4sv+Kp5RE/ixtea6Nw9d");
        
        // QCAR::init() will return positive numbers up to 100 as it progresses
        // towards success.  Negative numbers indicate error conditions
        NSInteger initSuccess = 0;
        do {
            initSuccess = QCAR::init();
        } while (0 <= initSuccess && 100 > initSuccess);
        
        if (100 == initSuccess) {
            // We can now continue the initialization of Vuforia
            // (on the main thread)
            [self performSelectorOnMainThread:@selector(prepareAR) withObject:nil waitUntilDone:NO];
        }
        else {
            // Failed to initialise QCAR
            [self.delegate onInitARDone:[self NSErrorWithCode:E_INITIALIZING_QCAR]];
        }
    }
}

// Resume QCAR
- (bool) resumeAR:(NSError **)error {
    
    QCAR::onResume();
    
    // if the camera was previously started, but not currently active, then
    // we restart it
    if ((self.cameraIsStarted) && (! self.cameraIsActive)) {
        NSLog(@"resumeAR");
        // initialize the camera
        if (! QCAR::CameraDevice::getInstance().init(mCamera)) {
            [self NSErrorWithCode:E_INITIALIZING_CAMERA error:error];
            return NO;
        }
        
        // start the camera
        if (!QCAR::CameraDevice::getInstance().start()) {
            [self NSErrorWithCode:E_STARTING_CAMERA error:error];
            return NO;
        }
        
        if ((self.delegate != nil) && [self.delegate respondsToSelector:@selector(onResume)]) {
            [self.delegate onResume];
        }
        
        self.cameraIsActive = YES;
    }
    return YES;
}


// Pause QCAR
- (bool)pauseAR:(NSError **)error {
    
    if (self.cameraIsActive) {
        NSLog(@"pauseAR");
        // Stop and deinit the camera
        if(! QCAR::CameraDevice::getInstance().stop()) {
            [self NSErrorWithCode:E_STOPPING_CAMERA error:error];
            return NO;
        }
        if(! QCAR::CameraDevice::getInstance().deinit()) {
            [self NSErrorWithCode:E_DEINIT_CAMERA error:error];
            return NO;
        }
        self.cameraIsActive = NO;
    }
    QCAR::onPause();
    return YES;
}

- (void) QCAR_onUpdate:(QCAR::State *) state {
    if ((self.delegate != nil) && [self.delegate respondsToSelector:@selector(onQCARUpdate:)]) {
        [self.delegate onQCARUpdate:state];
    }
}
- (CGSize)getCurrentARViewBoundsSize
{
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGSize viewSize = screenBounds.size;
    
    // If this device has a retina display, scale the view bounds
    // for the AR (OpenGL) view
    if (YES == self.isRetinaDisplay) {
        viewSize.width *= 2.0;
        viewSize.height *= 2.0;
    }
    return viewSize;
}


- (void) prepareAR  {
    // Tell QCAR we've created a drawing surface
    QCAR::onSurfaceCreated();
    
    
    
    // Frames from the camera are always landscape, no matter what the
    // orientation of the device.  Tell QCAR to rotate the video background (and
    // the projection matrix it provides to us for rendering our augmentation)
    // by the proper angle in order to match the EAGLView orientation
    if (self.mARViewOrientation == UIInterfaceOrientationPortrait)
    {
        QCAR::onSurfaceChanged(self.mARViewBoundsSize.width, self.mARViewBoundsSize.height);
        QCAR::setRotation(QCAR::ROTATE_IOS_90);
        
        self.mIsActivityInPortraitMode = YES;
    }
    else if (self.mARViewOrientation == UIInterfaceOrientationPortraitUpsideDown)
    {
        QCAR::onSurfaceChanged(self.mARViewBoundsSize.width, self.mARViewBoundsSize.height);
        QCAR::setRotation(QCAR::ROTATE_IOS_270);
        
        self.mIsActivityInPortraitMode = YES;
    }
    else if (self.mARViewOrientation == UIInterfaceOrientationLandscapeLeft)
    {
        QCAR::onSurfaceChanged(self.mARViewBoundsSize.height, self.mARViewBoundsSize.width);
        QCAR::setRotation(QCAR::ROTATE_IOS_180);
        
        self.mIsActivityInPortraitMode = NO;
    }
    else if (self.mARViewOrientation == UIInterfaceOrientationLandscapeRight)
    {
        QCAR::onSurfaceChanged(self.mARViewBoundsSize.height, self.mARViewBoundsSize.width);
        QCAR::setRotation(1);
        
        self.mIsActivityInPortraitMode = NO;
    }
    
    if (! [self.delegate doInitTrackers]) {
        [self.delegate onInitARDone:[self NSErrorWithCode:E_INIT_TRACKERS]];
        return;
    }
    [self.delegate onInitARDone:nil];
}

- (void) initTracker {
    // ask the application to initialize its trackers
    if (! [self.delegate doInitTrackers]) {
        [self.delegate onInitARDone:[self NSErrorWithCode:E_INIT_TRACKERS]];
        return;
    }
    [self loadTrackerData];
}


- (void) loadTrackerData {
    // Loading tracker data is a potentially lengthy operation, so perform it on
    // a background thread
    [self performSelectorInBackground:@selector(loadTrackerDataInBackground) withObject:nil];
    
}

// *** Performed on a background thread ***
- (void)loadTrackerDataInBackground
{
    // Background thread must have its own autorelease pool
    @autoreleasepool {
        // the application can now prepare the loading of the data
        if(! [self.delegate doLoadTrackersData]) {
            [self.delegate onInitARDone:[self NSErrorWithCode:E_LOADING_TRACKERS_DATA]];
            return;
        }
    }
    
    [self.delegate onInitARDone:nil];
}

// Configure QCAR with the video background size
- (void)configureVideoBackgroundWithViewWidth:(float)viewWidth andHeight:(float)viewHeight
{
    // Get the default video mode
    QCAR::CameraDevice& cameraDevice = QCAR::CameraDevice::getInstance();
    QCAR::VideoMode videoMode = cameraDevice.getVideoMode(QCAR::CameraDevice::MODE_DEFAULT);
    
    
    
    // Configure the video background
    QCAR::VideoBackgroundConfig config;
    config.mEnabled = true;
    //config.mSynchronous = true;
    config.mPosition.data[0] = 0.0f;
    config.mPosition.data[1] = 0.0f;
    
    // Determine the orientation of the view.  Note, this simple test assumes
    // that a view is portrait if its height is greater than its width.  This is
    // not always true: it is perfectly reasonable for a view with portrait
    // orientation to be wider than it is high.  The test is suitable for the
    // dimensions used in this sample
    if (self.mIsActivityInPortraitMode) {
        // --- View is portrait ---
        
        // Compare aspect ratios of video and screen.  If they are different we
        // use the full screen size while maintaining the video's aspect ratio,
        // which naturally entails some cropping of the video
        float aspectRatioVideo = (float)videoMode.mWidth / (float)videoMode.mHeight;
        float aspectRatioView = viewHeight / viewWidth;
        
        if (aspectRatioVideo < aspectRatioView) {
            // Video (when rotated) is wider than the view: crop left and right
            // (top and bottom of video)
            
            // --============--
            // - =          = _
            // - =          = _
            // - =          = _
            // - =          = _
            // - =          = _
            // - =          = _
            // - =          = _
            // - =          = _
            // --============--
            
            config.mSize.data[0] = (int)videoMode.mHeight * (viewHeight / (float)videoMode.mWidth);
            config.mSize.data[1] = (int)viewHeight;
        }
        else {
            // Video (when rotated) is narrower than the view: crop top and
            // bottom (left and right of video).  Also used when aspect ratios
            // match (no cropping)
            
            // ------------
            // -          -
            // -          -
            // ============
            // =          =
            // =          =
            // =          =
            // =          =
            // =          =
            // =          =
            // =          =
            // =          =
            // ============
            // -          -
            // -          -
            // ------------
            
            config.mSize.data[0] = (int)viewWidth;
            config.mSize.data[1] = (int)videoMode.mWidth * (viewWidth / (float)videoMode.mHeight);
        }
    }
    else {
        // --- View is landscape ---
        float temp = viewWidth;
        viewWidth = viewHeight;
        viewHeight = temp;
        
        // Compare aspect ratios of video and screen.  If they are different we
        // use the full screen size while maintaining the video's aspect ratio,
        // which naturally entails some cropping of the video
        float aspectRatioVideo = (float)videoMode.mWidth / (float)videoMode.mHeight;
        float aspectRatioView = viewWidth / viewHeight;
        
        if (aspectRatioVideo < aspectRatioView) {
            // Video is taller than the view: crop top and bottom
            
            // --------------------
            // ====================
            // =                  =
            // =                  =
            // =                  =
            // =                  =
            // ====================
            // --------------------
            
            config.mSize.data[0] = (int)viewWidth;
            config.mSize.data[1] = (int)videoMode.mHeight * (viewWidth / (float)videoMode.mWidth);
        }
        else {
            // Video is wider than the view: crop left and right.  Also used
            // when aspect ratios match (no cropping)
            
            // ---====================---
            // -  =                  =  -
            // -  =                  =  -
            // -  =                  =  -
            // -  =                  =  -
            // ---====================---
            
            config.mSize.data[0] = (int)videoMode.mWidth * (viewHeight / (float)videoMode.mHeight);
            config.mSize.data[1] = (int)viewHeight;
        }
    }
    
    // Calculate the viewport for the app to use when rendering
    viewport.posX = ((viewWidth - config.mSize.data[0]) / 2) + config.mPosition.data[0];
    viewport.posY = (((int)(viewHeight - config.mSize.data[1])) / (int) 2) + config.mPosition.data[1];
    viewport.sizeX = config.mSize.data[0];
    viewport.sizeY = config.mSize.data[1];
 
#ifdef DEBUG_SAMPLE_APP
    NSLog(@"VideoBackgroundConfig: size: %d,%d", config.mSize.data[0], config.mSize.data[1]);
    NSLog(@"VideoMode:w=%d h=%d", videoMode.mWidth, videoMode.mHeight);
    NSLog(@"width=%7.3f height=%7.3f", viewWidth, viewHeight);
    NSLog(@"ViewPort: X,Y: %d,%d Size X,Y:%d,%d", viewport.posX,viewport.posY,viewport.sizeX,viewport.sizeY);
#endif
    
    // Set the config
    QCAR::Renderer::getInstance().setVideoBackgroundConfig(config);
}

// Start QCAR camera with the specified view size
- (bool)startCamera:(QCAR::CameraDevice::CAMERA)camera viewWidth:(float)viewWidth andHeight:(float)viewHeight error:(NSError **)error
{
    NSLog(@"startCamera");
    // initialize the camera
    if (! QCAR::CameraDevice::getInstance().init(camera)) {
        [self NSErrorWithCode:-1 error:error];
        return NO;
    }
    
    // start the camera
    if (!QCAR::CameraDevice::getInstance().start()) {
        [self NSErrorWithCode:-1 error:error];
        return NO;
    }
    
    // we keep track of the current camera to restart this
    // camera when the application comes back to the foreground
    mCamera = camera;
    
    // ask the application to start the tracker(s)
    if(! [self.delegate doStartTrackers] ) {
        [self NSErrorWithCode:-1 error:error];
        return NO;
    }
    
    // configure QCAR video background
    [self configureVideoBackgroundWithViewWidth:viewWidth andHeight:viewHeight];
    
    // Cache the projection matrix
    const QCAR::CameraCalibration& cameraCalibration = QCAR::CameraDevice::getInstance().getCameraCalibration();
    
    float nearPlane = 2.0f;
    float farPlane = 2000.0f;
    
    QCAR::Vec2F size = cameraCalibration.getSize();
    QCAR::Vec2F focalLength = cameraCalibration.getFocalLength();
    QCAR::Vec2F principalPoint = cameraCalibration.getPrincipalPoint();
    float dx = principalPoint.data[0] - size.data[0] / 2;
    float dy = principalPoint.data[1] - size.data[1] / 2;
    float x =  2.0f * focalLength.data[0] / size.data[0];
    float y = -2.0f * focalLength.data[1] / size.data[1];
    float a =  2.0f * dx / size.data[0];
    float b = -2.0f * (dy + 1.0f) / size.data[1];
    float c = (farPlane + nearPlane) / (farPlane - nearPlane);
    float d = -nearPlane * (1.0f + c);
    
    QCAR::Matrix44F mat;
    mat.data[0] = x;      mat.data[1] = 0.0f;   mat.data[2] = 0.0f;  mat.data[3] = 0.0f;
    mat.data[4] = 0.0f;   mat.data[5] = y;      mat.data[6] = 0.0f;  mat.data[7] = 0.0f;
    mat.data[8] = a;      mat.data[9] = b;      mat.data[10] = c;    mat.data[11] = 1.0f;
    mat.data[12] = 0.0f;  mat.data[13] = 0.0f;  mat.data[14] = d;    mat.data[15] = 0.0f;
    
    
    
    
    
    
    
     
     float fovyRadians = 90.0;
     float aspect = (float) size.data[0] / (float) size.data[1];
     float nearZ= 2.0f;
     float farZ= 2000.0f;
     
     float cotan = 1.0f / tanf(fovyRadians / 2.0f);
     
     mat.data[0] = cotan / aspect;
     mat.data[1] = 0.0f;
     mat.data[2] = 0.0f;
     mat.data[3] = 0.0f;
     
     mat.data[4] = 0.0f;
     mat.data[5] = cotan;
     mat.data[6] = 0.0f;
     mat.data[7] = 0.0f;
     
     mat.data[8]  = 0.0f;
     mat.data[9]  = 0.0f;
     mat.data[10] = (farZ + nearZ) / (nearZ - farZ);
     mat.data[11] = -1.0f;
     
     mat.data[12] = 0.0f;
     mat.data[13] = 0.0f;
     mat.data[14] = (2.0f * farZ * nearZ) / (nearZ - farZ);
     mat.data[15] = 0.0f;

     
    
    
    

    _projectionMatrix = mat;
    _projectionMatrix = QCAR::Tool::getProjectionGL(cameraCalibration, 1.0f, 20000.0f);
    
    //_projectionMatrix.data[0] = x;
    //_projectionMatrix.data[5] = y;
    
    
    return YES;
}


- (bool) startAR:(QCAR::CameraDevice::CAMERA)camera error:(NSError **)error {
    // Start the camera.  This causes QCAR to locate our EAGLView in the view
    // hierarchy, start a render thread, and then call renderFrameQCAR on the
    // view periodically
    NSLog(@"startAR");
    if (! [self startCamera: camera viewWidth:self.mARViewBoundsSize.width andHeight:self.mARViewBoundsSize.height error:error]) {
        return NO;
    }
    self.cameraIsActive = YES;
    self.cameraIsStarted = YES;

    return YES;
}

// Stop QCAR camera
- (bool)stopAR:(NSError **)error {
    NSLog(@"stopAR");
    // Stop the camera
    if (self.cameraIsActive) {
        // Stop and deinit the camera
        QCAR::CameraDevice::getInstance().stop();
        QCAR::CameraDevice::getInstance().deinit();
        self.cameraIsActive = NO;
    }
    self.cameraIsStarted = NO;

    // ask the application to stop the trackers
    if(! [self.delegate doStopTrackers]) {
        [self NSErrorWithCode:E_STOPPING_TRACKERS error:error];
        return NO;
    }
    
    // ask the application to unload the data associated to the trackers
    if(! [self.delegate doUnloadTrackersData]) {
        [self NSErrorWithCode:E_UNLOADING_TRACKERS_DATA error:error];
        return NO;
    }
    
    // ask the application to deinit the trackers
    if(! [self.delegate doDeinitTrackers]) {
        [self NSErrorWithCode:E_DEINIT_TRACKERS error:error];
        return NO;
    }
    
    // Pause and deinitialise QCAR
    QCAR::onPause();
    QCAR::deinit();
    
    return YES;
}

// stop the camera
- (bool) stopCamera:(NSError **)error {
    NSLog(@"stopCamera");
    if (self.cameraIsActive) {
        // Stop and deinit the camera
        QCAR::CameraDevice::getInstance().stop();
        QCAR::CameraDevice::getInstance().deinit();
        self.cameraIsActive = NO;
    } else {
        [self NSErrorWithCode:E_CAMERA_NOT_STARTED error:error];
        return NO;
    }
    self.cameraIsStarted = NO;
    
    // Stop the trackers
    if(! [self.delegate doStopTrackers]) {
        [self NSErrorWithCode:E_STOPPING_TRACKERS error:error];
        return NO;
    }

    return YES;
}

- (void) errorMessage:(NSString *) message {
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:SAMPLE_APPLICATION_ERROR_DOMAIN
                                                    message:message
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
}

////////////////////////////////////////////////////////////////////////////////
// Callback function called by the tracker when each tracking cycle has finished
void VuforiaApplication_UpdateCallback::QCAR_onUpdate(QCAR::State& state)
{
    if (instance != nil) {
        [instance QCAR_onUpdate:&state];
    }
}

@end
