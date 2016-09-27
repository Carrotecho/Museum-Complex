//
//  FileLoader.cpp
//  
//
//  Created by easy proger on 05.07.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//
#define _CRT_SECURE_NO_WARNINGS
#include "FileLoader.h"

#include "Mesh.h"
#include "SkinnedMesh.h"

#include <string>
#include "forsyth.h"

#include "FlagsForShaderCompile.h"
#include "XMatrix4Math.h"
#include "SkinningMath.h"
#include "Camera.h"
#include "BaseLight.h"
#include "ParticleEmiter.hpp"

FileLoader::FileLoader(MeshCache*meshCache,XAnimations*animationsCache) {
    _meshCache = meshCache;
	_animationsCache = animationsCache;
    
    
    genericID = 0;
}





// new life )

int FileLoader::getNumMeshs(std::vector<SceneNode*> *childsList) {
	int ret = 0;
	for (uint i = 0; i < childsList->size(); i++) {
		SceneNode*node = (*childsList)[i];
		if (node->typeObject == MESH_OBJECT) {
			if (node->getMesh() != NULL) {
				if (!node->isRemoved()) ret++;
			}
		}
		else if (node->typeObject == PARTICLES_OBJECT) {
			if (!node->isRemoved()) ret++;
		}
	}
	return ret;
}

long FileLoader::getEndFile(FILE*file, uint numMeshs) {
	long lastOffset = ftell(file);
	fseek(file, 0, SEEK_END); // go to end
	long offsetEnd = ftell(file);
	offsetEnd -= 4*(numMeshs * 2); //
	fseek(file, lastOffset, SEEK_SET);
	return offsetEnd;
}


void FileLoader::safeParticle(ParticleInfo*emiter) {

}

void FileLoader::safeXMESH(const char*path,std::vector<SceneNode*> *childsList, std::vector<CameraInfo>*cameras, std::vector<LightInfo>*lights) {
	std::string tmpPath = path;
	tmpPath += "_tmp";

	FILE* fileOut = fopen(tmpPath.c_str(), "ab+");
	if (!fileOut) {
		return;
	}

	float version = 0.2;
	int numMeshes  = getNumMeshs(childsList);
	int numLights  = lights->size();
	int numCameras = cameras->size();

	fwrite(&version, 1, sizeof(float), fileOut);//version
	fwrite(&numMeshes, 1, sizeof(int), fileOut);//numMeshes

	fwrite(&numCameras, 1, sizeof(int), fileOut);//numCameras
	fwrite(&numLights, 1, sizeof(int), fileOut);//numLights

	


	for (uint i = 0; i < numCameras; i++) {
		fwrite(&(*cameras)[i], 1, sizeof(CameraInfo), fileOut);
	}
	for (uint i = 0; i < numLights; i++) {
		fwrite(&(*lights)[i], 1, sizeof(LightInfo), fileOut);
	}

	float freeData[20];
	fwrite(&freeData, 1, sizeof(float) * 20, fileOut);//numLights

	std::vector<long> offsetsOut, sizesOut;

	for (uint i = 0; i < childsList->size();i++) {
		SceneNode*node = (*childsList)[i];

		if (node->typeObject != MESH_OBJECT && node->typeObject != PARTICLES_OBJECT) continue;



		if (node->typeObject == PARTICLES_OBJECT) { 

			offsetsOut.push_back(ftell(fileOut));
			sizesOut.push_back(sizeof(ParticleInfo) + sizeof(int));

			int typeObject = FileObjectTypes_Particle;
			fwrite(&typeObject, 1, sizeof(int), fileOut);
			ParticleInfo info = ((ParticleEmiter*)node)->getInfo();
			info.typeGeneric = TypeGenericObjects::GenericObject_PARTICLE;
			fwrite(&info, 1, sizeof(ParticleInfo), fileOut);
			continue;
		}


		Mesh*mesh = node->getMesh();
		if (mesh == NULL) continue;

		FILE* fileMesh = fopen(mesh->pathForFile.c_str(), "rb");

		if (!fileMesh) {
			throw "can't open file "+ mesh->pathForFile;
			fclose(fileOut);
			return;
		}

		float versionInMesh = 0.2;
		int numMeshesInFileMesh = 0;

		fread(&versionInMesh, 1, sizeof(float), fileMesh);//version
		fread(&numMeshesInFileMesh, 1, sizeof(int), fileMesh);//numMeshes

		std::vector<long> offsets, sizes;
		offsets.resize(numMeshesInFileMesh);
		sizes.resize(numMeshesInFileMesh);
		loadOffsets(fileMesh, numMeshesInFileMesh, &offsets, &sizes);

		long offsetData  = offsets[mesh->indexMeshInFile];
		long endMeshData = mesh->indexMeshInFile >= numMeshesInFileMesh-1 ? getEndFile(fileMesh, numMeshesInFileMesh) : offsets[mesh->indexMeshInFile+1];
		long lengthData  = endMeshData - offsetData;

		fseek(fileMesh, offsetData, SEEK_SET);


		


		long offsetinOutStartData = ftell(fileOut);

		offsetsOut.push_back(offsetinOutStartData);
		sizesOut.push_back(sizes[mesh->indexMeshInFile]);


		byte*data = (byte*)malloc(lengthData);
		fread(data,  1, lengthData, fileMesh); // load data


		long offsetToPositiondata =  sizeof(int) /* type object */ + (sizeof(float) * 20) /* free data */;


		WritedData*writeData = (WritedData*)&data[offsetToPositiondata];
		writeData->quaternion = node->getQuaternion();
		writeData->scale = node->getScale();
		writeData->translation = node->getPosition();




		fwrite(data, 1, lengthData, fileOut); // safe data
		free(data);
		fclose(fileMesh);
	}

	for (long i = 0; i < offsetsOut.size(); i++) {
		long offset = offsetsOut[i];
		long size = sizesOut[i];
		fwrite(&offset, 1, 4, fileOut); // save offsets !
		fwrite(&size, 1, 4, fileOut); // save size mesh data !
	}

	fclose(fileOut);


	int result = remove(path);
	result = rename(tmpPath.c_str(), path);
}

void FileLoader::loadXMESH(const char*path,std::vector<Mesh*>*meshs, std::vector<GenericObjects*>*genericNodes,std::vector<CameraInfo>*cameras,std::vector<LightInfo>*lights,bool loadSceneData) {

	FILE* file = fopen(path, "rb");
	if (!file) {
		return;
	}

	float version = 0;

	int numMeshes = 0;
	int numLights = 0;
	int numCameras = 0;

	fread(&version, 1, sizeof(float), file);//version
	fread(&numMeshes, 1, sizeof(int), file);//numMeshes
	
	if (loadSceneData) {
		fread(&numCameras, 1, sizeof(int), file);//numCameras
		fread(&numLights, 1, sizeof(int), file);//numLights


		if (cameras == nullptr) {
			// need file seek
			long currentOffset = ftell(file);
			currentOffset += sizeof(CameraInfo)*numCameras;
			fseek(file, currentOffset, SEEK_SET);
		}
		else {
			cameras->resize(numCameras);
			for (uint i = 0; i < numCameras; i++) {
				fread(&(*cameras)[i], 1, sizeof(CameraInfo), file);
			}
		}

		if (lights == nullptr) {
			long currentOffset = ftell(file);
			currentOffset += sizeof(LightInfo)*numLights;
			fseek(file, currentOffset, SEEK_SET);
		}
		else {
			lights->resize(numLights);
			for (uint i = 0; i < numLights; i++) {
				fread(&(*lights)[i], 1, sizeof(LightInfo), file);
			}
		}
	}


	std::vector<long> offsets, sizes;
	offsets.resize(numMeshes);
	sizes.resize(numMeshes);
	loadOffsets(file, numMeshes, &offsets, &sizes);

	for (uint i = 0; i < offsets.size(); i++) {

		if (i == 242) {
			printf("reached\n");
		}


		printf("%d\n", i);
		fseek(file, offsets[i], SEEK_SET);

		

		int typeObject = -1;
		fread(&typeObject, 1, sizeof(int), file);

		if (typeObject == FileObjectTypes_Particle && genericNodes != nullptr) {

			ParticleInfo *info = new ParticleInfo;
			info->typeGeneric = TypeGenericObjects::GenericObject_PARTICLE;

			fread(info, 1, sizeof(ParticleInfo), file);
			genericNodes->push_back(info);
			continue;
		}


		Mesh*newMesh;

		if (_meshsCacheMap.find(path) != _meshsCacheMap.end() && _meshsCacheMap[path].size() > i && _meshsCacheMap[path][i] != nullptr) {
			newMesh = _meshsCacheMap[path][i];
		}
		else {
			newMesh = new Mesh();
            newMesh->udid = genericID++;
			if (_meshsCacheMap[path].size() < i + 1) {
				_meshsCacheMap[path].resize(i + 1);
			}
			_meshsCacheMap[path][i] = newMesh; // count used 1 !
		}

		if (newMesh->isDamaged) {
			newMesh->clear();

			newMesh->pathForFile = path;
			newMesh->indexMeshInFile = i;
			newMesh->offsetInFile = ftell(file);

			bool exportedAnimations = false;

			std::string nameMesh = loadStaticMesh(file, newMesh, version, true, &exportedAnimations);

			if (newMesh->morphed) newMesh->typeVBO = GL_DYNAMIC_DRAW;
			_meshCache->fillGPU(newMesh);

			newMesh->nameMesh = nameMesh;


			int assignedBaseAnimationsID = -1;
			if (exportedAnimations) {
				assignedBaseAnimationsID = _animationsCache->getAnimationID((path + nameMesh).c_str());
				if (assignedBaseAnimationsID == -1) {
					long offsetInFile = ftell(file);
					XAnimation* anim = readXMESHAnimation(file);
					anim->offsetInFile = offsetInFile;
					assignedBaseAnimationsID = _animationsCache->addAnimation((path + nameMesh).c_str(), anim);
				}
			}
			newMesh->assignedBaseAnimationsID = assignedBaseAnimationsID;
		}

		meshs->push_back(newMesh);
	}

	fclose(file);
}


void FileLoader::clear() {
    
    std::map<std::string,std::vector<Mesh*>>::iterator it = _meshsCacheMap.begin();
    while ( it != _meshsCacheMap.end() ) {
        for (int i = 0; i < it->second.size(); i++) {
            Mesh * mesh = it->second[i];
            mesh->release();
        }
        it = _meshsCacheMap.erase(it);
    }
    
    
}



XAnimation* FileLoader::readXMESHAnimation(FILE*file) {

	float freedata[20];
	fread(&freedata[0], 1, sizeof(float) * 20, file);// 





	XAnimation* anim = NULL;
    int lenghtName = 0;
    fread(&lenghtName,    1,    sizeof(int), file);// len name

    char* nameMesh = (char*)malloc(sizeof(char)*lenghtName+1); // alloc mem
    // loading !
    memset(nameMesh,    0, lenghtName*sizeof(char));
    fread(&nameMesh[0],    1, sizeof(char)*lenghtName, file);// load name
        
    nameMesh[lenghtName] = 0;

    std::string nameMeshStd = nameMesh;

    free(nameMesh);
    nameMesh = NULL;
        

	bool skinning = false;
	bool frameAnimation = false;
    bool morphAnimation = false;
    
    
	fread(&skinning, 1, sizeof(char), file); // 
	fread(&frameAnimation, 1, sizeof(char), file); //
    fread(&morphAnimation, 1, sizeof(char), file); //
    

	int numAnimationFrames = 0;
	fread(&numAnimationFrames,    1,    sizeof(int), file);//tx

    if (skinning) {// skinning
        int numBones = 0;
            
        fread(&numBones,    1,    sizeof(int), file);//tx

        SkinningAnimation *skinAnim = new SkinningAnimation();
	    skinAnim->skinningAnimation = true;
            
        if (skinAnim) {
            skinAnim->frames = (SkinningFrame *)malloc(sizeof(SkinningFrame)*numAnimationFrames);
            skinAnim->setNumBones(numBones);
            skinAnim->setNumAnimationFrames(numAnimationFrames);
            skinAnim->bones.resize(numBones);
            for (int f = 0; f < numAnimationFrames; f++) {
                    
                SkinningFrame*frame = &skinAnim->frames[f];
                frame->pos = (XVector3*)malloc(sizeof(XVector3)*numBones);
                frame->rot = (XVector4*)malloc(sizeof(XVector4)*numBones);
                frame->parentID = (int*)malloc(sizeof(int)*numBones);
                    
                for (int bi = 0; bi < numBones;bi++){
                    fread(&frame->parentID[bi],1, sizeof(int), file);
                    fread(&frame->pos[bi],1, sizeof(XVector3), file);
                    fread(&frame->rot[bi],1, sizeof(XVector4), file);
                    if (f == 0) {
                        skinAnim->bones[bi].boneID = bi;
                        skinAnim->bones[bi].parentID = frame->parentID[bi];
                    }
                }
            }
            sortBones(&skinAnim->bones);
        }
        anim = skinAnim;

    }
	if (frameAnimation) {
		if (!anim) anim = new XAnimation();

		anim->frameAnimation = true;
		anim->anim_frames = (AnimationFrame *)malloc(sizeof(AnimationFrame)*numAnimationFrames);
		anim->setNumAnimationFrames(numAnimationFrames);
		for (int f = 0; f < numAnimationFrames; f++) {
			AnimationFrame*frame = &anim->anim_frames[f];
			fread(&frame->mat,1,   sizeof(XMatrix4), file);
		}
	}
    
    
    if (morphAnimation) {
        if (!anim) anim = new XAnimation();
        
        int numChannels = 0;
        fread(&numChannels, 1, sizeof(int), file);
        
        anim->morphAnimation = true;
        anim->morph_anim_frames = (MorphAnimationFrame *)malloc(sizeof(MorphAnimationFrame)*numAnimationFrames);
        anim->setNumAnimationFrames(numAnimationFrames);
        
        for (int f = 0; f < numAnimationFrames; f++) {
            MorphAnimationFrame*frame = &anim->morph_anim_frames[f];
            frame->numChannels = numChannels;
            frame->weights = (float *)malloc(numChannels*sizeof(float));
            fread(frame->weights,1,   numChannels*sizeof(float), file);
        }
    }
    
   
    anim->countUsed = 1;
    anim->isValid = true;
    
    return anim;
}



std::string FileLoader::loadStaticMesh(FILE*file,Mesh*model,float version,bool reload,bool* exportAnimations) {

	

	float freedata[20];
	fread(&freedata[0], 1, sizeof(float) * 20, file);

	long lastoffsetinOut = ftell(file);

	fread(&model->initQuaternion, 1, sizeof(XVector4), file);//tx
	fread(&model->initScale, 1, sizeof(XVector3), file);//tx
	fread(&model->initTranslation, 1, sizeof(XVector3), file);//tx

	fread(&model->bboxMax, 1, sizeof(XVector3), file);//numVertex
	fread(&model->bboxMin, 1, sizeof(XVector3), file);//numVertex
	fread(&model->bboxCenter, 1, sizeof(XVector3), file);//numVertex


    int lenghtName = 0;
    fread(&lenghtName,    1,    sizeof(int), file);// len name
    char* nameMesh = (char*)malloc(sizeof(char)*(lenghtName+1)); // alloc mem
    fread(&nameMesh[0],    1, sizeof(char)*lenghtName, file);// load name
    
	nameMesh[lenghtName] = 0;
    std::string nameMeshStd = "";
	nameMeshStd+=nameMesh;
    printf("%s\n",nameMesh);
    free(nameMesh);
    nameMesh = NULL;
    
    bool haveTX,haveT,haveB,haveN,haveC,skinned,morphed;
    
    morphed = false;
    
    fread(&haveTX,    1,    sizeof(char), file);//tx
    fread(&haveT,     1,    sizeof(char), file);//t
    fread(&haveB,     1,    sizeof(char), file);//b
    fread(&haveN,     1,    sizeof(char), file);//n
    fread(&haveC,     1,    sizeof(char), file);//c
    fread(&skinned,   1,    sizeof(char), file);//c
    fread(&morphed,   1,    sizeof(char), file);//c
    fread(exportAnimations,   1,    sizeof(char), file);//c
    

	if (skinned) {
		model->initQuaternion = XVector4(0, 0, 0, 0);
		model->initRotation = XVector3(0, 0, 0);
		model->initScale = XVector3(1, 1, 1);
		model->initTranslation = XVector3(0, 0, 0);
	}

    fread(&model->numAllVertex, 1,    sizeof(int),  file);//numVertex
    fread(&model->numAllFaces,  1,    sizeof(int),  file);//numFaces
    
    uint maxVertexInChunk;
    fread(&maxVertexInChunk, 1,    sizeof(int),  file);//
    
    uint numChunks;
    fread(&numChunks, 1,    sizeof(int),  file);//
    
    model->numTextureCoordinats = haveTX;
    model->tangent  = haveT;
    model->binormal = haveB;
    model->normal   = haveN;
    model->color    = haveC;
    model->skinned  = skinned;
    model->morphed  = morphed;

    
    uint sizeVertex = model->getSizeVertex();
    
    model->numChunks = numChunks;
    
    for (unsigned int i = 0; i < numChunks; i++) {
        
        uint chunkNumVertx,chunkNumFaces;
        fread(&chunkNumVertx, 1,    sizeof(int),  file);//
        fread(&chunkNumFaces, 1,    sizeof(int),  file);//
        
        model->numVertexC[i] = chunkNumVertx;
        model->numFacesC[i]  = chunkNumFaces;
        
        uint sizeChunkVertex = chunkNumVertx*sizeVertex;
        uint sizeChunkFaces  = chunkNumFaces*(3*sizeof(facesType));
        
        model->vertex[i] = malloc(sizeChunkVertex);
        model->faces[i]  = malloc(sizeChunkFaces);
        
        fread(model->vertex[i],1, sizeChunkVertex, file);
        
        if (morphed) {
            // need here loading morph;
            
            int numMorphedVertex = 0;
            fread(&numMorphedVertex, 1,    sizeof(int),  file);//
            int numChannels = 0;
            fread(&numChannels, 1,    sizeof(int),  file);//
            
            MorphInfo* morphInfo         = &model->morphInfo[i];
            morphInfo->numChannels       = numChannels;
            morphInfo->numAffectedVertex = numMorphedVertex;
            
            model->morphVertOriginal = (float*)malloc(numMorphedVertex* sizeVertex);
            
            morphInfo->channels = (MorphChannelInfo*)malloc(numChannels*sizeof(MorphChannelInfo));
            


            for (int channelID = 0; channelID < numChannels; channelID++) {
                morphInfo->channels[channelID].deltas = (XVector3*)malloc(numMorphedVertex*sizeof(XVector3));
                fread(morphInfo->channels[channelID].deltas, 1,    numMorphedVertex*sizeof(XVector3),  file);//
            }
            // backUP morphVertex
            memcpy(model->morphVertOriginal, model->vertex[i], numMorphedVertex* sizeVertex);
        }
        
        
        fread(model->faces[i],           1, sizeChunkFaces,  file);
        
        
        //model->faces[i] = reorderForsyth(fases, chunkNumFaces, chunkNumVertx);
        //free(fases);
    }
    
    
    
    
    
    if (skinned) {
        unsigned int numBones = 0;
        fread(&numBones, 1,    sizeof(int),  file);//
        
        Mesh*skinnedMesh = model;
		model->initSkin();

        if (skinnedMesh) {
            
            skinnedMesh->vertexDeclaration|=SKINING_MESH;
            skinnedMesh->numBones = numBones;
            
            skinnedMesh->bindFrame->pos = (XVector3*)malloc(sizeof(XVector3)*numBones);
            skinnedMesh->bindFrame->rot = (XVector4*)malloc(sizeof(XVector4)*numBones);
            skinnedMesh->bindFrame->parentID = (int*)malloc(sizeof(int)*numBones);
            skinnedMesh->bindFrame->mat = (XMatrix4*)malloc(sizeof(XMatrix4)*numBones);
            
            
            skinnedMesh->boneIerarhy.resize(numBones);
            for (unsigned int i = 0; i < numBones;i++) {
                fread(&skinnedMesh->bindFrame->parentID[i],1, sizeof(int), file);
                fread(&skinnedMesh->bindFrame->pos[i],1, sizeof(XVector3), file);
                fread(&skinnedMesh->bindFrame->rot[i],1, sizeof(XVector4), file);
                skinnedMesh->boneIerarhy[i].boneID = i;
                skinnedMesh->boneIerarhy[i].parentID = skinnedMesh->bindFrame->parentID[i];
            }
            sortBones(&skinnedMesh->boneIerarhy);
            
            
            for (unsigned int bi = 0; bi < skinnedMesh->boneIerarhy.size(); bi++) {
                BoneInfo*boneInfo = &skinnedMesh->boneIerarhy[bi];
                XMatrix4MakeWithQuaternionAndTranslation2(&skinnedMesh->bindFrame->mat[boneInfo->boneID], &skinnedMesh->bindFrame->rot[boneInfo->boneID], &skinnedMesh->bindFrame->pos[boneInfo->boneID]);
            }
            
    
            
        }else {
            throw "not skinned mesh !";
        }
    }
    
    
    
    
    
    return nameMeshStd;
}







void FileLoader::reloadChunkInMesh(Mesh*model) {

	FILE* file = fopen(model->pathForFile.c_str(), "rb");
	if (!file) {
		printf("error open file: %s\n", model->pathForFile.c_str());
		return;
	}

	if (model->offsetInFile == 0) {
		printf("error in mesh offset!\n");
		return;
	}

	float version = 0;
	fread(&version, 1, sizeof(float), file);//version
    fseek(file, model->offsetInFile, SEEK_SET);
    bool exportAnimations = false;
	loadStaticMesh(file, model, version, true,&exportAnimations);
	_meshCache->fillGPU(model);

	fclose(file);
}



void FileLoader::loadOffsets(FILE*file,int numMeshes,std::vector<long>*offsets, std::vector<long>*sizes) {
	long lastOffset = ftell(file);
	fseek ( file, 0, SEEK_END ); // go to end
	long offsetEnd = ftell ( file );
    
    
    
	offsetEnd-=4*(numMeshes*2); //
	fseek(file, offsetEnd, SEEK_SET); // offsets 

	for (int i = 0;i < numMeshes;i++) {
		long offset,size;
	    fread(&offset,  1, 4, file); // C
		fread(&size, 1, 4, file); // C
		(*offsets)[i] = offset;
		(*sizes)[i] = size;
	}
	fseek(file, lastOffset, SEEK_SET);
}

















