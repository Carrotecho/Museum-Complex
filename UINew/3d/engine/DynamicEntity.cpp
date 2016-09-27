//
//  DynamicEntity.cpp
//
//  Created by easy proger on 01.09.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#include "DynamicEntity.h"
#include "SkinningMath.h"
#include "XMathBase.h"

#include <sstream>



void DynamicEntity::setAnimation(XAnimation*anim) {

    haveAnimation = false;
    if (anim) {
		if (anim->skinningAnimation) {
			
            SkinAnimationInfo*info = (SkinAnimationInfo*)malloc(sizeof(SkinAnimationInfo));
            info->animation = (SkinningAnimation*) anim;
            info->currentFrameID = 0;
            info->weightTrack = 1.0;
            info->typeBlend = ANIMATION_BLEND_INTERPOLATION;
            if (skinningAnimations.size() >= 1) {
                SkinAnimationInfo*infoD = skinningAnimations[0];
                free(infoD);
                skinningAnimations.erase(skinningAnimations.begin());
            }
            skinningAnimations.push_back(info);
            //(*skinAnimIndx)=skinningAnimations.size()-1;
            haveAnimation = true;
        } else 
		if (anim->frameAnimation) {
			FrameAnimationInfo*info = (FrameAnimationInfo*)malloc(sizeof(FrameAnimationInfo));
			info->animation = anim;
			info->currentFrameID = 0;

			if (frameAnimations.size() >=1 ) {
                FrameAnimationInfo*infoD = frameAnimations[0];
                free(infoD);
                frameAnimations.erase(frameAnimations.begin());
            }
            frameAnimations.push_back(info);
            haveAnimation = true;
            //(*frameAnimIndx)=frameAnimations.size()-1;
		}
        if (anim->morphAnimation) {
            MorphAnimationInfo*info = (MorphAnimationInfo*)malloc(sizeof(MorphAnimationInfo));
            info->animation = anim;
            info->currentFrameID = 0;
            
            if (morphAnimations.size() >= 1) {
                MorphAnimationInfo*infoD = morphAnimations[0];
                free(infoD);
                morphAnimations.erase(morphAnimations.begin());
            }
            morphAnimations.push_back(info);
            haveAnimation = true;
            //(*morphAnimIndx)=morphAnimations.size()-1;
        }
        
    }
}



void DynamicEntity::updateFrameAnimation(int frameID) {
    FrameAnimationInfo *animInfo = frameAnimations[0];
    animInfo->currentFrameID = frameID;
    if (animInfo->currentFrameID > animInfo->animation->numAnimationFrames-1) {
        animInfo->currentFrameID= 0;
    }
    this->lastFrameID = animInfo->currentFrameID;
    AnimationFrame* frameAnimation = &animInfo->animation->anim_frames[animInfo->currentFrameID];
    if (!frameAnimation) return;
    
    // frame animations can be only 1 ! no blend mode !


	XMatrix4MakeWithArray(&frameMatrix, &frameAnimation->mat.m[0]);
}

void DynamicEntity::calculateTransformMatrix() {
	SceneNode::calculateTransformMatrix();

	if (frameAnimations.size()) {
        memcpy(&tmpMatrix2.m[0], &modelViewMatrix.m[0],sizeof(XMatrix4));
		XMatrix4Mult(&modelViewMatrix,&tmpMatrix2,&frameMatrix); // local + frame 
	}
}

void DynamicEntity::updateSkinAnimation(int frameID) {
    // diabled blend tracks !
	if(!skinningAnimations.size()) return;
    calculatedSkinFrame = NULL;
    SkinAnimationInfo *animInfo = skinningAnimations[0];
    animInfo->currentFrameID = frameID;
    if (animInfo->currentFrameID > animInfo->animation->numAnimationFrames-1) {
        animInfo->currentFrameID = 0;
    }
    this->lastFrameID = animInfo->currentFrameID;
    
    if (skinCacheCollections->size() > 1000) {
        std::map<std::string, SkinningFrame*>::iterator it=skinCacheCollections->begin();
        for (int i = 0; i<100; i++) {
            removeSkinningFrame(it->second);
            skinCacheCollections->erase(it++);
        }
    }
    
    std::string udid = "";
    
    for (unsigned int i = 0; i < skinningAnimations.size(); i++) {
        SkinAnimationInfo *animInfo = skinningAnimations[i];
        if (animInfo) {
            if (!animInfo->animation->isValid) {
                continue;
            }
            char str[255];
            itostr(animInfo->animation->idInCache,str);
            udid+=str;
            udid+="-";
            itostr(animInfo->currentFrameID,str);
            udid+=str;
            udid+="|";
        }
    }

    
    std::map<std::string, SkinningFrame*>::iterator it=skinCacheCollections->find(udid);
    if(it!=skinCacheCollections->end()) {
        calculatedSkinFrame = (*it).second;
    }else {
        // math skinning !!!
        Mesh*mesh = this->getMesh();
        if (mesh && mesh->bindFrame) {
            SkinningFrame*frame = getSkinningFrame(&skinningAnimations,mesh->bindFrame,mesh->numBones,&mesh->boneIerarhy);
            (*skinCacheCollections)[udid] = frame;
            
            calculatedSkinFrame = frame;
        }
    }
    
}
void DynamicEntity::updateMorph(int frameID) {
    
    if (!GLSyncExtEnabled) {
        return;
    }
    
    waitEndUpdate = true;
    MorphAnimationInfo* animInfo = morphAnimations[0];
    animInfo->currentFrameID = frameID;
    if (animInfo->currentFrameID > animInfo->animation->numAnimationFrames-1) {
        animInfo->currentFrameID = 0;
    }
    
    MorphAnimationFrame* morphAnimationFrame = &animInfo->animation->morph_anim_frames[animInfo->currentFrameID];
    if (!morphAnimationFrame) return;
    
    Mesh*mesh = this->getMesh();
    if (!mesh || mesh->isDamaged || !mesh->morphVertOriginal) return;
    
    
    
    MorphInfo*morpInfo = &mesh->morphInfo[0];
    uint numAffectedVertex = morpInfo->numAffectedVertex;
    uint numChunnels = morpInfo->numChannels;
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBufferID[0]);
    uint offset = 0;
    uint length = numAffectedVertex*mesh->sizeVertex;
    float *vertices = (float*)glMapBufferRangeEXT(GL_ARRAY_BUFFER, offset, length,
                                                  GL_MAP_WRITE_BIT_EXT | GL_MAP_FLUSH_EXPLICIT_BIT_EXT |
                                                  GL_MAP_UNSYNCHRONIZED_BIT_EXT );
    
    
    uint numFloats = mesh->sizeFloats;
    for (uint i = 0; i < numAffectedVertex; i++) {
        
        vertices[(i*numFloats)  ] = mesh->morphVertOriginal[(i*numFloats) ];
        vertices[(i*numFloats)+1] = mesh->morphVertOriginal[(i*numFloats)+1];
        vertices[(i*numFloats)+2] = mesh->morphVertOriginal[(i*numFloats)+2];
        
		XVector3 test;
		test.x = vertices[(i*numFloats)];
		test.y = vertices[(i*numFloats) + 1];
		test.z = vertices[(i*numFloats) + 2];

        for (uint channelI = 0; channelI < numChunnels; channelI++) {
			float weight = 100.0-morphAnimationFrame->weights[channelI];
 
            XVector3*delta = &morpInfo->channels[channelI].deltas[i];
            vertices[(i*numFloats)  ] -= delta->x * weight;
            vertices[(i*numFloats)+1] -= delta->y * weight;
            vertices[(i*numFloats)+2] -= delta->z * weight;
        }
    }
    
    // Wait for fence (set below) before modifying buffer.
    glClientWaitSyncAPPLE(fence, GL_SYNC_FLUSH_COMMANDS_BIT_APPLE,
                          GL_TIMEOUT_IGNORED_APPLE);
    
    
    
    glFlushMappedBufferRangeEXT(GL_ARRAY_BUFFER, offset, length);
    glUnmapBufferOES(GL_ARRAY_BUFFER);

}



XVector3* DynamicEntity::getManipulatorPosition() {
	if (skinningAnimations.size()) return &zeroPos;
	return &_pos;
}

void DynamicEntity::update(uint frameID, double delta) {

    if (morphAnimations.size()) {
        this->updateMorph(frameID);
    }
    // only skin or frame ! not same time
    if (skinningAnimations.size()) {
        this->lastFrameID = frameID;
        //this->updateSkinAnimation(frameID);
    }else if (frameAnimations.size()) {
        this->updateFrameAnimation(frameID);
    }
}


void DynamicEntity::endUpdate() {
    
    for (uint i = 0; i < childs.size(); i++) {
        childs[i]->endUpdate();
    }
    
    
    
    if (!waitEndUpdate) {
        return;
    }
    fence = glFenceSyncAPPLE(GL_SYNC_GPU_COMMANDS_COMPLETE_APPLE, 0);
    waitEndUpdate = false;
}


SkinningFrame* DynamicEntity::getSkinAnimation(bool update) {
    this->updateSkinAnimation(this->lastFrameID);
    return calculatedSkinFrame;
}

char* DynamicEntity::itoa(int val, int base){
    
    static char buf[32] = {0};
    
    int i = 30;
    
    for(; val && i ; --i, val /= base)
        
        buf[i] = "0123456789abcdef"[val % base];
    
    return &buf[i+1];
    
}