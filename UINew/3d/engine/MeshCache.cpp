//
//  MeshCache.cpp
//  ShadowMapsTestProject
//
//  Created by easy on 25.11.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "MeshCache.h"
#include "ResourceManager.h"

using namespace std;

namespace Xengine
{
    
    MeshCache::MeshCache(std::map<uint, Mesh*>* damagedMeshs)
    {
        currentIDVBOV = -1;
        currentIDVBOI = -1;
        createdNumVBO = -1;
        createdNumIBO = -1;
        
        for (int i = 0; i < MAX_VBOS; i++) {
            modelVertexBuffers[i] = 0;
            modelIndexBuffers[i] = 0;
        }
        

        
        
		_damagedMeshs = damagedMeshs;
        initialize();
    }
    MeshCache::~MeshCache()
    {
        clear();
    }
    
    
    void MeshCache::createVBOVertex(uint type,uint size,void*data,bool restoreBuffer){
        if (!restoreBuffer) {
            currentIDVBOV++;
            createdNumVBO++;
        }
        
        glGenBuffers(1, &modelVertexBuffers[currentIDVBOV]);
        glBindBuffer(GL_ARRAY_BUFFER, modelVertexBuffers[currentIDVBOV]);
        glBufferData(GL_ARRAY_BUFFER, size, data, type);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        typeModelBuffer[currentIDVBOV] = type;
        sizeModelBuffer[currentIDVBOV] = size;
        
        offsetModelVertexBuffer[currentIDVBOV] = 0;
    }
    
    void MeshCache::createVBOIndex(){
        currentIDVBOI++;
        
        glGenBuffers(1, &modelIndexBuffers[currentIDVBOI]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIndexBuffers[currentIDVBOI]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxSizeVBO, 0, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        offsetModelIndexBuffer[currentIDVBOI]  = 0;
        createdNumIBO++;
    }
    
    
    void MeshCache::initialize()
    {
        createVBOVertex();
        createVBOIndex();
    }
    
    void MeshCache::clear() {
        for (int i = 0; i < MAX_VBOS; i++) {
            if (modelVertexBuffers[i]) {
                glBindBuffer(GL_ARRAY_BUFFER, modelVertexBuffers[i]);
                glDeleteBuffers(1, &modelVertexBuffers[i]);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                std::vector<StoredData*>::iterator it = storedDatasV[i].begin();
                while ( it != storedDatasV[i].end() )
                {
                    StoredData *data = *it;
                    removeChunk(data->mesh, data->indexChunk, &it, nullptr);
                }
                
            }
            if (modelIndexBuffers[i]) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIndexBuffers[i]);
                glDeleteBuffers(1, &modelIndexBuffers[i]);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                XLOG("MeshCache clear IBO bufferID:%d storeDataSize:%d",modelIndexBuffers[i],storedDatasI[i].size());
                std::vector<StoredData*>::iterator itI = storedDatasI[i].begin();
                while ( itI != storedDatasI[i].end() )
                {
                    StoredData *data = *itI;
                    removeChunk(data->mesh, data->indexChunk, nullptr, &itI);
                }
                XLOG("MeshCache clear IBO storeDataSize:%d",storedDatasI[i].size());
            }
            
        }
        
        
        
        
        
    }
    
    
    void MeshCache::removeVBO(uint index) {
        int foundIndex = findIndexBufferbyID(modelVertexBuffers,index);
       
        
        if (foundIndex == -1) {
            return;
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, modelVertexBuffers[foundIndex]);
        glDeleteBuffers(1, &modelVertexBuffers[foundIndex]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        for (uint i = 0; i < storedDatasV[foundIndex].size(); i++) {
            StoredData*data = storedDatasV[foundIndex][i];
			removeChunk(data->mesh, data->indexChunk, nullptr, nullptr);
        }
        

        storedDatasV[foundIndex].clear();
        modelVertexBuffers[foundIndex] = 0;
        sizeModelBuffer[foundIndex] = 0;
    }
  
	StoredData* MeshCache::findStoredData(std::vector<StoredData*>*vectorFind, uint meshIndex, uint chunkIndex,int * indexFind) {
        
		for (uint i = 0; i < vectorFind->size();i++) {
			StoredData* tmp = (*vectorFind)[i];
			if (tmp->mesh->udid == meshIndex && tmp->indexChunk == chunkIndex) {
				*indexFind = i;
				return tmp;
			}
		}
		return nullptr;
	}

    
    int MeshCache::findIndexBufferbyID(GLuint*inBuffer,int index){
        int foundIndex = -1;
        
        for (uint i = 0; i < MAX_VBOS; i++) {
            if (inBuffer[i] == index) {
                foundIndex = i;
                break;
            }
        }
        return foundIndex;
    }
    
	void MeshCache::removeChunk(Mesh*damagedMesh, uint chunkIndex, std::vector<StoredData*>::iterator *itV, std::vector<StoredData*>::iterator *itI) {
        XLOG("enter removeChunk");
		// remove index data 

		uint indexBufferID  = findIndexBufferbyID( modelIndexBuffers,damagedMesh->indexBufferID[chunkIndex]);
		uint vertexBufferID = findIndexBufferbyID(modelVertexBuffers, damagedMesh->vertexBufferID[chunkIndex]);

		damagedMesh->setAsDamagedVBOChunk(chunkIndex);
		damagedMesh->setAsDamagedIBOChunk(chunkIndex);

		
		std::vector<StoredData*>*vecVertexStored = &storedDatasV[vertexBufferID];
		std::vector<StoredData*>*vecIndexStored = &storedDatasI[indexBufferID];

        XLOG("before finddata removeChunk");
        
		int vertexStoredIndex = 0;
		int indexStoredIndex = 0;
		StoredData*vertexStoredData = findStoredData(vecVertexStored,damagedMesh->udid, chunkIndex,&vertexStoredIndex);
		StoredData*indexStoredData  = findStoredData(vecIndexStored, damagedMesh->udid, chunkIndex,&indexStoredIndex);

		if (vertexStoredData != nullptr) {
            if (itV != nullptr) {
                
				*itV = vecVertexStored->erase(vecVertexStored->begin() + vertexStoredIndex);
			} else {
				vecVertexStored->erase(vecVertexStored->begin() + vertexStoredIndex);
			}
			free(vertexStoredData);
		}
		else {
			if (itV != nullptr) {
				*itV+=1;
			}
		}

        XLOG("after clear vertex stored data removeChunk");
        
		if (indexStoredData != nullptr) {
            if (itI != nullptr) {
                
				*itI = vecIndexStored->erase(vecIndexStored->begin() + indexStoredIndex);
			} else {
				vecIndexStored->erase(vecIndexStored->begin() + indexStoredIndex);
			}
			free(indexStoredData);
		}
		else {
			if (itI != nullptr) {
				*itI+=1;
			}
		}
        XLOG("after clear index stored data removeChunk");
        
        
		// check 
		bool damagedAllMesh = damagedMesh->isDamagedAll();
		if (damagedAllMesh) {
			damagedMesh->release(); //mesh damaged need release 
		}
		damagedMesh->isUsed = false;

        XLOG("after release mesh index stored data removeChunk");
        
        
		//if (_damagedMeshs->find(damagedMesh->udid) != _damagedMeshs->end())
		//(*_damagedMeshs)[damagedMesh->udid] = damagedMesh;
        
        XLOG("end removeChunk");
	}

    void MeshCache::loadVChunk(Mesh*model,uint chunkI) {
        // restore buffer if deleted
        if (sizeModelBuffer[currentIDVBOV] == 0) {
            createVBOVertex(GL_STATIC_DRAW,maxSizeVBO,0,true);
        }
        
        if (sizeModelBuffer[currentIDVBOV] == 0) {
            printf("can't restore buffer \n");
            return;
        }
        
        uint sizeVertexData = model->numVertexC[chunkI] * model->sizeVertex;
        

		if (createdNumVBO < MAX_VBOS - 1) {
			if (model->typeVBO != typeModelBuffer[currentIDVBOV]) {
				if (model->typeVBO == GL_DYNAMIC_DRAW) {
					createVBOVertex(GL_DYNAMIC_DRAW, sizeVertexData, model->vertex[chunkI]);
				}
				else if (model->typeVBO == GL_STATIC_DRAW) {
					createVBOVertex();
				}
				else {
					printf("error UNKNOWN BUFFER TYPE \n");
				}
			}
		}
		else {
			// max VBO created 
		}


        
        
        
        
        uint currentSizeVertexVBO = sizeModelBuffer[currentIDVBOV] - offsetModelVertexBuffer[currentIDVBOV];
        
        if (sizeVertexData > currentSizeVertexVBO )  {
            
            if (currentIDVBOV>=createdNumVBO) {
                if (createdNumVBO < MAX_VBOS-1 ) {
                    printf("%d \n",createdNumVBO);
                    createVBOVertex();
                }else {
                    currentIDVBOV = 0; // max VBO created
                }
            }else {
                currentIDVBOV++;
            }
            offsetModelVertexBuffer[currentIDVBOV] = 0;
            
            if (sizeModelBuffer[currentIDVBOV] < sizeVertexData) {
                printf("EROR vertex vbo small \n");
                throw "EROR vertex vbo small";
            }
        }
        
        if (model->typeVBO == GL_STATIC_DRAW) {
            glBindBuffer(GL_ARRAY_BUFFER, modelVertexBuffers[currentIDVBOV]);
            glBufferSubData(GL_ARRAY_BUFFER, offsetModelVertexBuffer[currentIDVBOV],  sizeVertexData, model->vertex[chunkI]);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        
        std::vector<StoredData*>::iterator it = storedDatasV[currentIDVBOV].begin();
        while ( it != storedDatasV[currentIDVBOV].end() )
        {
            StoredData *data = *it;
            
            
            uint blockstart = data->startOffset;
            uint blockEnd   = data->startOffset+data->size;
            
            uint writedBlockStart = offsetModelVertexBuffer[currentIDVBOV];
            uint writedBlockEnded = offsetModelVertexBuffer[currentIDVBOV]+sizeVertexData;
            
          
            bool sss = false;
                
            if (blockstart <= writedBlockStart){
                sss = blockEnd >= writedBlockStart;
            }
            else {
                sss = blockstart <= writedBlockEnded;
            };
            
            if (sss) {
				removeChunk(data->mesh, data->indexChunk, &it, nullptr);
            }else {
                ++it;
            }
        }
        
        StoredData *datatostore = (StoredData *)malloc(sizeof(StoredData));
        datatostore->size = sizeVertexData;
        datatostore->startOffset = offsetModelVertexBuffer[currentIDVBOV];
        datatostore->mesh = model;
        datatostore->indexChunk = chunkI;


        storedDatasV[currentIDVBOV].push_back(datatostore);
        
        model->vertexBufferID[chunkI] = modelVertexBuffers[currentIDVBOV];
        model->calculateOffsets(offsetModelVertexBuffer[currentIDVBOV],chunkI);
        offsetModelVertexBuffer[currentIDVBOV]+=sizeVertexData+model->sizeVertex;
        

    }
    
    void MeshCache::loadIChunk(Mesh*model,uint chunkI){
        uint indexSize = 3 * sizeof(facesType);
        uint sizeIndexData = model->numFacesC[chunkI]*indexSize;
        
        
        uint currentSizeFacesVBO = maxSizeVBOI - offsetModelIndexBuffer[currentIDVBOI];
        if (sizeIndexData > currentSizeFacesVBO)  {
            
            if (currentIDVBOI >= createdNumIBO) {
                if (createdNumIBO < MAX_VBOS-1 ) {
                    createVBOIndex();
                }else {
                    currentIDVBOI = 0;
                }
            }else {
                currentIDVBOI++;
            }
            offsetModelIndexBuffer[currentIDVBOI] = 0;
            
            
            if (maxSizeVBOI < sizeIndexData) {
                printf("ERrOR index vbo small ");
                throw "ERrOR index vbo small";
            }
            
        }
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIndexBuffers[currentIDVBOI]);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offsetModelIndexBuffer[currentIDVBOI], sizeIndexData, model->faces[chunkI]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        
        
        
        
        
        std::vector<StoredData*>::iterator itI = storedDatasI[currentIDVBOI].begin();
        while ( itI != storedDatasI[currentIDVBOI].end() )
        {
            StoredData *data = *itI;
            
            
            
            uint blockstart = data->startOffset;
            uint blockEnd   = data->startOffset+data->size;
            
            uint writedBlockStart = offsetModelIndexBuffer[currentIDVBOI];
            uint writedBlockEnded = offsetModelIndexBuffer[currentIDVBOI]+sizeIndexData;
            
            
            bool sss = false;
            
            
            if (blockstart <= writedBlockStart){
                sss = blockEnd >= writedBlockStart;
            }
            else {
                sss = blockstart <= writedBlockEnded;
            };
            
            
            if (sss) {
				removeChunk(data->mesh, data->indexChunk, nullptr, &itI);
            }else {
                ++itI;
            }
        }
        
        StoredData* datatostoreI  = (StoredData *)malloc(sizeof(StoredData));
        datatostoreI->size = sizeIndexData;
        datatostoreI->startOffset = offsetModelIndexBuffer[currentIDVBOI];
        datatostoreI->mesh = model;
        datatostoreI->indexChunk = chunkI;
        storedDatasI[currentIDVBOI].push_back(datatostoreI);
        
        
        model->indexOffset[chunkI] = offsetModelIndexBuffer[currentIDVBOI];
        offsetModelIndexBuffer[currentIDVBOI]+=sizeIndexData+indexSize;
        model->indexBufferID[chunkI] = modelIndexBuffers[currentIDVBOI];
        
    }


	void MeshCache::fillGPU(Mesh*model) {

		if (model->isDamagedAll())
			model->retain(); // add link to GPU 

		for (unsigned int j = 0; j < model->numChunks; j++) {
			if (model->indexChunksVBODamaged[j]) {
				loadVChunk(model, j);
				model->indexChunksVBODamaged[j] = false;
			}


			if (model->indexChunksIBODamaged[j]) {
				loadIChunk(model, j);
				model->indexChunksIBODamaged[j] = false;
			}

			free(model->vertex[j]);
			free(model->faces[j]);
			model->faces[j] = NULL;
			model->vertex[j] = NULL;
		}
		model->isDamaged = false;
	}

    
}
