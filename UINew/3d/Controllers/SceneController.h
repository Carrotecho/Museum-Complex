//
//  SceneController.h
//  UltraVision
//
//  Created by easy proger on 20.09.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef __UltraVision__SceneController__
#define __UltraVision__SceneController__


#define IDLE_MESH 1
#define LOADING 2
#define SAVING_TEXTURE 2

#include <stdio.h>
#include <iostream>
#include <string>

#define XTECH_ENGINE_FRAMEWORK 0

#include "pugixml.hpp"
#include "SceneNode.h"
#include "FileLoader.h"
#include "SoundSystem.h"
#include "TextureCache.h"
#include "XAnimations.h"
#include "DynamicEntity.h"

#include "InteractiveObject.hpp"

#define ACTION_SHOW_MESH      0
#define ACTION_HIDE_MESH      1
#define ACTION_ROT_X_MESH     2
#define ACTION_ROT_Y_MESH     3
#define ACTION_ROT_Z_MESH     4
#define ACTION_STOP_ANIM_MESH 5
#define ACTION_MOVE_X_MESH    6
#define ACTION_MOVE_Y_MESH    7
#define ACTION_MOVE_Z_MESH    8
#define ACTION_PLAY_SOUND     9
#define ACTION_PLAY_ANIMATION 10
#define ACTION_SCALE_X_MESH   11
#define ACTION_SCALE_Y_MESH   12
#define ACTION_SCALE_Z_MESH   13
#define ACTION_TAKE_TEXTURE_FROM_FRAME   14
#define ACTION_SHOW_MARKER_HOLDER        15
#define ACTION_HIDE_MARKER_HOLDER        16

#define ACTION_SCALE_X_SCENE             17
#define ACTION_SCALE_Y_SCENE             18
#define ACTION_SCALE_Z_SCENE             19

#define ACTION_MOVE_X_SCENE              20
#define ACTION_MOVE_Y_SCENE              21
#define ACTION_MOVE_Z_SCENE              22

#define ACTION_ROT_X_SCENE               23
#define ACTION_ROT_Y_SCENE               24
#define ACTION_ROT_Z_SCENE               25

#define ACTION_RELOAD_SCENE              26
#define ACTION_PLAY_ANIMATION_INIT       27
#define ACTION_UPDATE_NODE               28

#define MAX_ACTION 50
#define MAX_TYPES_ACTION 50

#define LANG_RU  1
#define LANG_ENG 2

#define TYPE_ACTION_FRAME 1
#define TYPE_ACTION_TAP 2
#define TYPE_ACTION_VIRTUAL_TAP 3

#define CAMERA_STYLE_VUFORIA 0
#define CAMERA_STYLE_FLIP 1
#define CAMERA_STYLE_FLIP_IF_LOST_MARKER 2

using namespace std;

typedef struct XVirtualButton{
    float x1,x2,y1,y2;
    std::string name;
    
    XVirtualButton(string name, float x1, float x2, float y1, float y2): name(name), x1(x1), x2(x2), y1(y1), y2(y2) {
        
    };
}XVirtualButton;

typedef struct{
    float delta;
    float startValue;
    float endValue;
    float currentValue;
    float currentValue2;
	float currentValue3;
    float currentValue4;
    
    bool loop;
    
    int meshID;
    int id;
    uint typeAction;
    uint type;
    uint typeInit;
    int erased;
    uint currentFrame;
    uint maxFrame;
}Action;

typedef struct{
    bool pressed;
    int numPressed;
}VirtualButtonTapStatus;

typedef struct{
    uint frameID;
    Action actions[MAX_ACTION];
    uint sizeActions;
}Frame;

class SceneController:public SceneNode {
public:
    
    uint hintID,flipHintID;
    
    float scaleSceneX,scaleSceneY,scaleSceneZ,rotationSceneX,rotationSceneY,rotationSceneZ,moveSceneX,moveSceneY,moveSceneZ;
    void setPositionFromCameraStyle();
    
    XVector3 moveSceneFlip,scaleSceneFlip,rotationSceneFlip;
    
    XVector4 rotationQuaternion,rotationQuaternionFlip;
    
    void lostMarker(bool val);
    std::string langString;
    
    bool isValidController;
    //
    int updateNode(Action* a);
    int reloadScene(Action* a);
    
    int scaleXScene(Action* a);
    int scaleYScene(Action* a);
    int scaleZScene(Action* a);
    
    int moveXScene(Action* a);
    int moveYScene(Action* a);
    int moveZScene(Action* a);
    
    int rotationYScene(Action* a);
    int rotationZScene(Action* a);
    int rotationXScene(Action* a);
    
    int playAnimationInit(Action* a);
    int playAnimation(Action* a);
    int playSound(Action* a);
    int stopAnimations(Action* a);
    
    int scale(Action* a);
    int scaleX(Action* a);
    int scaleY(Action* a);
    int scaleZ(Action* a);
    
    int moveX(Action* a);
    int moveY(Action* a);
    int moveZ(Action* a);
    int rotationY(Action* a);
    int rotationZ(Action* a);
    int rotationX(Action* a);
    int hideMesh(Action* a);
    int showMesh(Action* a);
    
    int hideMarkerHolder(Action* a);
    int showMarkerHolder(Action* a);
    
    int takeTextureFromFrame(Action* a);
    
    void loadFrameEvents() ;
    bool needReloadScene;
    bool waitTextureFromFrame;
	bool isInEditor;

    bool revertSwipesEvent;
    
    bool isHaveClassIndexesObject;
    int getCurrentIndexInClassObject();
    int indexSwiped;
    
    void removeScene();
    
    SceneController(std::string markerPath,FileLoader*loader,std::string localFilePath,std::map<std::string,Material*>*materialCache,TextureCache*textureCache,std::map<std::string,Mesh*>* meshsCacheMap,SoundSystem* soundSystem,XAnimations*animCache,std::map<std::string,SkinningFrame*>*skinAnimatorCache);
    ~SceneController();
    
    float widthMarker,heightMarker;
    
    std::vector<std::string> getResources();
    
    bool showMarkerHolderB;
    void setSceneStatus(uint status) ;
    uint getSceneStatus();
    bool isValid();
    void reloadViewMeshes();
    void loadScene();
    void clearScene();
    
    void touchesVirtualButtonBegan(std::string name,bool pressed,int id);
    void touchesBegan(float x,float y);
    void touchesMoved(float x,float y);
    void swipeController(uint direction);
    void zoom(float val);
    void update();
    void endUpdate();
    void setFrame(uint frameIndx);

    void reloadMarker(std::string markerPath);
    std::string markerID;
    void showScene(bool show);
    bool matValid;
    uint getCurrentFrame();
    void getVirtualsButtons(std::vector<XVirtualButton> *vector);
    bool createVirtualButton;
    float getVirtualButtonPosition(float val1,float val2,float val3);
    bool cleared;
    void statusShow(int status);
    
    uint cameraStyle;
    
    Mesh* getMeshFromCache(std::string*nameMesh);
private:
    bool isFlipCameraOnLostMarker;
    DynamicEntity* createNode2(DynamicEntity*object,pugi::xml_node *section);
    
    InteractiveObject* currentIndexesObject;
    
    
    void fillRes(pugi::xml_node *meshSection2,std::map<std::string,int> *paths);
    
    int lastStatusShow;
    void generateMarkerPlane(Mesh*mesh);
    Texture* loadTexture(std::string texturePath);
    void parseMaterialFlag(const std::string &s,std::vector<uint>*vectorOut);
    void getChangedValue(Action* a,float *inputVal,int*retVal);
    void copyAction(Action*out,Action*in);
    uint currentTapIndex;
    void clearActionsByType(uint type);
    
    
    
    int (SceneController::*action[MAX_TYPES_ACTION])(Action* a);
    std::map<std::string,uint> typeIDS;
    std::map<std::string,uint> typeInitIDS;
    std::map<std::string,uint> matIDS;
    std::map<std::string,std::string> resourcePrefixes;
    
    int getTypeIDByName(std::string name);
    void updateActions();
    void createActions(Frame*frame);
    
    void createFrame(pugi::xml_node *section,Frame*frame,uint typeAction);
    void createNode(pugi::xml_node *section);
    
    uint statusScene;
    pugi::xml_document markerXML;
    FileLoader*_loader;
    std::string _localFilePath;
    std::map<std::string,Material*>*_materialCache;
    TextureCache*_textureCache;
    std::map<std::string,Mesh*>* _meshsCacheMap;
    SoundSystem*_soundSystem;
    XAnimations*_animCache;
    std::map<std::string,SkinningFrame*>*_skinAnimatorCache;
    
    std::map<std::string,int> resourcesLibIDS;
    int currentFrame;
    
    std::map<std::string,std::vector<Frame*>*> virtualButtonsTap;
    std::vector<Frame*> framesTap;
    std::vector<Frame*> frames;
    std::vector<Action*> actions;
    
    std::string getResourcePrefixByType(std::string name);
    bool hidden;
    void validateMaterial(Material*mat);
    std::vector<uint> childsIDsForHide;
    
    std::map<int,VirtualButtonTapStatus> virtualsButtonsTouchStatus;
    
    
    std::vector<std::string> split(const std::string &s, char delim);
    std::vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems);
    int getTypeInitIDByName(std::string name);
};



#endif /* defined(__UltraVision__SceneController__) */
