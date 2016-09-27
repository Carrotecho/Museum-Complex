//
//  SceneController.cpp
//  UltraVision
//
//  Created by easy proger on 20.09.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#include "SceneController.h"
#include <algorithm>
#include <string>



#if XTECH_ENGINE_FRAMEWORK
#include <XTechEngine/SkinnedMesh.h>
#include <XTechEngine/DynamicEntity.h>
#include <XTechEngine/XMathBase.h>
#else
#include "SkinnedMesh.h"
#include "DynamicEntity.h"
#include "XMathBase.h"
#include "BookNode.hpp"
#include "ListNode.hpp"
#endif

#include "XTECH-C-Interface.h"

#include "ShaderCache.h"
#include "ResourceManager.h"

SceneController::SceneController(std::string markerPath,FileLoader*loader,std::string localFilePath,std::map<std::string,Material*>*materialCache,TextureCache*textureCache,std::map<std::string,Mesh*>* meshsCacheMap,SoundSystem* soundSystem,XAnimations*animCache,std::map<std::string,SkinningFrame*>*skinAnimatorCache) {

    hintID = 0;
    
    langString = "";
    cameraStyle = CAMERA_STYLE_VUFORIA;
    
    _skinAnimatorCache = skinAnimatorCache;
    _animCache         = animCache;
    _loader            = loader;
    _materialCache     = materialCache;
    _textureCache      = textureCache;
    _localFilePath     = localFilePath;
    _meshsCacheMap     = meshsCacheMap;
    _soundSystem       = soundSystem;
    
    isFlipCameraOnLostMarker = false;
    currentIndexesObject = nullptr;
    isHaveClassIndexesObject = false;
	isInEditor = false;
    
    revertSwipesEvent = false;
    
    isValidController = false;
    
    showMarkerHolderB = false;
    waitTextureFromFrame = false;
    needReloadScene = false;
    
    reloadMarker(markerPath);
    
    matValid = true;
    currentFrame = -1;
    hidden = false;
    statusScene = IDLE_MESH;
    
    lastStatusShow = -1;
    createVirtualButton = false;
    currentTapIndex = 0;
    
    scaleSceneX = 1.0;
    scaleSceneY = 1.0;
    scaleSceneZ = 1.0;
    
    rotationSceneX = 0;
    rotationSceneY = 0;
    rotationSceneZ = 0;
    
    XVector3 degress(rotationSceneX,rotationSceneY,rotationSceneZ);
    degress*=DEG_TO_RAD;
    XQuaternionFromEuler(&rotationQuaternion,&degress);
    
    
    moveSceneX = 0;
    moveSceneY = 0;
    moveSceneZ = 0;
    
    
    matIDS["ligthFong"]       = DYNAMIC_PER_PIXEL_BASE_LIGHT;
    matIDS["resave_shadow"]   = RESAVE_SHADOW_SM;
    matIDS["specular"]        = SPECULAR_MATERIAL;
    matIDS["normalMap"]       = NORMAL_MAP_MATERIAL;
    matIDS["ligthVertexFong"] = DYNAMIC_PER_VERTEX_BASE_LIGHT;
     
    
    typeIDS["reloadScene"]          = ACTION_RELOAD_SCENE;
    typeIDS["showMesh"]             = ACTION_SHOW_MESH;
    typeIDS["hideMesh"]             = ACTION_HIDE_MESH;
    
    typeIDS["rotationMeshX"]        = ACTION_ROT_X_MESH;
    typeIDS["rotationMeshY"]        = ACTION_ROT_Y_MESH;
    typeIDS["rotationMeshZ"]        = ACTION_ROT_Z_MESH;
    
    typeIDS["moveMeshX"]            = ACTION_MOVE_X_MESH;
    typeIDS["moveMeshY"]            = ACTION_MOVE_Y_MESH;
    typeIDS["moveMeshZ"]            = ACTION_MOVE_Z_MESH;
    
    typeIDS["stopAnimation"]        = ACTION_STOP_ANIM_MESH;
    typeIDS["playSound"]            = ACTION_PLAY_SOUND;
    
    typeIDS["playAnim"]             = ACTION_PLAY_ANIMATION;
    
    typeIDS["scaleMeshX"]           = ACTION_SCALE_X_MESH;
    typeIDS["scaleMeshY"]           = ACTION_SCALE_Y_MESH;
    typeIDS["scaleMeshZ"]           = ACTION_SCALE_Z_MESH;
    
    typeIDS["takeTextureFromFrame"] = ACTION_TAKE_TEXTURE_FROM_FRAME;
    
    typeIDS["showMarkerHolder"]     = ACTION_SHOW_MARKER_HOLDER;
    typeIDS["hideMarkerHolder"]     = ACTION_HIDE_MARKER_HOLDER;
    
    typeIDS["scaleSceneX"]          = ACTION_SCALE_X_SCENE;
    typeIDS["scaleSceneY"]          = ACTION_SCALE_Y_SCENE;
    typeIDS["scaleSceneZ"]          = ACTION_SCALE_Z_SCENE;
    
    typeIDS["moveSceneX"]           = ACTION_MOVE_X_SCENE;
    typeIDS["moveSceneY"]           = ACTION_MOVE_Y_SCENE;
    typeIDS["moveSceneZ"]           = ACTION_MOVE_Z_SCENE;
    
    typeIDS["rotationSceneX"]       = ACTION_ROT_X_SCENE;
    typeIDS["rotationSceneY"]       = ACTION_ROT_Y_SCENE;
    typeIDS["rotationSceneZ"]       = ACTION_ROT_Z_SCENE;
    
    typeIDS["updateNode"]           = ACTION_UPDATE_NODE;
    
    
    typeInitIDS["playAnim"]         = ACTION_PLAY_ANIMATION_INIT;
    
    action[ACTION_SHOW_MESH]               = &SceneController::showMesh;
    action[ACTION_HIDE_MESH]               = &SceneController::hideMesh;
    
    action[ACTION_ROT_X_MESH]              = &SceneController::rotationX;
    action[ACTION_ROT_Y_MESH]              = &SceneController::rotationY;
    action[ACTION_ROT_Z_MESH]              = &SceneController::rotationZ;
    
    action[ACTION_MOVE_X_MESH]             = &SceneController::moveX;
    action[ACTION_MOVE_Y_MESH]             = &SceneController::moveY;
    action[ACTION_MOVE_Z_MESH]             = &SceneController::moveZ;
    
    action[ACTION_STOP_ANIM_MESH]          = &SceneController::stopAnimations;
    action[ACTION_PLAY_SOUND]              = &SceneController::playSound;
    action[ACTION_PLAY_ANIMATION]          = &SceneController::playAnimation;
    action[ACTION_PLAY_ANIMATION_INIT]     = &SceneController::playAnimationInit;
    
    action[ACTION_SCALE_X_MESH]            = &SceneController::scaleX;
    action[ACTION_SCALE_Y_MESH]            = &SceneController::scaleY;
    action[ACTION_SCALE_Z_MESH]            = &SceneController::scaleZ;
    
    action[ACTION_TAKE_TEXTURE_FROM_FRAME] = &SceneController::takeTextureFromFrame;
    action[ACTION_SHOW_MARKER_HOLDER]      = &SceneController::showMarkerHolder;
    action[ACTION_HIDE_MARKER_HOLDER]      = &SceneController::hideMarkerHolder;
    
    action[ACTION_SCALE_X_SCENE]           = &SceneController::scaleXScene;
    action[ACTION_SCALE_Y_SCENE]           = &SceneController::scaleYScene;
    action[ACTION_SCALE_Z_SCENE]           = &SceneController::scaleZScene;
    
    action[ACTION_ROT_X_SCENE]             = &SceneController::rotationXScene;
    action[ACTION_ROT_Y_SCENE]             = &SceneController::rotationYScene;
    action[ACTION_ROT_Z_SCENE]             = &SceneController::rotationZScene;
    
    action[ACTION_MOVE_X_SCENE]            = &SceneController::moveXScene;
    action[ACTION_MOVE_Y_SCENE]            = &SceneController::moveYScene;
    action[ACTION_MOVE_Z_SCENE]            = &SceneController::moveZScene;
    
    action[ACTION_MOVE_Z_SCENE]            = &SceneController::moveZScene;
    
    
    action[ACTION_UPDATE_NODE]            = &SceneController::updateNode;
    
    for(std::map<std::string,uint>::iterator iter = typeIDS.begin(); iter != typeIDS.end(); ++iter)
    {
        std::string k =  iter->first;
        if (k.find("Mesh") != -1) {
            resourcePrefixes[k] = "mesh_";
        }else if (k.find("Sound") != -1) {
            resourcePrefixes[k] = "sound_";
        }else if (k.find("Anim") != -1) {
            resourcePrefixes[k] = "mesh_";
        }
    }
    resourcesLibIDS["mesh_-1"] = -1;
    resourcesLibIDS[""] = -1;
}



SceneController::~SceneController(){
    printf("remove SceneController\n");
    clearScene();
    

}
void SceneController::validateMaterial(Material*mat) {
    matValid = matValid && mat->validate();
}
void SceneController::showScene(bool show) {
    if (hidden == show) {
        return;
    }
    
    if (show) {
        matValid  = true;
        for (unsigned int i = 0; i < childsIDsForHide.size(); i++) {
            SceneNode*obj = this->childs[i];
            obj->setVisible(true);
            Material*mat = obj->getMaterial();
            validateMaterial(mat);
        }
        childsIDsForHide.clear();
    }else {
        childsIDsForHide.clear();
        for (unsigned int i = 0; i < this->childs.size(); i++) {
            if (this->childs[i]->isVisible) {
                childsIDsForHide.push_back(i);
                this->childs[i]->setVisible(false);
            }
        }
    }
    hidden = show;
}

void SceneController::lostMarker(bool val) {
    uint prevCameraStyle = cameraStyle;
    if (isFlipCameraOnLostMarker && val) {
        prevCameraStyle = CAMERA_STYLE_FLIP;
    }else if (isFlipCameraOnLostMarker && !val) {
        prevCameraStyle = CAMERA_STYLE_VUFORIA;
    }
    
    if (prevCameraStyle != cameraStyle) {
        cameraStyle = prevCameraStyle;
        setPositionFromCameraStyle();
    }
}

void SceneController::reloadMarker(std::string markerPath) {
    FILE *fp;
    int count, pos, shaderSize;
    char*shaderXMLsrc;
    
    
    
    fp = fopen( markerPath.c_str(), "r");
    if ( !fp ) {
        XLOG("SceneController filedload marker file:%s",markerPath.c_str());
        return;
    }
    
    
    
    pos = (int) ftell ( fp );
    fseek ( fp, 0, SEEK_END );			//move to end
    shaderSize = ( int ) ftell ( fp ) - pos;	//calculates file size
    fseek ( fp, 0, SEEK_SET ); 			//rewind to beginning
    
    shaderXMLsrc = (char *) malloc(shaderSize + 1);
    
    // Read the source code
    count = (int) fread(shaderXMLsrc, 1, shaderSize, fp);
    shaderXMLsrc[count] = '\0';
    
    if (ferror(fp))
        count = 0;
    fclose(fp);
    
    
    
    markerXML.load(shaderXMLsrc);
    free(shaderXMLsrc);
    
    
    isValidController = true;
}

void SceneController::swipeController(uint direction) {
    for (uint i = 0; i < childs.size(); i++) {
        childs[i]->swipeEvent(direction);
    }
}

void SceneController::zoom(float val){
    for (uint i = 0; i < childs.size(); i++) {
        childs[i]->zoom(val);
    }
}

void SceneController::touchesMoved(float x,float y) {
    for (uint i = 0; i < childs.size(); i++) {
        childs[i]->touchesMoved(x,y);
    }
}

int SceneController::getCurrentIndexInClassObject() {
    if (isHaveClassIndexesObject && currentIndexesObject != nullptr) {
        return currentIndexesObject->getInteractiveIndex();
    }
    return -1;
}

void SceneController::touchesBegan(float x,float y) {
    
    for (uint i = 0; i < childs.size(); i++) {
        childs[i]->touchesBegan(x,y);
    }
    
    if (!framesTap.size()) {
        return;
    }
    if (currentTapIndex >= framesTap.size()) {
        currentTapIndex = 0;
    }
    
    clearActionsByType(TYPE_ACTION_TAP);
    createActions(framesTap[currentTapIndex]);
    
    currentTapIndex++;
}

void SceneController::touchesVirtualButtonBegan(std::string name,bool pressed,int id) {
    if (virtualsButtonsTouchStatus.find(id) != virtualsButtonsTouchStatus.end()) {
        if (virtualsButtonsTouchStatus[id].pressed != pressed) {
            if (pressed) {
                std::vector<Frame*>*vectorFrames = virtualButtonsTap[name.c_str()];
                if (!vectorFrames || !vectorFrames->size()) return;
                if ((unsigned int)virtualsButtonsTouchStatus[id].numPressed >= vectorFrames->size()) {
                    virtualsButtonsTouchStatus[id].numPressed = 0;
                }
                clearActionsByType(TYPE_ACTION_VIRTUAL_TAP);
                int indexAction = virtualsButtonsTouchStatus[id].numPressed;
                createActions(vectorFrames->operator[](indexAction));
                virtualsButtonsTouchStatus[id].numPressed++;
            }
        }
    }else {
        virtualsButtonsTouchStatus[id].numPressed = 0;
    }
    virtualsButtonsTouchStatus[id].pressed = pressed;
}


int SceneController::reloadScene(Action* a) {
    // clear current actions ! important !
    
    
    needReloadScene = true;
    
    return 2;
}


int SceneController::hideMarkerHolder(Action* a) {
    showMarkerHolderB = false;
    return 1;
}

int SceneController::showMarkerHolder(Action* a) {
    showMarkerHolderB = true;
    return 1;
}

int SceneController::takeTextureFromFrame(Action* a) {
    waitTextureFromFrame = true;
    return 1;
}

void SceneController::removeScene() {
    
    
    clearScene();
}



int SceneController::stopAnimations(Action* a) {
    clearActionsByType(TYPE_ACTION_FRAME);
    return 2;
}

int SceneController::hideMesh(Action* a) {
    if (a->id == -1) {
        for (unsigned int i = 0; i < childs.size(); i++) {
            childs[i]->setVisible(false);
        }
    }else if (childs.size() > (unsigned int)a->id) {
        childs[a->id]->setVisible(false);
    }
    return 1;
}


int SceneController::updateNode(Action* a) {
    DynamicEntity*animationNode = (DynamicEntity*)childs[a->meshID];
    animationNode->update(0);
    return 0;
}

int SceneController::playAnimationInit(Action* a) {
    DynamicEntity*animationNode = (DynamicEntity*)childs[a->meshID];
    XAnimation*anim = _animCache->getAnimation(animationNode->getMesh()->assignedBaseAnimationsID);
    if (anim !=nullptr) {
        animationNode->setAnimation(anim);
    }
    return 1;
}
int SceneController::playAnimation(Action* a) {
    DynamicEntity*animationNode = (DynamicEntity*)childs[a->meshID];
    if (!animationNode || !animationNode->haveAnimation) {
        return 1;
    }
    
    
    int ret1 = 1;
    float val = (float)animationNode->lastFrameID;
    getChangedValue(a,&val,&ret1);
    
    animationNode->update((int)val);

    return ret1;
}

int SceneController::playSound(Action* a) {
    _soundSystem->playSound(a->id,a->loop);
    return 1;
}
int SceneController::showMesh(Action* a) {
    if (a->id == -1) {
        for (uint i = 0; i < childs.size(); i++) {
            childs[i]->setVisible(true);
            validateMaterial(childs[i]->getMaterial());
        }
    }else if (childs.size() > (uint)a->id) {
        childs[a->id]->setVisible(true);
        validateMaterial(childs[a->id]->getMaterial());
    }
    return 1;
}




void SceneController::getChangedValue(Action* a,float *inputVal,int*retVal) {
    if (a->startValue != 0 || a->endValue != 0) {
        
        if (a->startValue > a->endValue) {
            
            if (a->endValue <= a->startValue-a->currentValue) {
                *inputVal = a->startValue-a->currentValue;
                if (a->delta > 0.001) {
                    a->currentValue+=a->delta;
                }else {
                    a->currentValue++;
                }
            }else {
                
                if (a->loop) {
                    a->currentValue = 0;
                    *retVal = 0;
                }else {
                    *retVal = 1;
                }
                
                return;
            }
        }else {
            if (a->endValue >= a->startValue+a->currentValue) {
                *inputVal = a->startValue+a->currentValue;
                if (a->delta > 0.001) {
                    a->currentValue+=a->delta;
                }else {
                    a->currentValue++;
                }
                
            }else {
                
                if (a->loop) {
                    a->currentValue = 0;
                    *retVal = 0;
                }else {
                    *retVal = 1;
                }
                return;
            }
        }
        
    }else {
		if (a->delta > 0.001) {
			(*inputVal)+=a->delta;
		}else {
			(*inputVal)++;
		}
        
    }
    *retVal = 0;
}

int SceneController::scaleXScene(Action* a) {
    int ret = 0;
    float val = scaleSceneX;
    getChangedValue(a,&val,&ret);
    scaleSceneX = val;
    return ret;
}

int SceneController::scaleYScene(Action* a) {
    int ret = 0;
    float val = scaleSceneY;
    getChangedValue(a,&val,&ret);
    scaleSceneY = val;
    return ret;
}

int SceneController::scaleZScene(Action* a) {
    int ret = 0;
    float val = scaleSceneZ;
    getChangedValue(a,&val,&ret);
    scaleSceneZ = val;
    return ret;
}


int SceneController::moveXScene(Action* a) {
    int ret = 0;
    float val = moveSceneX;
    getChangedValue(a,&val,&ret);
    moveSceneX = val;
    return ret;
}

int SceneController::moveYScene(Action* a) {
    int ret = 0;
    float val = moveSceneY;
    getChangedValue(a,&val,&ret);
    moveSceneY = val;
    return ret;
}

int SceneController::moveZScene(Action* a) {
    int ret = 0;
    float val = moveSceneZ;
    getChangedValue(a,&val,&ret);
    moveSceneZ = val;
    return ret;
}


int SceneController::rotationYScene(Action* a) {
    int ret = 0;
    float val = rotationSceneX;
    getChangedValue(a,&val,&ret);
    rotationSceneX = val;
    return ret;
}

int SceneController::rotationZScene(Action* a) {
    int ret = 0;
    float val = rotationSceneY;
    getChangedValue(a,&val,&ret);
    rotationSceneY = val;
    return ret;
}

int SceneController::rotationXScene(Action* a) {
    int ret = 0;
    float val = rotationSceneZ;
    getChangedValue(a,&val,&ret);
    rotationSceneZ = val;
    return ret;
}


int SceneController::scaleX(Action* a) {
    if ((uint)a->id >= childs.size()) {
        return 1;
    }
    SceneNode*node = childs[a->id];
    
    int ret = 0;
	float val = node->getScale()->x;
    getChangedValue(a,&val,&ret);
    node->setScaleX(val);
    
    return ret;
}

int SceneController::scaleY(Action* a) {
    if ((uint)a->id >= childs.size()) {
        return 1;
    }
    SceneNode*node = childs[a->id];
    
    int ret = 0;
    float val = node->getScale()->y;
    getChangedValue(a,&val,&ret);
    node->setScaleY(val);
    
    return ret;
}

int SceneController::scaleZ(Action* a) {
    if ((uint)a->id >= childs.size()) {
        return 1;
    }
    SceneNode*node = childs[a->id];
    
    int ret = 0;
    float val = node->getScale()->z;
    getChangedValue(a,&val,&ret);
    node->setScaleZ(val);
    
    return ret;
}




int SceneController::moveX(Action* a) {
    if ((uint)a->id >= childs.size()) {
        return 1;
    }
    SceneNode*node = childs[a->id];
    
    int ret = 0;
	float val = node->getPosition()->x;
    getChangedValue(a,&val,&ret);
    node->setX(val);
    
    return ret;
}

int SceneController::moveY(Action* a) {
    if ((uint)a->id >= childs.size()) {
        return 1;
    }
    SceneNode*node = childs[a->id];
    
    int ret = 0;
    float val = node->getPosition()->y;
    getChangedValue(a,&val,&ret);
    node->setY(val);
    
    return ret;
}

int SceneController::moveZ(Action* a) {
    if ((uint)a->id >= childs.size()) {
        return 1;
    }
    SceneNode*node = childs[a->id];
    
    int ret = 0;
    float val = node->getPosition()->z;
    getChangedValue(a,&val,&ret);
    node->setZ(val);
    
    return ret;
}

int SceneController::rotationX(Action* a) {
    if ((uint)a->id >= childs.size()) {
        return 1;
    }
    SceneNode*node = childs[a->id];
    
    int ret = 0;
    
    XVector3 rotation = node->getRotationDegress();
    
	float val = rotation.x;
    getChangedValue(a,&val,&ret);
    rotation.x = val;
    node->setRotationEulerinDegress(&rotation);
    
    
    return ret;
}

int SceneController::rotationY(Action* a) {
    if ((uint)a->id >= childs.size()) {
        return 1;
    }
    
    SceneNode*node = childs[a->id];
    
    XVector3 rotation = node->getRotationDegress();
    
    int ret = 0;
    float val = rotation.y;
    getChangedValue(a,&val,&ret);
    rotation.y = val;
    node->setRotationEulerinDegress(&rotation);
    
    return ret;
}

int SceneController::rotationZ(Action* a) {
    if ((uint)a->id >= childs.size()) {
        return 1;
    }
    
    
    SceneNode*node = childs[a->id];
    XVector3 rotation = node->getRotationDegress();

    int ret = 0;
    float val = rotation.z;
    getChangedValue(a,&val,&ret);
    rotation.z = val;
    node->setRotationEulerinDegress(&rotation);
    
    return ret;
}

int SceneController::getTypeIDByName(std::string name) {
    if (typeIDS.find(name) != typeIDS.end()) {
        return typeIDS[name];
    }
    return -1;
}
int SceneController::getTypeInitIDByName(std::string name) {
    if (typeInitIDS.find(name) != typeInitIDS.end()) {
        return typeInitIDS[name];
    }
    return -1;
}

void SceneController::copyAction(Action*out,Action*in) {
    out->startValue    = in->startValue;
    out->endValue      = in->endValue;
    out->currentValue  = in->currentValue;
    out->delta         = in->delta;
    out->loop          = in->loop;
    out->currentValue2 = in->currentValue2;
	out->currentValue3 = in->currentValue3;
    out->currentValue4 = in->currentValue4;
    out->meshID        = in->meshID;
    out->id            = in->id;
    out->typeAction    = in->typeAction;
    out->type          = in->type;
    out->typeInit      = in->typeInit;
    out->erased        = in->erased;
    out->currentFrame  = in->currentFrame;
    out->maxFrame      = in->maxFrame;
}


void SceneController::createActions(Frame*frame) {
    for (uint i = 0; i < frame->sizeActions; i++) {
        Action*actionCopy = (Action*)malloc(sizeof(Action));
        copyAction(actionCopy,&frame->actions[i]);
        actions.push_back(actionCopy);
        
        // init action
        if (actionCopy->typeInit != -1) (this->*action[actionCopy->typeInit])(actionCopy);
    }
}


std::string SceneController::getResourcePrefixByType(std::string name) {
    if (resourcePrefixes.find(name) != resourcePrefixes.end()) {
        return resourcePrefixes[name];
    }
    return "";
}

void SceneController::createFrame(pugi::xml_node *section,Frame*frame,uint typeAction) {
    
    frame->sizeActions = 0;
    
    for (pugi::xml_node actionSection = section->child("action"); actionSection; actionSection = actionSection.next_sibling("action")) {
        if (frame->sizeActions >= MAX_ACTION) {
            break;
        }
        
        int typeID     = getTypeIDByName(actionSection.attribute("type").as_string());
        int typeInitID = getTypeInitIDByName(actionSection.attribute("type").as_string());
        if (typeID < 0) {
            break;
        }
        std::string resourceID = getResourcePrefixByType(actionSection.attribute("type").as_string());
        resourceID+=actionSection.attribute("id").as_string();




        if (resourcesLibIDS.find(resourceID) == resourcesLibIDS.end()) {

            continue;
        }

		if (frame->sizeActions >= MAX_ACTION) {
            break;
        }
        frame->actions[frame->sizeActions].delta        = actionSection.attribute("delta").as_float();
        frame->actions[frame->sizeActions].typeAction   = typeAction;
        frame->actions[frame->sizeActions].currentValue = 0;
        frame->actions[frame->sizeActions].startValue   = actionSection.attribute("startValue").as_float();
        frame->actions[frame->sizeActions].endValue     = actionSection.attribute("endValue").as_float();
        
        frame->actions[frame->sizeActions].loop         = actionSection.attribute("loop").empty() ? false : actionSection.attribute("loop").as_bool();
        
        
        frame->actions[frame->sizeActions].currentValue2 = -1;
		frame->actions[frame->sizeActions].currentValue3 = -1;
        frame->actions[frame->sizeActions].currentValue4 = -1;
        
        if (!actionSection.attribute("mesh").empty()) {
            std::string meshResourceID = "mesh_";
            meshResourceID+=actionSection.attribute("mesh").as_string();
            frame->actions[frame->sizeActions].meshID   = resourcesLibIDS[meshResourceID];
        }
        
        frame->actions[frame->sizeActions].id           = resourcesLibIDS[resourceID];
        frame->actions[frame->sizeActions].type         = typeID;
        frame->actions[frame->sizeActions].typeInit     = typeInitID;
        frame->sizeActions++;
    }
}




void SceneController::parseMaterialFlag(const std::string &s,std::vector<uint>*vectorOut) {
    //difuse|ligthFong|resave_shadow|specular
    std::vector<std::string> elems;
    const char delim[] = "|";
    split(s, *delim, elems);
    
    for (uint i = 0; i < elems.size(); i++) {
        if (matIDS.find(elems[i]) != matIDS.end()) {
            vectorOut->push_back(matIDS[elems[i]]);
        }
    }
}

Texture* SceneController::loadTexture(std::string texturePath) {
    Texture* texture = 0;
    texture = _textureCache->getTextureByPath(_localFilePath+texturePath);
    return texture;
}



Mesh* SceneController::getMeshFromCache(std::string*nameMesh) {
    
    if (_meshsCacheMap->find(*nameMesh) != _meshsCacheMap->end()) {
        return (*_meshsCacheMap)[*nameMesh];
    }else {
        return nullptr;
    }
}


std::vector<std::string> & SceneController::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> SceneController::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}



void SceneController::createNode(pugi::xml_node *section) {
    DynamicEntity *object = nullptr;
    cameraStyle = CAMERA_STYLE_VUFORIA;
    
    if (!section->attribute("class").empty() ) {
        
        if (std::strcmp(section->attribute("class").as_string(), "Book") == 0) {
            // book
            
            
            flipHintID = 1;
            
            hintID = 0;
            std::string pathForpages = section->attribute("path").as_string();
            pathForpages+=section->attribute("namePages").as_string();
            
            Mesh*pagesMesh = getMeshFromCache(&pathForpages);

            if (pagesMesh == nullptr) return;
            
            if (pagesMesh->isDamaged) {
                _loader->reloadChunkInMesh(pagesMesh);
            }
            
            const std::string pagesPaths = section->attribute("pages").as_string();
            
            
            XAnimation*anim = nullptr;
            if (pagesMesh->assignedBaseAnimationsID != -1 ) {
                anim =  _animCache->getAnimation(pagesMesh->assignedBaseAnimationsID );
            }
            
            
            std::string listSoundID = "sound_";
            listSoundID+=section->attribute("soundListID").as_string();
            
            std::string endSoundID = "sound_";
            endSoundID+=section->attribute("soundCloseID").as_string();
            
            std::string openSoundID = "sound_";
            openSoundID+=section->attribute("soundOpenID").as_string();
            
            
            ListNode * list = new ListNode(_skinAnimatorCache,_soundSystem);
            currentIndexesObject = list;
            
           
            BookNode* book = new BookNode(pagesMesh,_skinAnimatorCache,split(pagesPaths,':'),_localFilePath,anim,
                                          section->attribute("bookOpenS").as_float(),
                                          section->attribute("bookOpenE").as_float(),
                                          section->attribute("pageS").as_float(),
                                          section->attribute("pageE").as_float(),
                                          section->attribute("bookCloseS").as_float(),
                                          section->attribute("bookCloseE").as_float(),
                                          _soundSystem,
                                          resourcesLibIDS[listSoundID],
                                          resourcesLibIDS[endSoundID],
                                          resourcesLibIDS[openSoundID]);
            object = createNode2(book,section);
            
            list->setNoList();
            
            list->addNodeToList(book,section->attribute("startFrame").as_float(),
                                section->attribute("centerFrame").as_float(),
                                section->attribute("endFrame").as_float(),
                                section->attribute("delta").as_float(),
                                
                                section->attribute("rX").as_bool(),
                                section->attribute("rY").as_bool(),
                                section->attribute("rZ").as_bool(),
                                section->attribute("rXn").as_uint(),
                                section->attribute("rYn").as_uint(),
                                section->attribute("rZn").as_uint(),
                                section->attribute("rDelta").as_float(),
                                
                                section->attribute("zoom").as_bool(),
                                section->attribute("zoomDelta").as_float(),
                                !section->attribute("initialScale").empty(),
                                section->attribute("initialScale").as_float(),
                                !section->attribute("zoomMin").empty(),
                                section->attribute("zoomMin").as_float(),
                                !section->attribute("zoomMax").empty(),
                                section->attribute("zoomMax").as_float(),
                                
                                !section->attribute("initialRotateX").empty(),
                                section->attribute("initialRotateX").as_float(),
                                !section->attribute("initialRotateY").empty(),
                                section->attribute("initialRotateY").as_float(),
                                !section->attribute("initialRotateZ").empty(),
                                section->attribute("initialRotateZ").as_float(),
                                !section->attribute("deltaMoveXInZoom").empty(),
                                section->attribute("deltaMoveXInZoom").as_float());
           //
            if (!section->attribute("cameraStyle").empty()) {
                cameraStyle = section->attribute("cameraStyle").as_uint();
                if (cameraStyle == CAMERA_STYLE_FLIP_IF_LOST_MARKER) {
                    cameraStyle = CAMERA_STYLE_VUFORIA;
                    isFlipCameraOnLostMarker = true;
                }
            }
            
            list->translateToChildSwipeEvent(true);
            
            list->nextNode();
            
            std::string systemID = "mesh_";
            systemID+=section->attribute("id").as_string();
            
            resourcesLibIDS[systemID] = (int)this->childs.size();
            this->addChild(list);
            
            return;

        }else if (std::strcmp(section->attribute("class").as_string(), "List") == 0) {
            isHaveClassIndexesObject = true;
            hintID = 3;
            
            
            
            
            ListNode * list = new ListNode(_skinAnimatorCache,_soundSystem);
            currentIndexesObject = list;
            object =  list;
            
            if (!section->attribute("cameraStyle").empty()) {
                cameraStyle = section->attribute("cameraStyle").as_uint();
            }
            
            //cameraStyle
            if (!section->attribute("noList").empty()) {
                list->setNoList();
                
            }
            
            if (!section->attribute("cera").empty()) {
                std::string listSoundID = "sound_";
                listSoundID+=section->attribute("ceraListSoundID").as_string();
                list->setCera(resourcesLibIDS[listSoundID]);
                
            }
            
            uint numObject = 0;
            
            for (pugi::xml_node meshSection2 = section->child("mesh"); meshSection2; meshSection2 = meshSection2.next_sibling("mesh")) {
                numObject++;
                DynamicEntity* listNode = createNode2(nullptr, &meshSection2);
                if (listNode != nullptr) {
                    list->addNodeToList(listNode,
                                        meshSection2.attribute("startFrame").as_float(),
                                        meshSection2.attribute("centerFrame").as_float(),
                                        meshSection2.attribute("endFrame").as_float(),
                                        meshSection2.attribute("delta").as_float(),
                                        
                                        meshSection2.attribute("rX").as_bool(),
                                        meshSection2.attribute("rY").as_bool(),
                                        meshSection2.attribute("rZ").as_bool(),
                                        meshSection2.attribute("rXn").as_uint(),
                                        meshSection2.attribute("rYn").as_uint(),
                                        meshSection2.attribute("rZn").as_uint(),
                                        meshSection2.attribute("rDelta").as_float(),
                                        
                                        meshSection2.attribute("zoom").as_bool(),
                                        meshSection2.attribute("zoomDelta").as_float(),
                                        !meshSection2.attribute("initialScale").empty(),
                                        meshSection2.attribute("initialScale").as_float(),
                                        !meshSection2.attribute("zoomMin").empty(),
                                        meshSection2.attribute("zoomMin").as_float(),
                                        !meshSection2.attribute("zoomMax").empty(),
                                        meshSection2.attribute("zoomMax").as_float(),
                                        
                                        !meshSection2.attribute("initialRotateX").empty(),
                                        meshSection2.attribute("initialRotateX").as_float(),
                                        !meshSection2.attribute("initialRotateY").empty(),
                                        meshSection2.attribute("initialRotateY").as_float(),
                                        !meshSection2.attribute("initialRotateZ").empty(),
                                        meshSection2.attribute("initialRotateZ").as_float(),
                                        !section->attribute("deltaMoveXInZoom").empty(),
                                        section->attribute("deltaMoveXInZoom").as_float());
                }
                
            }
            
            
            if (numObject == 1) {
                hintID = 2;
            }
            
           
            
            list->nextNode();
            
            std::string systemID = "mesh_";
            systemID+=section->attribute("id").as_string();
            
            resourcesLibIDS[systemID] = (int)this->childs.size();
            this->addChild(object);
            
            
            flipHintID = hintID;
            
            return;
        }
        
        
        
        
        
        if (object == nullptr) return;
        
        
    }
    
    
    
    object = createNode2(object,section);
    if (object == nullptr) return;
    
    
    std::string systemID = "mesh_";
    systemID+=section->attribute("id").as_string();

	resourcesLibIDS[systemID] = (int)this->childs.size();
    this->addChild(object);
}


DynamicEntity* SceneController::createNode2(DynamicEntity*object,pugi::xml_node *section) {
    
    
    
    
    Mesh*mesh = nullptr;
    
    std::string pathForCache_mesh = section->attribute("path").as_string();
    pathForCache_mesh+= section->attribute("nameObject").as_string();
    
    if (_meshsCacheMap->find(pathForCache_mesh) != _meshsCacheMap->end()) {
        mesh = (*_meshsCacheMap)[pathForCache_mesh];
        if (object == nullptr) object = new DynamicEntity(_skinAnimatorCache);
    }else {
        return nullptr;
    }
    
    Material*matFlags = new Material();
    
    
    
    if (!section->attribute("materialFlag").empty()) {
        std::vector<uint> materialIDS;
        parseMaterialFlag(section->attribute("materialFlag").as_string(),&materialIDS);
        for (uint i = 0; i < materialIDS.size(); i++) {
            matFlags->setFlag(materialIDS[i], true);
        }
    }
    
    if (mesh->color && mesh->numTextureCoordinats) {
        matFlags->setFlag(TEXTURE_AND_COLOR, true);
    }else {
        if (mesh->color) {
            matFlags->setFlag(COLOR_MATERIAL,true);
        }else if (mesh->numTextureCoordinats){
            matFlags->setFlag(NORMAL_MATERIAL,true);
        }else {
            matFlags->setFlag(BASE_MATERIAL_COLOR, true);
        }
    }
    
    std::string testureAttrID = "texturePath";
    
    if (!section->attribute("langTextures").empty()) {
        testureAttrID+=langString;
    }
    
    Texture* difuseTexture = 0;
    Texture* specularTexture = 0;
    Texture* normalTexture = 0;
    
    if (!section->attribute(testureAttrID.c_str()).empty()) {
        difuseTexture   = loadTexture(section->attribute(testureAttrID.c_str()).as_string());
    }
    
    if (matFlags->getFlagVal(NORMAL_MATERIAL)) {
        if (!difuseTexture) {
            // its bad ! texture need but not create !
            difuseTexture = new Texture();
            difuseTexture->createTexture();
        }
    }
    
    
    if (!section->attribute("specularPath").empty()) {
        specularTexture = loadTexture(section->attribute("specularPath").as_string());
        matFlags->setFlag(SPECULAR_MATERIAL, true);
    }
    if (!section->attribute("normalMapPath").empty()) {
        normalTexture  = loadTexture(section->attribute("normalMapPath").as_string());
        
        matFlags->setFlag(NORMAL_MAP_MATERIAL, true);
        
    }
    
    
    if (!matFlags->getFlagVal(NORMAL_MATERIAL) && !matFlags->getFlagVal(COLOR_MATERIAL) && !matFlags->getFlagVal(TEXTURE_AND_COLOR)) {
        matFlags->setFlag(BASE_MATERIAL_COLOR, true);
    }
    
    
    
    matFlags->setSpecularPower(section->attribute("specularPower").as_float());
    
    
    std::string uid = matFlags->matFlagsString;
    
    uid+=section->attribute(testureAttrID.c_str()).as_string();
    uid+=section->attribute("specularPath").as_string();
    uid+=section->attribute("normalMapPath").as_string();
    
    Material*mat = NULL;
    
    
    
    if ( _materialCache->find(uid) != _materialCache->end() ) {
        mat = _materialCache->operator[](uid);
    }
    
    if (!mat) {
        mat = new Material();
        for (int i = 0; i < MAX_MATERIALS; i++) {
            mat->setFlag(i,matFlags->getFlagVal(i));
        }
        mat->setSpecularPower(matFlags->getSpecularPower());
        mat->diffuseTexture    = difuseTexture;
        mat->normalMapTexture  = normalTexture;
        mat->specularTexture   = specularTexture;
        (*_materialCache)[uid] = mat; // push material
    };
    
    delete matFlags;
    
    
    if (mesh->isDamaged) {
        _loader->reloadChunkInMesh(mesh);
    }
    object->setMesh(mesh);
    
    if (mesh->assignedBaseAnimationsID != -1) {
        XAnimation*anim = _animCache->getAnimation(mesh->assignedBaseAnimationsID);
        if (anim !=nullptr) {
            object->setAnimation(anim);
            object->update(0);
        }
    }
    
    if (!section->attribute("blend").empty() && section->attribute("blend").as_bool()) {
        object->blendFunction = true;
        object->blendFunctionIn = GL_SRC_ALPHA;
        object->blendFunctionOut = GL_ONE_MINUS_SRC_ALPHA;
    }
    
    
    object->setTranslate(&mesh->initTranslation);
    object->setQuaternion(&mesh->initQuaternion);
    object->setScale(&mesh->initScale);
    
    object->setMaterial(mat);
    
    object->setVisible(true);
    
    return object;
}



void SceneController::fillRes(pugi::xml_node *meshSection2, std::map<std::string, int> *paths) {
    if (!meshSection2->attribute("path").empty()) {
        std::string path = meshSection2->attribute("path").as_string();
        (*paths)[path] = 1;
    }
    
    if (!meshSection2->attribute("langTextures").empty()) {
        std::string langs = meshSection2->attribute("langTextures").as_string();
        std::vector<std::string> langsArray;
        const char delim[] = "|";
        split(langs, *delim, langsArray);
        for (int i = 0; i < langsArray.size(); i++) {
            
            std::string testurePathAttrID = "texturePath";
            testurePathAttrID+=langsArray[i];
            if (!meshSection2->attribute(testurePathAttrID.c_str()).empty()) {
                std::string texpath = meshSection2->attribute(testurePathAttrID.c_str()).as_string();
                (*paths)[texpath] = 1;
            }
        }
        
        
        
    }else {
        if (!meshSection2->attribute("texturePath").empty()) {
            std::string texpath = meshSection2->attribute("texturePath").as_string();
            (*paths)[texpath] = 1;
        }
    }
    
    
    
    
    if (!meshSection2->attribute("specularPath").empty()) {
        std::string texpath = meshSection2->attribute("specularPath").as_string();
        (*paths)[texpath] = 1;
    }
    
    if (!meshSection2->attribute("normalMapPath").empty()) {
        std::string texpath = meshSection2->attribute("normalMapPath").as_string();
        (*paths)[texpath] = 1;
    }
    
    if (!meshSection2->attribute("pages").empty()) {
        std::string pagesPaths = meshSection2->attribute("pages").as_string();
        std::vector<std::string> _pagetexturesPaths = split(pagesPaths,':');
        for (int i = 0; i < _pagetexturesPaths.size(); i++) {
            (*paths)[_pagetexturesPaths[i]] = 1;
        }
    };
}

std::vector<std::string> SceneController::getResources() {
    std::vector<std::string> paths_fin;
    if (!this->isValidController) {
        return paths_fin;
    }
    
    
    std::map<std::string,int> paths;
	//meshs
    
    
    for (pugi::xml_node meshSection = markerXML.child("meshs").child("mesh"); meshSection; meshSection = meshSection.next_sibling("mesh")) {
        //fillRes
        for (pugi::xml_node meshSection2 = meshSection.child("mesh"); meshSection2; meshSection2 = meshSection2.next_sibling("mesh")) {
            fillRes(&meshSection2,&paths);
        }
        fillRes(&meshSection,&paths);
    }
    
    
    //sounds
    for (pugi::xml_node soundSection = markerXML.child("sounds").child("sound"); soundSection; soundSection = soundSection.next_sibling("sound")) {
        std::string path = soundSection.attribute("path").as_string();
        paths[path] = 1;
    }
    //animations
    for (pugi::xml_node animationSection = markerXML.child("animations").child("animation"); animationSection; animationSection = animationSection.next_sibling("animation")) {
        std::string pathForAnim = animationSection.attribute("path").as_string();
        paths[pathForAnim] = 1;
    }
    
    for (std::map<std::string,int>::iterator it=paths.begin(); it!=paths.end(); ++it) {
        paths_fin.push_back( it->first);
    }
    
    
    return paths_fin;
}



void SceneController::clearScene() {
    
    XLOG("CLEAR SCENE");
    
    if (_soundSystem) _soundSystem->stopSound();
    
    
 
    
    for (auto const mesh : *_meshsCacheMap) {
        Mesh*m = mesh.second;
        m->clear();
        m->isDamaged = true;
    }
    _meshsCacheMap->clear();
    
    for (std::vector<Frame*>::iterator it=frames.begin();it!=frames.end();){
        Frame*frame = (*it);
        free(frame);
        it = frames.erase(it);
    }
    
 
    
    for (auto const it:*_skinAnimatorCache ) {
        removeSkinningFrame(it.second);
    }
    _skinAnimatorCache->clear();
    
    
    frames.clear();
    
    for (std::vector<Frame*>::iterator it=framesTap.begin();it!=framesTap.end();){
        Frame*frame = (*it);
        free(frame);
        it = framesTap.erase(it);
    }
    
    framesTap.clear();
    
    for (std::vector<Action*>::iterator it=actions.begin();it!=actions.end();){
        Action*action = (*it);
        free(action);
        it = actions.erase(it);
    }
    
    actions.clear();
    
    for (std::map<std::string,std::vector<Frame*>*>::iterator itVirtual=virtualButtonsTap.begin(); itVirtual!=virtualButtonsTap.end(); ++itVirtual) {
        for (std::vector<Frame*>::iterator it=itVirtual->second->begin();it!=itVirtual->second->end();){
            Frame*frame = (*it);
            free(frame);
            it = itVirtual->second->erase(it);
        }
        delete itVirtual->second;
    }
    virtualButtonsTap.clear();
    
    
    cleared = true;
}

void SceneController::setPositionFromCameraStyle() {
    if (cameraStyle == CAMERA_STYLE_VUFORIA) {
        XMatrix4 mat,tmp1,tmp2;
        XVector3 pos(moveSceneX, moveSceneY, moveSceneZ);
        XMatrix4MakeWithQuaternionAndTranslation2(&mat,&rotationQuaternion,&pos);
        XMatrix4Scale(&tmp2, scaleSceneX, scaleSceneY, scaleSceneZ);
        mat = XMatrix4Mult( &tmp2,&mat );
        this->setModelViewMatrixF(&mat.m[0]);
    }else if (cameraStyle == CAMERA_STYLE_FLIP) {
        XMatrix4 mat,tmp1,tmp2;
        XMatrix4MakeWithQuaternionAndTranslation2(&mat,&rotationQuaternionFlip,&moveSceneFlip);
        XMatrix4Scale(&tmp2, scaleSceneFlip.x, scaleSceneFlip.y, scaleSceneFlip.z);
        mat = XMatrix4Mult( &tmp2,&mat );
        this->setModelViewMatrixF(&mat.m[0]);
    }
    
    //this->resetViewMatrixToInit();
    
}

void SceneController::loadScene() {
    hintID = 0;
    
    if (!this->isValidController) {
        return;
    }
    
    clearScene();
    
    
    
    
    if (!markerXML.child("sceneParams").empty()) {
        pugi::xml_node sceneParams = markerXML.child("sceneParams");
        
        moveSceneX = sceneParams.attribute("moveX").empty() ? 0.0 : sceneParams.attribute("moveX").as_float();
        moveSceneY = sceneParams.attribute("moveY").empty() ? 0.0 : sceneParams.attribute("moveY").as_float();
        moveSceneZ = sceneParams.attribute("moveZ").empty() ? 0.0 : sceneParams.attribute("moveZ").as_float();
        
        scaleSceneX = sceneParams.attribute("scaleX").empty() ? 1.0 : sceneParams.attribute("scaleX").as_float();
        scaleSceneY = sceneParams.attribute("scaleY").empty() ? 1.0 : sceneParams.attribute("scaleY").as_float();
        scaleSceneZ = sceneParams.attribute("scaleZ").empty() ? 1.0 : sceneParams.attribute("scaleZ").as_float();
        
        rotationSceneX = sceneParams.attribute("rotationX").empty() ? 0.0 : sceneParams.attribute("rotationX").as_float();
        rotationSceneY = sceneParams.attribute("rotationY").empty() ? 0.0 : sceneParams.attribute("rotationY").as_float();
        rotationSceneZ = sceneParams.attribute("rotationZ").empty() ? 0.0 : sceneParams.attribute("rotationZ").as_float();
        
        
        
        
        moveSceneFlip.x = sceneParams.attribute("moveFlipX").empty() ? moveSceneX : sceneParams.attribute("moveFlipX").as_float();
        moveSceneFlip.y = sceneParams.attribute("moveFlipY").empty() ? moveSceneY : sceneParams.attribute("moveFlipY").as_float();
        moveSceneFlip.z = sceneParams.attribute("moveFlipZ").empty() ? moveSceneZ : sceneParams.attribute("moveFlipZ").as_float();
        
        scaleSceneFlip.x = sceneParams.attribute("scaleFlipX").empty() ? scaleSceneX : sceneParams.attribute("scaleFlipX").as_float();
        scaleSceneFlip.y = sceneParams.attribute("scaleFlipY").empty() ? scaleSceneY : sceneParams.attribute("scaleFlipY").as_float();
        scaleSceneFlip.z = sceneParams.attribute("scaleFlipZ").empty() ? scaleSceneZ : sceneParams.attribute("scaleFlipZ").as_float();
        
        rotationSceneFlip.x = sceneParams.attribute("rotationFlipX").empty() ? rotationSceneX : sceneParams.attribute("rotationFlipX").as_float();
        rotationSceneFlip.y = sceneParams.attribute("rotationFlipY").empty() ? rotationSceneY : sceneParams.attribute("rotationFlipY").as_float();
        rotationSceneFlip.z = sceneParams.attribute("rotationFlipZ").empty() ? rotationSceneZ : sceneParams.attribute("rotationFlipZ").as_float();
        
        
        
        XVector3 degress(rotationSceneX,rotationSceneY,rotationSceneZ);
        degress*=DEG_TO_RAD;
        
        XQuaternionFromEuler(&rotationQuaternion,&degress);
        
        
        XVector3 degressFlip(rotationSceneFlip.x,rotationSceneFlip.y,rotationSceneFlip.z);
        degressFlip*=DEG_TO_RAD;
        
        XQuaternionFromEuler(&rotationQuaternionFlip,&degressFlip);
        
        
        revertSwipesEvent = sceneParams.attribute("revertSwipes").empty() ? false : sceneParams.attribute("revertSwipes").as_bool();
    }
    
	// pre load all meshes !
    std::map<std::string,std::vector<std::string>> forLoad;
	for (pugi::xml_node meshSection = markerXML.child("meshs").child("mesh"); meshSection; meshSection = meshSection.next_sibling("mesh")) {
        
        for (pugi::xml_node meshSection2 = meshSection.child("mesh"); meshSection2; meshSection2 = meshSection2.next_sibling("mesh")) {
            forLoad[meshSection2.attribute("path").as_string()].push_back(meshSection2.attribute("nameObject").as_string());
        }
        
		forLoad[meshSection.attribute("path").as_string()].push_back(meshSection.attribute("nameObject").as_string());
	}

    for(std::map<std::string,std::vector<std::string>>::iterator it = forLoad.begin(); it != forLoad.end(); ++it) {
		std::vector<Mesh*> meshs;
		std::string tmp = _localFilePath;
		tmp+=it->first.c_str();
        
        _loader->loadXMESH(tmp.c_str(), &meshs, nullptr, nullptr, nullptr, false);
		
		for (uint i = 0; i < meshs.size();i++) {
			tmp = it->first;
			tmp+=meshs[i]->nameMesh;
            // not need retain because new Class cr≥eate element and not need add count
			(*_meshsCacheMap)[tmp] = meshs[i];
		}
	}
	
    for (pugi::xml_node soundSection = markerXML.child("sounds").child("sound"); soundSection; soundSection = soundSection.next_sibling("sound")) {
        int soundID = _soundSystem->addFile((char*)(_localFilePath+soundSection.attribute("path").as_string()).c_str());
        std::string systemID = "sound_";
        systemID+=soundSection.attribute("id").as_string();
        resourcesLibIDS[systemID] = soundID;
    }
    
    
    
    

    for (pugi::xml_node meshSection = markerXML.child("meshs").child("mesh"); meshSection; meshSection = meshSection.next_sibling("mesh")) {
        createNode(&meshSection);
    }
    
    

    
    loadFrameEvents();
    
    for (pugi::xml_node tapSection = markerXML.child("tapOnScreen").child("tap"); tapSection; tapSection = tapSection.next_sibling("tap")) {
        Frame*frame = (Frame*)malloc(sizeof(Frame));
        createFrame(&tapSection,frame,TYPE_ACTION_TAP);
        framesTap.push_back(frame);
    }
    
    for (pugi::xml_node virtualButtonSection = markerXML.child("virtualButtons").child("virtualButton"); virtualButtonSection; virtualButtonSection = virtualButtonSection.next_sibling("virtualButton")) {
        std::vector<Frame*>* vector = new std::vector<Frame*>();
        virtualButtonsTap[virtualButtonSection.attribute("id").as_string()] = vector;
    }

    for (pugi::xml_node tapVirtualButtonSection = markerXML.child("tapOnVirtualButton").child("tap"); tapVirtualButtonSection; tapVirtualButtonSection = tapVirtualButtonSection.next_sibling("tap")) {
        if (virtualButtonsTap.find(tapVirtualButtonSection.attribute("id").as_string()) != virtualButtonsTap.end()) {
            Frame*frame = (Frame*)malloc(sizeof(Frame));
            createFrame(&tapVirtualButtonSection,frame,TYPE_ACTION_VIRTUAL_TAP);
            std::vector<Frame*>* vector = virtualButtonsTap[tapVirtualButtonSection.attribute("id").as_string()];
            vector->push_back(frame);
        }
    }
    
    
    cleared = false;
    
    currentFrame = 0;
    
    setPositionFromCameraStyle();
}

void SceneController::loadFrameEvents() {
    frames.clear();
    
    
    
    
    for (pugi::xml_node frameSection = markerXML.child("frames").child("frame"); frameSection; frameSection = frameSection.next_sibling("frame")) {
        Frame*frame = (Frame*)malloc(sizeof(Frame));
        frame->frameID = frameSection.attribute("id").as_uint();
        
        createFrame(&frameSection,frame,TYPE_ACTION_FRAME);
        frames.push_back(frame);
    }
}

float SceneController::getVirtualButtonPosition(float val1,float val2,float val3) {
    return ((val1/2.0f)-(val1*(val2/val3)));
}

void SceneController::getVirtualsButtons(std::vector<XVirtualButton> *vector) {

    if (!this->isValidController) {
        return;
    }
    
    pugi::xml_node virtualButtons = markerXML.child("virtualButtons");
    
    
    if (virtualButtons.empty()) {
        return;
    }
    
    
    float widthTarget    = virtualButtons.attribute("widthTarget").as_float();
    float heightTarget   = virtualButtons.attribute("heightTarget").as_float();
    
    float widthOriginal  = virtualButtons.attribute("widthOriginal").as_float();
    float heightOriginal = virtualButtons.attribute("heightOriginal").as_float();
    
    
    for (pugi::xml_node virtualButtonSection = markerXML.child("virtualButtons").child("virtualButton"); virtualButtonSection; virtualButtonSection = virtualButtonSection.next_sibling("virtualButton")) {
        float x1 = -getVirtualButtonPosition(widthTarget,  virtualButtonSection.attribute("x1").as_float(),widthOriginal);
        float x2 = -getVirtualButtonPosition(widthTarget,  virtualButtonSection.attribute("x2").as_float(),widthOriginal);
        float y1 =  getVirtualButtonPosition(heightTarget, virtualButtonSection.attribute("y1").as_float(),heightOriginal);
        float y2 =  getVirtualButtonPosition(heightTarget, virtualButtonSection.attribute("y2").as_float(),heightOriginal);
        
        std::string name = virtualButtonSection.attribute("id").as_string();
        vector->push_back(XVirtualButton(name,x1,x2,y1,y2));
    }
}

void SceneController::clearActionsByType(uint type) {

    for (std::vector<Action*>::iterator it=actions.begin();it!=actions.end();){
        if((*it)->typeAction == type) {
            Action*action = (*it);
            free(action);
            it = actions.erase(it);
        } else {
            ++it;
        }
    }
}


void SceneController::statusShow(int status) {
    if (lastStatusShow != status) {
        lastStatusShow = status;
        // here need do action 
    }
}

void SceneController::updateActions() {

    for (std::vector<Action*>::iterator it=actions.begin();it!=actions.end();){
        int ret = (this->*action[(*it)->type])((*it));
        if(ret == 1) {
            Action*action = (*it);
            free(action);
            it = actions.erase(it);
        } else if (ret == 2){
            break;
        } else {
            ++it;
        }
    }
}
void SceneController::endUpdate() {
    for (uint i = 0 ; i < this->childs.size(); i++) {
        SceneNode* n = this->childs[i];
        n->endUpdate();
    }
}
void SceneController::update() {
    if (currentFrame<0) {
        return;
    }
    
    
    for (std::vector<Frame*>::iterator it=frames.begin();it!=frames.end();){
        if((*it)->frameID == currentFrame) {
            createActions((*it));
			if (!isInEditor) {
				Frame*frame = (*it);
            	free(frame);
            	it = frames.erase(it);
			}else {
				++it;
			}
        } else {
            ++it;
        }
    }
    
    updateActions();
    
    
    if (currentFrame < 3000) {
        currentFrame++;
    }
}

uint SceneController::getCurrentFrame() {
    return currentFrame;
}

void SceneController::setFrame(uint frameIndx) {
	// frame limit ) 
	if (frameIndx > 3000) return;

	if (!isInEditor) {
		// not in editor not work ! 
	}else {
		// clear current actions ! important !
		for (std::vector<Action*>::iterator it=actions.begin();it!=actions.end();){
        	Action*action = (*it);
        	free(action);
        	it = actions.erase(it);
    	}
		// set current frame to zero and in cycle play all frames
        loadScene();
		currentFrame = 0;
		for (uint i = 0;i <= frameIndx; i++) {
			update();
		}
	}

}

void SceneController::setSceneStatus(uint status) {
    statusScene = status;
}

uint SceneController::getSceneStatus() {
    return statusScene;
}




bool SceneController::isValid() {
    
    bool result = true;
    for (uint i = 0; i < this->childs.size(); i++) {
        result = result && this->childs[i]->isValidMeshNode();
    }
    return result;
    
}
