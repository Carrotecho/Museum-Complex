//
// Created by Easy Proger on 10.11.15.
//

#ifndef DEVARMUSEUM_GLVIEW_H
#define DEVARMUSEUM_GLVIEW_H

#include <iostream>
#include <string>


#include "Camera.h"

#include <QCAR/Image.h>
#include <QCAR/QCAR.h>
#include <QCAR/State.h>
#include <QCAR/Tool.h>
#include <QCAR/Rectangle.h>
#include <QCAR/VirtualButton.h>
#include <QCAR/Renderer.h>
#include <QCAR/ImageTarget.h>
#include <QCAR/CameraDevice.h>
#include <QCAR/VirtualButtonResult.h>
#include <QCAR/ImageTargetResult.h>
#include <QCAR/TrackableResult.h>
#include <QCAR/VideoBackgroundConfig.h>
#include <QCAR/DataSet.h>
#include <QCAR/TrackerManager.h>
#include <QCAR/ImageTarget.h>
#include <QCAR/ObjectTracker.h>

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

#include "SceneController.h"


#define SHOW_AR 1

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include <gl/glew.h>
    #define  XLOG(...) printf(__VA_ARGS__)
#else
    #ifdef __ANDROID__


        #define GL_GLEXT_PROTOTYPES 1
        #include <GLES2/gl2.h>
    #else
        #include <OpenGLES/ES2/gl.h>
    #endif
#endif



#define GET_GLERRORA()                                                                                 \
{                                                                                                     \
GLenum code = glGetError();                                                                       \
while (code!=GL_NO_ERROR) {                                                                       \
XLOG("%s [line %d]  GL Error: %s\n",__FUNCTION__,__LINE__,xOpenGLStrError(code));  \
code = glGetError();                                                                          \
}                                                                                                 \
}

//#define SHOW_FPS 1

#define MAX_OBJECTS_IDS 1000
#define MESSAGE_NO_MESSAGE 0
#define MESSAGE_MARKER_CHANGE 1
#define MESSAGE_CREATE_NEW_SCENE 2
#define MESSAGE_RELOAD_SCENE 3
#define MESSAGE_CREATE_NEW_SCENE_ERROR 4
#define MESSAGE_CHANGE_INDEXES_OBJECT 5
#define MESSAGE_SHOW_HINT 6

typedef struct RenderResultInfo{
    RenderResultInfo() {
        messageID = MESSAGE_NO_MESSAGE;
        intval1 = 0;
        boolval1 = false;
        floatVal1 = 0;
    };
    RenderResultInfo(unsigned int _messageID) {
        messageID = _messageID;
        intval1 = 0;
        boolval1 = false;
        floatVal1 = 0;
    };
    unsigned int messageID;
    int intval1;
    bool boolval1;
    float floatVal1;
    std::vector<std::string> paths;
    
}RenderResultInfo;

const int32_t NUM_SAMPLES = 100;

class GLViewA {
public:
    
    int _langID;
    
    float current_FPS_;
    time_t tv_last_sec_;
    
    double last_tick_;
    int32_t tickindex_;
    double ticksum_;
    double ticklist_[NUM_SAMPLES];
    
    double UpdateTick( double current_tick );
    bool UpdateFPS( float &fFPS );
    
    void destroyEngine();
    void pauseGL();
    void resumeGL();
    
    bool needClearScene;
    bool isRunningThread;
    GLViewA();
    ~GLViewA(){

    };
    void clearScene();
    void stageEndCreateNewScene();
    void setDefaultFrameBuffer(uint idBuffer);
    void changeCameraStyle(uint style);
    XMatrix4 vuforiaperspectivematrix;
    
    uint cameraStyle;
    
    uint lastMarkerId;
    
    bool isInited;
    unsigned int shaderHideMarkerID;
    
     bool timerShowended;
    
    bool showScene;
    
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
    
    RenderResultInfo stageTwoCreateNewScene(std::string langID);
    std::string resourcePath;
    std::string serverPath;
    std::string localPath;
    std::string markerName;
    std::string markerPrefix;
    
    float colorsValues[256];
    void reloadSceneEnd();
    void reloadResourceInScene();
    void reloadChilds(SceneNode*node);
    RenderResultInfo createNewScene(std::string markID,float widthMarker,float heightMarker);
    void logToConsole(const char*message,...);
    void initEngine(const char* _resourcePath,bool*_markerChecker,const char* serverPath,const char*markerPrefix,const char*localPath);
    
    struct tagViewport {
        int posX;
        int posY;
        int sizeX;
        int sizeY;
    } viewport;
    
    QCAR::CameraDevice::CAMERA currentCamera;
    QCAR::DataSet*dataSetCurrent;
    
    
    int initTracker();
    void deinitTracker();
    int loadTrackerData(const char*pathDataSet);
    int destroyTrackerData();
    
    void configureVideoBackgroundWithViewWidth(float viewWidth,float viewHeight);
    
    bool*markerChecker;
    
    XMatrix4 viewMatrix,resultM4,transM4;
    
    void swipeRight();
    void swipeLeft();
    void swipeDown();
    void swipeUp();
    
    int lastIndexIndexesObject;
    
    Renderer* render;
    SceneGraph* mainSceneGraph;
    XAnimations*animationsCache;
    FileLoader*loader;
    SoundSystem* soundSystem;
    TextureCache* textureCache;
    MeshCache*meshCache;
    Light* defaultLight;
    
    QCAR::Matrix44F projectionMatrixVuforia;
    
    
    std::map<std::string,SkinningFrame*> skinAnimationsCache;
    std::map<std::string,SceneController*> sceneLibs;
    std::map<std::string,Material*> materialCache;
    std::map<std::string,Mesh*> meshsCacheMap;
    
    SceneController* currentSceneController;
    SceneController* tempSceneController;
    
    RenderResultInfo renderFrame();
    
    
    void setIsExtendedTrackingActivated(bool val);
    void setIsActivityInPortraitMode(bool val);
    void setScreenSize(int width,int height);
    
    void startCamera(int cameraNumber);
    void stopCamera();
    
    void setCameraProjectionMatrix();
    
    bool startExtendedTracking();
    bool stopExtendedTracking();
    bool autoFocus();
    bool setFocusMode(int focusID);
    bool activateFlash(bool flash);
    void touchesMove(float x,float y);
    void touchesBegan(float x,float y);
    void scale(float value);
    const char*getVersion();
private:
    bool isPaused;
    
    uint framesWithoutMarker;
    uint maxFramesWithOutMarker;
    int frameID;
    
    uint numNotShowedFrames;
    uint maxNotShowFrames;
    uint numShowFrames;
    uint stableFrameCount;
    
    
    Camera*cameraRenderer;
    
    bool mIsExtendedTrackingActivated;
    bool mIsActivityInPortraitMode;
    int screenWidth;
    int screenHeight;
};




#endif //DEVARMUSEUM_GLVIEW_H
