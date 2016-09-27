//
// Created by Easy Proger on 10.11.15.
//

#include "GLViewA.h"
//stl test

#include <string>
#include <time.h>

#include <sys/time.h>

GLViewA::GLViewA():last_tick_( 0.f ),
tv_last_sec_( 0 ),
tickindex_( 0 ),
ticksum_( 0 ) {
    
    isInited = false;
    tempSceneController = nullptr;
    mIsExtendedTrackingActivated = false;

    isPaused = false;
    
    needClearScene = false;
    offetEditorScaleX = 1.0;
    offetEditorScaleY = 1.0;
    offetEditorScaleZ = 1.0;
    
    offetEditorX = 0.0;
    offetEditorY = 0.0;
    offetEditorZ = 0.0;
    
    offsetEditorRotationX = 0.0;
    offsetEditorRotationY = 0.0;
    offsetEditorRotationZ = 0.0;
    
    lastIndexIndexesObject = -1;
    
    timerShowended = true;
    isRunningThread = false;
    cameraStyle = CAMERA_STYLE_VUFORIA;
    
    framesWithoutMarker = 0;
    maxFramesWithOutMarker = 150;
    frameID = 400;
    
    numNotShowedFrames = 0;
    maxNotShowFrames = 50;
    numShowFrames = 0;
    stableFrameCount = 50;
    
    for( int32_t i = 0; i < NUM_SAMPLES; ++i )
        ticklist_[i] = 0;
}

void GLViewA::swipeUp() {
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            
            if (currentSceneController->revertSwipesEvent) {
                this->swipeRight();
            }
            
            
        }
    }
}

void GLViewA::swipeDown() {
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            
            if (currentSceneController->revertSwipesEvent) {
                this->swipeLeft();
            }
            
            
        }
    }
}

void GLViewA::swipeLeft() {
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            
            currentSceneController->swipeController(2);
            
        }
    }
}

void GLViewA::swipeRight() {
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            
            currentSceneController->swipeController(1);
            
        }
    }
}

void GLViewA::scale(float value) {
    
    XLOG("Scaleis:%f",value);
    
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            currentSceneController->zoom(value);
        }
    }
}

void GLViewA::touchesBegan(float x,float y) {
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            currentSceneController->touchesBegan(x, y);
        }
    }
}

void GLViewA::touchesMove(float x,float y) {
    if (currentSceneController) {
        if (currentSceneController->isValid()) {
            currentSceneController->touchesMoved(x, y);
        }
    }
}


void GLViewA::destroyEngine() {
    isInited = false;
    XLOG("destroy engine");
    clearScene();
    
    delete textureCache;
    delete soundSystem;
    delete mainSceneGraph;
    delete meshCache;
    delete animationsCache;
    delete loader;
    delete render;

}

void GLViewA::initEngine(const char* _resourcePath,bool*_markerChecker,const char* _serverPath,const char*_markerPrefix,const char*_localPath) {
    XLOG("try init engine");
    
    if (isInited) {
        return;
    }
    isInited = true;
    
    resourcePath = _resourcePath;
    serverPath   = _serverPath;
    markerPrefix = _markerPrefix;
    localPath    = _localPath;
    
    
    markerChecker = (bool*)malloc(sizeof(bool)*MAX_OBJECTS_IDS);
    
    for (int i = 0; i < MAX_OBJECTS_IDS; i++) {
        markerChecker[i] = _markerChecker[i]; // copy data
        
    }
    
    
    
    cameraRenderer = new Camera();

    
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
    
    
    textureCache    = new TextureCache();
    soundSystem     = new SoundSystem();
    mainSceneGraph  = new SceneGraph();
    defaultLight    = new Light();
    
    
    std::map<uint, Mesh*> damagedMeshs;
    
    meshCache       = new MeshCache(&damagedMeshs);
    animationsCache = new XAnimations(&skinAnimationsCache);
    loader          = new FileLoader(meshCache,animationsCache);
 
    render          = new Renderer(cameraRenderer,resourcePath);
    mainSceneGraph  = new SceneGraph();
    
   
    cameraRenderer->setPerspective(90.0, WIDTH_VIEWPORT   ,HEIGHT_VIEWPORT, 1.0, 10000.0);
    cameraRenderer->resetModelViewMatrix();
    cameraRenderer->lookAt(vec3(0.0f, 500.0f, 2000.0f), vec3(0.0, 0.0, 0.0), vec3(0,1,0));
    
    render->setDefaultSceneGraph(mainSceneGraph);
    mainSceneGraph->addLightinTree(defaultLight);
    
    defaultLight->lookAt(vec3(1000.0f, 1000.0f, 1000.0f), vec3(0.0, 0.0, 0.0), vec3(0,1,0));
    glEnable(GL_DEPTH_TEST);
}

void GLViewA::setDefaultFrameBuffer(uint idBuffer) {
    render->setDefaultBuffer(idBuffer);
}

int GLViewA::initTracker() {
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::Tracker* tracker = trackerManager.initTracker(QCAR::ObjectTracker::getClassType());
    if (tracker == NULL)
    {
        printf("Failed to initialize ObjectTracker.");
        return 0;
    }
    
    printf("Successfully initialized ObjectTracker.");
    return 1;
}

void GLViewA::deinitTracker() {
    // Deinit the object tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    trackerManager.deinitTracker(QCAR::ObjectTracker::getClassType());
}



int GLViewA::loadTrackerData(const char*pathDataSet) {
    
    // Get the object tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::ObjectTracker* objectTracker = static_cast<QCAR::ObjectTracker*>(
                                                                           trackerManager.getTracker(QCAR::ObjectTracker::getClassType()));
    if (objectTracker == NULL)
    {
        printf("Failed to load tracking data set because the ObjectTracker has not"
            " been initialized.");
        return 0;
    }
    
    // Create the data sets:
    dataSetCurrent = objectTracker->createDataSet();
    if (dataSetCurrent == 0)
    {
        printf("Failed to create a new tracking data.");
        return 0;
    }
    
    
    
    // Load the data sets:
    if (!dataSetCurrent->load(pathDataSet, QCAR::STORAGE_ABSOLUTE))
    {
        printf("Failed to load data set.");
        return 0;
    }
    
    
    
    // Activate the data set:
    if (!objectTracker->activateDataSet(dataSetCurrent))
    {
        printf("Failed to activate data set.");
        return 0;
    }
    
    // if need extendTrack
    
    
    if (mIsExtendedTrackingActivated) {
        for (int tIdx = 0; tIdx < dataSetCurrent->getNumTrackables(); tIdx++) {
            QCAR::Trackable* trackable = dataSetCurrent->getTrackable(tIdx);
            trackable->startExtendedTracking();
        }
    }
    
    printf("Successfully loaded and activated data set.");
    return 1;
}


int GLViewA::destroyTrackerData() {
    
    // Get the object tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::ObjectTracker* objectTracker = static_cast<QCAR::ObjectTracker*>(
                                                                           trackerManager.getTracker(QCAR::ObjectTracker::getClassType()));
    if (objectTracker == NULL)
    {
        printf("Failed to destroy the tracking data set because the ObjectTracker has not"
            " been initialized.");
        return 0;
    }
    
    if (dataSetCurrent != 0)
    {
        if (objectTracker->getActiveDataSet() == dataSetCurrent &&
            !objectTracker->deactivateDataSet(dataSetCurrent))
        {
            printf("Failed to destroy the tracking data set StonesAndChips because the data set "
                "could not be deactivated.");
            return 0;
        }
        
        if (!objectTracker->destroyDataSet(dataSetCurrent))
        {
            printf("Failed to destroy the tracking data set StonesAndChips.");
            return 0;
        }
        
        printf("Successfully destroyed the data set StonesAndChips.");
        dataSetCurrent = 0;
    }
    return 1;
}


// Configure QCAR with the video background size


void GLViewA::configureVideoBackgroundWithViewWidth(float viewWidth,float viewHeight) {
    // Get the default video mode:
    QCAR::CameraDevice& cameraDevice = QCAR::CameraDevice::getInstance();
    QCAR::VideoMode videoMode = cameraDevice.
    getVideoMode(QCAR::CameraDevice::MODE_DEFAULT);
    
    
    // Configure the video background
    QCAR::VideoBackgroundConfig config;
    config.mEnabled = true;
    config.mPosition.data[0] = 0.0f;
    config.mPosition.data[1] = 0.0f;
    
    if (mIsActivityInPortraitMode)
    {
        //LOG("configureVideoBackground PORTRAIT");
        config.mSize.data[0] = videoMode.mHeight
        * (screenHeight / (float)videoMode.mWidth);
        config.mSize.data[1] = screenHeight;
        
        if(config.mSize.data[0] < screenWidth)
        {
            XLOG("Correcting rendering background size to handle missmatch between screen and video aspect ratios.");
            config.mSize.data[0] = screenWidth;
            config.mSize.data[1] = screenWidth *
            (videoMode.mWidth / (float)videoMode.mHeight);
        }
    }
    else
    {
        //LOG("configureVideoBackground LANDSCAPE");
        config.mSize.data[0] = screenWidth;
        config.mSize.data[1] = videoMode.mHeight
        * (screenWidth / (float)videoMode.mWidth);
        
        if(config.mSize.data[1] < screenHeight)
        {
            XLOG("Correcting rendering background size to handle missmatch between screen and video aspect ratios.");
            config.mSize.data[0] = screenHeight
            * (videoMode.mWidth / (float)videoMode.mHeight);
            config.mSize.data[1] = screenHeight;
        }
    }
    
    XLOG("Configure Video Background : Video (%d,%d), Screen (%d,%d), mSize (%d,%d)", videoMode.mWidth, videoMode.mHeight, screenWidth, screenHeight, config.mSize.data[0], config.mSize.data[1]);
    
    // Set the config:
    QCAR::Renderer::getInstance().setVideoBackgroundConfig(config);
}


void GLViewA::changeCameraStyle(uint style) {
    cameraStyle = style;
    if (cameraStyle == CAMERA_STYLE_VUFORIA) {
        cameraRenderer->setPerspectivematrix(&vuforiaperspectivematrix.m[0]);
    }else if (cameraStyle == CAMERA_STYLE_FLIP) {
        int viewport[4];;
        glGetIntegerv(GL_VIEWPORT, viewport);
        cameraRenderer->setPerspective(90.0, viewport[2]   ,viewport[3], 1.0, 1000.0);
        cameraRenderer->lookAt(vec3(0.0,0.0,50), vec3(0,0,0), vec3(0,1,0));
        XMatrix4MakeWithArray(&viewMatrix, &cameraRenderer->getModelViewMatrix()->m[0]);
    }
}


void GLViewA::pauseGL() {
    isPaused = true;
}
void GLViewA::resumeGL() {
    isPaused = false;
}




RenderResultInfo GLViewA::renderFrame() {
    
    
    RenderResultInfo renderResultInfo;
    renderResultInfo.intval1 = 0;
    renderResultInfo.boolval1 = 0;
    renderResultInfo.floatVal1 = 0;
    renderResultInfo.messageID = 0;
    
#ifdef SHOW_FPS
    renderResultInfo.floatVal1 = 0;
    UpdateFPS(renderResultInfo.floatVal1);
#endif
    renderResultInfo.messageID = MESSAGE_NO_MESSAGE;
    
    if (isPaused) {
        XLOG("is paused");
        return renderResultInfo;
    }
    
    // Clear color and depth buffer
    glDisable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QCAR::Renderer::getInstance().drawVideoBackground();
    QCAR::Renderer::getInstance().end();
    
    
    
    
    
    
    
    
    
    // Get the state from QCAR and mark the beginning of a rendering section
    QCAR::State state = QCAR::Renderer::getInstance().begin();

    if (isRunningThread) {
        return renderResultInfo;
    }
   
    
    if (needClearScene) {
        clearScene();
        needClearScene = false;
    }
    
    
    if (!state.getNumTrackableResults()) {
        this->lastMarkerId = 0;
        
        if (currentSceneController && currentSceneController->isValid()) {
            currentSceneController->lostMarker(true);
        }
        
    }else {
        if (currentSceneController && currentSceneController->isValid()) {
            currentSceneController->lostMarker(false);
        }
    }
    
    framesWithoutMarker++;
    bool canRender = false;
    for (int i = 0; i < state.getNumTrackableResults(); ++i) {
        // Get the trackable
        framesWithoutMarker = 0;
        const QCAR::TrackableResult* result = state.getTrackableResult(i);
        const QCAR::Trackable& trackable    = result->getTrackable();
        const QCAR::Matrix34F& poseMatrix   = result->getPose();
        
        
        
        if (cameraStyle == CAMERA_STYLE_VUFORIA) {
            QCAR::Matrix44F modelViewMatrix = QCAR::Tool::convertPose2GLMatrix(poseMatrix);
            modelViewMatrix.data[12]/=200.0;
            modelViewMatrix.data[13]/=200.0;
            modelViewMatrix.data[14]/=200.0;
            
            XMatrix4MakeWithArray(&viewMatrix, &modelViewMatrix.data[0]);
        }else if (cameraStyle == CAMERA_STYLE_FLIP) {
            cameraRenderer->lookAt(vec3(0.0,0.0,50), vec3(0,0,0), vec3(0,1,0));
            XMatrix4MakeWithArray(&viewMatrix, &cameraRenderer->getModelViewMatrix()->m[0]);
        }
        
        
        
        string markerID = trackable.getName();
        markerID = markerID.substr(0,10);
        
        
        
        std::string info = markerID.substr(0,6);
        
        int isInfo = std::strcmp("infosd", info.c_str());
        
        //markerID = "object_480";
        markerName = markerID;
        
        
        
        string cuttedMarkerId = markerID.substr (7,3);
        int markerIDInt = atoi(cuttedMarkerId.c_str());
        
        if (isInfo == 0) {
            
        }else {
            if (this->lastMarkerId != markerIDInt) {
                this->lastMarkerId = markerIDInt;
                if (MAX_OBJECTS_IDS > markerIDInt) {
                    RenderResultInfo info;
                    info.messageID = MESSAGE_MARKER_CHANGE;
                    info.intval1 = markerIDInt;
                    info.boolval1 = markerChecker[markerIDInt];
                    return info;
                }
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
                clearScene();
                numShowFrames = 0;
                return RenderResultInfo();
            }
            
        }else {
            int lll = this->lastMarkerId;
            clearScene();
            this->lastMarkerId = lll;
            
            sceneLibs.clear();
            sceneLibs[markerID] = NULL;
            
            animationsCache->clear();
            
            textureCache->correctCache();
            
            soundSystem->clearAllSoundData();

            
            const QCAR::ImageTarget &imageTarget = (const QCAR::ImageTarget &) trackable;
            QCAR::Vec3F targetSize = imageTarget.getSize();
            targetSize.data[0]/=200.0;
            targetSize.data[1]/=200.0;
            
         
            isRunningThread = true;
            return createNewScene(markerID, targetSize.data[0], targetSize.data[1]);
        }
        
        if (currentSceneController) {
            
            if (currentSceneController->isValid()) {
                canRender = true;
            }else {
                canRender = false;
                
                if (currentSceneController->getSceneStatus() != LOADING) {
                    
                    currentSceneController->setSceneStatus(LOADING);
                    XLOG("RELOAD NEED");
                    RenderResultInfo createSceneInfo;
                    createSceneInfo.messageID = MESSAGE_RELOAD_SCENE;
                    isRunningThread = true;
                    return createSceneInfo;
                    //reloadResourceInScene();
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
        
        
        
        //if (currentSceneController && (currentSceneController->showMarkerHolderB || currentSceneController->waitTextureFromFrame)) {
            //currentResultTakePicForMarker = [self frameMarkerWork:state markerID:markerID];
            //numShowFrames = 0;
        //}
       
    }
    
    
    XMatrix4Mult(&resultM4, &viewMatrix, &transM4);
    
    XMatrix4 test222Invers;
    XMatrix4Inverse(&test222Invers,&resultM4);
    
    cameraRenderer->getInfo()->lookAtPos.x = test222Invers.m[12];
    cameraRenderer->getInfo()->lookAtPos.y = test222Invers.m[13];
    cameraRenderer->getInfo()->lookAtPos.z = test222Invers.m[14];
    
    cameraRenderer->setModelViewMatrix(&resultM4.m[0]);
    
    
    if (currentSceneController && currentSceneController->isValid()) {
        if (currentSceneController->cameraStyle != cameraStyle) {
            changeCameraStyle(currentSceneController->cameraStyle);
            
            renderResultInfo.messageID = MESSAGE_SHOW_HINT;
            if (cameraStyle == CAMERA_STYLE_FLIP) {
                renderResultInfo.intval1 = currentSceneController->flipHintID;
            }else {
                renderResultInfo.intval1 = currentSceneController->hintID;
            }

            return renderResultInfo;
        }
    }
    
    if (cameraStyle == CAMERA_STYLE_FLIP && currentSceneController) {
        if (currentSceneController->isValid()) {
            canRender = true;
            
            int index = currentSceneController->getCurrentIndexInClassObject();
            
            if (lastIndexIndexesObject != index) {
                lastIndexIndexesObject = index;
                
                renderResultInfo.messageID = MESSAGE_CHANGE_INDEXES_OBJECT;
                renderResultInfo.intval1   = index;
                
            }
        }
        framesWithoutMarker = 0;
    }
    
    if (framesWithoutMarker > maxFramesWithOutMarker) {
        
        clearScene();
        
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
    
    
    return renderResultInfo;
}


double GLViewA::UpdateTick( double currentTick )
{
    ticksum_ -= ticklist_[tickindex_];
    ticksum_ += currentTick;
    ticklist_[tickindex_] = currentTick;
    tickindex_ = (tickindex_ + 1) % NUM_SAMPLES;
    
    return ((double) ticksum_ / NUM_SAMPLES);
}

bool GLViewA::UpdateFPS( float &fFPS )
{
    struct timeval Time;
    gettimeofday( &Time, NULL );
    
    double time = Time.tv_sec + Time.tv_usec * 1.0 / 1000000.0;
    double tick = time - last_tick_;
    double d = UpdateTick( tick );
    last_tick_ = time;
    
    if( Time.tv_sec - tv_last_sec_ >= 1 )
    {
        current_FPS_ = 1.f / d;
        tv_last_sec_ = Time.tv_sec;
        fFPS = current_FPS_;
        return true;
    }
    else
    {
        fFPS = current_FPS_;
        return false;
    }
}


void GLViewA::reloadChilds(SceneNode*node) {
    for (int i = 0; i < node->childs.size(); i++) {
        reloadChilds(node->childs[i]);
    }
    
    Mesh*mesh = node->getMesh();
    Material*mat = node->getMaterial();
    
    if (mat != nullptr)
    mat->reloadTextures();
    
    if (mesh && mesh->isDamaged) {
        loader->reloadChunkInMesh(mesh);
    };
}


void GLViewA::reloadSceneEnd() {
    XLOG("RELOAD END");
    currentSceneController->setSceneStatus(IDLE_MESH);
    isRunningThread = false;
}

void GLViewA::reloadResourceInScene() {
    XLOG("RELOAD IN PROGRESS");
    SceneController*controller = currentSceneController;
    
    
    for (uint i = 0; i < controller->childs.size(); i++) {
        SceneNode*node = controller->childs[i];
        
        reloadChilds(node);
        
        
    }
    
    controller->matValid = true;
}

void GLViewA::clearScene() {
    if (currentSceneController) {
        XLOG("CLEAR SCENE");

        currentSceneController->removeScene();
        delete currentSceneController;
        currentSceneController = NULL;
        sceneLibs.clear();
        mainSceneGraph->clear();
        mainSceneGraph->createListObjects();
    }
}

void GLViewA::stageEndCreateNewScene() {
    isRunningThread = false;
    XLOG("CREATE END");
}

// ASYNC METHOD
RenderResultInfo GLViewA::stageTwoCreateNewScene(std::string langID) {
    
    RenderResultInfo resultMessage;
    resultMessage.intval1 = 0;
    resultMessage.messageID = MESSAGE_NO_MESSAGE;
    
    XLOG("CREATE IN PROGRESS lang:%s",langID.c_str());
    
    if (tempSceneController == nullptr) {
        return resultMessage;
    }
    
    
    
    if (!tempSceneController->isValidController) {
        sceneLibs.clear();
        delete tempSceneController;
        return resultMessage;
    }
    
    tempSceneController->langString = langID;
    
    tempSceneController->loadScene();
    
    resultMessage.intval1 = tempSceneController->hintID;
    resultMessage.messageID = MESSAGE_SHOW_HINT;
    
    
    
    offetEditorX = tempSceneController->moveSceneX;
    offetEditorY = tempSceneController->moveSceneY;
    offetEditorZ = tempSceneController->moveSceneZ;
    
    offetEditorScaleX = tempSceneController->scaleSceneX;
    offetEditorScaleY = tempSceneController->scaleSceneY;
    offetEditorScaleZ = tempSceneController->scaleSceneZ;
    
    offsetEditorRotationX = tempSceneController->rotationSceneX;
    offsetEditorRotationY = tempSceneController->rotationSceneY;
    offsetEditorRotationZ = tempSceneController->rotationSceneZ;
    
    XLOG("Position %f %f %f scale %f %f %f ratation %f %f %f",offetEditorX,offetEditorY,offetEditorZ,offetEditorScaleX,offetEditorScaleY,offetEditorScaleZ,offsetEditorRotationX,offsetEditorRotationY,offsetEditorRotationZ);
    
    
    XVector3 degress(offsetEditorRotationX,offsetEditorRotationY,offsetEditorRotationZ);
    degress*=DEG_TO_RAD;
    
    XQuaternionFromEuler(&offsetEditorQuaternion,&degress);
    
    currentSceneController = tempSceneController;
    tempSceneController = nullptr;
    
    
    sceneLibs[markerName] = currentSceneController;
    
    mainSceneGraph->clear();
    mainSceneGraph->addObjectinTree(currentSceneController);
    mainSceneGraph->createListObjects();
    
    
    return resultMessage;
}

// NOT ASYNC METHOD
RenderResultInfo GLViewA::createNewScene(std::string markID,float widthMarker,float heightMarker) {
    
    RenderResultInfo createSceneInfo;
    
    
    createSceneInfo.messageID = MESSAGE_CREATE_NEW_SCENE;
    
    
    std::string markerID =markerPrefix;
    markerID += markID;
    std::string localMarkerPath   = localPath+"markers"+markerID+".xml";
    
    std::string serverMarkerPathX = serverPath+"markers"+markerID+".xml";
    
    createSceneInfo.paths.push_back(serverMarkerPathX);
    
    // create new scene here !
    
    tempSceneController = new SceneController(localMarkerPath,loader,localPath,&materialCache,textureCache,&meshsCacheMap,soundSystem,animationsCache,&skinAnimationsCache);
    
    tempSceneController->widthMarker = widthMarker;
    tempSceneController->heightMarker = heightMarker;
    
    tempSceneController->markerID = markerID;
    tempSceneController->childsList = &mainSceneGraph->listObjects;
    // cache all resource !
    
    
    std::vector<std::string> resources = tempSceneController->getResources();
    
    for (uint i = 0 ; i < resources.size(); i++) {
        std::string pathForRes = serverPath+resources[i];
        createSceneInfo.paths.push_back(pathForRes);
    }
    
    
    if (!tempSceneController->isValidController) {
        // NOT ASYNC METHOD
        sceneLibs.clear();
        delete tempSceneController;
        
        createSceneInfo.messageID = MESSAGE_CREATE_NEW_SCENE_ERROR;
        return createSceneInfo;
    }
    
    
    return createSceneInfo;
}





void GLViewA::setIsExtendedTrackingActivated(bool val) {
    mIsExtendedTrackingActivated = val;
}

void GLViewA::setIsActivityInPortraitMode(bool val) {
    mIsActivityInPortraitMode = val;
}
void GLViewA::setScreenSize(int width,int height) {
    screenWidth = width;
    screenHeight = height;
}

void GLViewA::startCamera(int cameraNumber) {
    
    currentCamera = static_cast<QCAR::CameraDevice::CAMERA> (cameraNumber);
    
    // Initialize the camera:
    if (!QCAR::CameraDevice::getInstance().init(currentCamera))
        return;
    
    // Select the default camera mode:
    if (!QCAR::CameraDevice::getInstance().selectVideoMode(
                                                           QCAR::CameraDevice::MODE_DEFAULT))
        return;
    
    // Configure the rendering of the video background
    configureVideoBackgroundWithViewWidth(screenWidth, screenHeight);
    
    // Start the camera:
    if (!QCAR::CameraDevice::getInstance().start())
        return;
    
    // Uncomment to enable flash
    //if(QCAR::CameraDevice::getInstance().setFlashTorchMode(true))
    //    LOG("IMAGE TARGETS : enabled torch");
    
    // Uncomment to enable infinity focus mode, or any other supported focus mode
    // See CameraDevice.h for supported focus modes
    //if(QCAR::CameraDevice::getInstance().setFocusMode(QCAR::CameraDevice::FOCUS_MODE_INFINITY))
    //    LOG("IMAGE TARGETS : enabled infinity focus");
    
    // Start the tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::Tracker* objectTracker = trackerManager.getTracker(QCAR::ObjectTracker::getClassType());
    if(objectTracker != 0)
        objectTracker->start();
    
    
}

void GLViewA::stopCamera() {
    // Stop the tracker:
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::Tracker* objectTracker = trackerManager.getTracker(QCAR::ObjectTracker::getClassType());
    if(objectTracker != 0)
        objectTracker->stop();
    
    QCAR::CameraDevice::getInstance().stop();
    QCAR::CameraDevice::getInstance().deinit();
}


void GLViewA::setCameraProjectionMatrix() {

    
    const QCAR::CameraCalibration& cameraCalibration = QCAR::CameraDevice::getInstance().getCameraCalibration();
    projectionMatrixVuforia = QCAR::Tool::getProjectionGL(cameraCalibration, 1.0f, 20000.0f);
    
    XMatrix4MakeWithArray(&vuforiaperspectivematrix, &projectionMatrixVuforia.data[0]);
    
    cameraRenderer->setPerspectivematrix(&projectionMatrixVuforia.data[0]);
    
    
    
    
    cameraRenderer->resetModelViewMatrix();

    
}

bool GLViewA::startExtendedTracking() {
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::ObjectTracker* objectTracker = static_cast<QCAR::ObjectTracker*>(
                                                                           trackerManager.getTracker(QCAR::ObjectTracker::getClassType()));
    
    QCAR::DataSet* currentDataSet = objectTracker->getActiveDataSet();
    if (objectTracker == 0 || currentDataSet == 0)
        return 0;
    
    for (int tIdx = 0; tIdx < currentDataSet->getNumTrackables(); tIdx++)
    {
        QCAR::Trackable* trackable = currentDataSet->getTrackable(tIdx);
        if(!trackable->startExtendedTracking())
            return 0;
    }
    
    mIsExtendedTrackingActivated = true;
    return 1;
}

bool GLViewA::stopExtendedTracking() {
    QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
    QCAR::ObjectTracker* objectTracker = static_cast<QCAR::ObjectTracker*>(
                                                                           trackerManager.getTracker(QCAR::ObjectTracker::getClassType()));
    
    QCAR::DataSet* currentDataSet = objectTracker->getActiveDataSet();
    if (objectTracker == 0 || currentDataSet == 0)
        return 0;
    
    for (int tIdx = 0; tIdx < currentDataSet->getNumTrackables(); tIdx++)
    {
        QCAR::Trackable* trackable = currentDataSet->getTrackable(tIdx);
        if(!trackable->stopExtendedTracking())
            return 0;
    }
    
    mIsExtendedTrackingActivated = false;
    return 1;
}



bool GLViewA::autoFocus() {
    return QCAR::CameraDevice::getInstance().setFocusMode(QCAR::CameraDevice::FOCUS_MODE_TRIGGERAUTO) ? 1 : 0;
}


bool GLViewA::setFocusMode(int focusID) {
    int qcarFocusMode;
    
    switch ((int)focusID)
    {
        case 0:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_NORMAL;
            break;
            
        case 1:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_CONTINUOUSAUTO;
            break;
            
        case 2:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_INFINITY;
            break;
            
        case 3:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_MACRO;
            break;
            
        default:
            return 0;
    }
    
    return QCAR::CameraDevice::getInstance().setFocusMode(qcarFocusMode) ? 1 : 0;
}


bool GLViewA::activateFlash(bool flash) {
    return QCAR::CameraDevice::getInstance().setFlashTorchMode((flash==1)) ? 1 : 0;
}

const char*GLViewA::getVersion() {

    std::string test = "hello world test crashlitics v1 cool )";

    return test.c_str();
}