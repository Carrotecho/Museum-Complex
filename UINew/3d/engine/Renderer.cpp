//
//  Renderer.cpp
//  XTech
//
//  Created by easy proger on 07.02.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#include "Renderer.h"




Renderer::~Renderer() {
    delete shaderParamsSetter;
    delete shaderCache;
    delete depthFrameBuffer;
    delete depthTexture;
    
    if (pickFrameBuffer != nullptr){delete pickFrameBuffer;}
    if (pickTexture != nullptr) { delete pickTexture; }
    
    depthTexture = nullptr;
    pickFrameBuffer = nullptr;
    pickTexture = nullptr;
    shaderParamsSetter = nullptr;
    shaderCache = nullptr;
    depthFrameBuffer = nullptr;
}

void Renderer::clear() {
    
}


Renderer::Renderer(Camera*camera,std::string _resPath) {
    
    genericID = 0;
    resourcePath = _resPath;
    
    pickTextureWidth = 1024.0;
	pickTextureHeight = 1024.0;
	
	mDefaultFrameBuffer = 0;

	offscreen_framebuffer = 0;
	pickFrameBuffer = NULL;
	pickTexture = NULL;
    
    depthTexture = new Texture();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	depthTexture->createClearTexture(WIDTH_SHADOW_MAP, HEIGHT_SHADOW_MAP, GL_TEXTURE_2D, GL_RGBA, GL_RED, GL_FLOAT);
#else
    depthTexture->createClearTexture(WIDTH_SHADOW_MAP, HEIGHT_SHADOW_MAP, GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_HALF_FLOAT_OES);
#endif


    
    shaderCache = new ShaderCache(resourcePath);
	   
    depthFrameBuffer = new FrameBuffer();
    depthFrameBuffer->create(WIDTH_SHADOW_MAP, HEIGHT_SHADOW_MAP, BUFFER_FORMAT_DEPTH);
    depthFrameBuffer->attachTexture(depthTexture);
    
    shaderParamsSetter = new ShaderParamsSetter(camera,
                                                depthTexture->getTextureID(),
                                                0,
                                                0,
                                                0);
    
    
	action[FORWARD_RENDERER][SHADOW_DEPTH_TEHNICS] = &Renderer::renderToShadowMap;
    action[FORWARD_RENDERER][BACK_BUFFER_TEHNICS] = &Renderer::renderBackBufferFR;

    GET_GLERROR();
}

void Renderer::setDefaultBuffer(uint bufferID) {
    mDefaultFrameBuffer = bufferID;
}
void Renderer::setDefaultSceneGraph(SceneGraph* sceneGraph) {
    defaultSceneGraph = sceneGraph;
}





void Renderer::renderBackBufferPICKObject(XQuery* query) {
	pickFrameBuffer->bind();

	glViewport(0, 0, pickTextureWidth, pickTextureHeight);

	glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0.0);
    
	

    uint lastShaderID = 0;
	uint lastVBOBuffer = 0;
	uint lastIBOByffer = 0;

	glDisable(GL_BLEND);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if (defaultSceneGraph->listLights.size() > 0) {
        Light* defaultLight = defaultSceneGraph->listLights[0];
        shaderParamsSetter->setLight(defaultLight);
    }
    
    for (unsigned int i=0;i<query->size;i++)
    {
        SceneNode* node = query->list[i];
        Mesh* model = node->getMesh();

		if (model->isDamaged) model->isUsed = true;
		if (!node->isVisible || model->isDamaged) continue;

		if (node->zTest) {
			glEnable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_DEPTH_TEST);
		}

		nodesForPickObject.push_back(node);
		node->indexMesh = (float)(nodesForPickObject.size() / 255.0);
		renderObject(node, model, &lastShaderID, &lastVBOBuffer, &lastIBOByffer, PICK_OBJECT_TEHNICS);
    }
   
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	pickFrameBuffer->unbind();
}



void Renderer::renderBackBufferFR(XQuery* query) {
	if (mDefaultFrameBuffer)
    glBindFramebuffer(GL_FRAMEBUFFER, mDefaultFrameBuffer);
    
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    
    
	//glDepthMask(GL_TRUE);

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0,0.0,0.0,0.0);
    
	

    uint lastShaderID = 0;
	uint lastVBOBuffer = 0;
	uint lastIBOByffer = 0;
    
    if (defaultSceneGraph->listLights.size() > 0) {
        Light* defaultLight = defaultSceneGraph->listLights[0];
        shaderParamsSetter->setLight(defaultLight);
    }
    
    for (unsigned int i=0;i<query->size;i++)
    {
        SceneNode* node = query->list[i];
        Mesh* model = node->getMesh();

		if (model->isDamaged) model->isUsed = true;
		if (!node->isVisible || model->isDamaged) continue;

		if (node->blendFunction) {
			glEnable(GL_BLEND);
			glBlendFunc(node->blendFunctionIn, node->blendFunctionOut);
		} else {
			glDisable(GL_BLEND);
		}

		if (node->typeObject == PARTICLES_OBJECT) {
			glDepthMask(GL_FALSE);
		}

		renderObject(node, model, &lastShaderID, &lastVBOBuffer, &lastIBOByffer, BACK_BUFFER_TEHNICS);
    }
	
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Renderer::changeShadowRender(int type) {
    
}






void Renderer::nop(XQuery* query) {
    
}



void Renderer::renderObject(SceneNode*node,Mesh*model,uint *lastShaderID,uint *lastVBOBuffer,uint *lastIBOByffer,uint pass) {

	if (node->zTest) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
	

	
	


	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    
    
    

	Shader* shader = shaderCache->get(node->getMaterial(), model, pass);
	shaderParamsSetter->setRenderObject(node);

    
	if (*lastShaderID != shader->idProgram) {
		*lastShaderID = shader->idProgram;
		glUseProgram(*lastShaderID);
        
        
        
        
	}


	for (unsigned int u = 0; u < shader->uniformsLenght; u++) {
		(shaderParamsSetter->*shader->uniforms[u].action)(shader->uniforms[u].idUniform);
	}
	for (unsigned int a = 0; a < shader->attributesLenght; a++) {
		glEnableVertexAttribArray(shader->attributes[a].idAttribute);
	}


	for (unsigned int chunkI = 0; chunkI < model->numChunks; chunkI++) {

		if (model->vertexBufferID[chunkI] != *lastVBOBuffer) {
			glBindBuffer(GL_ARRAY_BUFFER, model->vertexBufferID[chunkI]);
			*lastVBOBuffer = model->vertexBufferID[chunkI];
		}
		if (model->indexBufferID[chunkI] != *lastIBOByffer) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indexBufferID[chunkI]);
			*lastIBOByffer = model->indexBufferID[chunkI];
		}

		for (unsigned int a = 0; a < shader->attributesLenght; a++) {
			glVertexAttribPointer(shader->attributes[a].idAttribute,
				shader->attributes[a].size,
				shader->attributes[a].type,
				shader->attributes[a].normalized,
				model->sizeVertex,
				(GLvoid*)model->offsets[chunkI][shader->attributes[a].idAttributeConstant]);
		}

		glDrawElements(node->customVertexData, model->numFacesC[chunkI] * model->numIndexPerData, model->typeIndexes, (GLvoid*)model->indexOffset[chunkI]);
        
        
        
	}
	for (unsigned int a = 0; a < shader->attributesLenght; a++) {
		glDisableVertexAttribArray(shader->attributes[a].idAttribute);
	}
}


void Renderer::renderToShadowMap(XQuery* query) {

    depthFrameBuffer->bind();
    glViewport(0, 0, WIDTH_SHADOW_MAP, HEIGHT_SHADOW_MAP);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    
    // XTODO: cull facing posible slower (( 
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 2.0);
    
    uint lastShaderID = 0;
	uint lastVBOBuffer = 0;
	uint lastIBOByffer = 0;
    
    Light* defaultLight = defaultSceneGraph->listLights[0];
    shaderParamsSetter->setLight(defaultLight);
	glDisable(GL_BLEND);
    for (unsigned int i=0;i<query->size;i++)
    {
        SceneNode* node = query->list[i];
        Mesh* model = node->getMesh();

		if (model->isDamaged) model->isUsed = true;
		if (!node->isVisible || model->isDamaged) continue;

		

		renderObject(node, model, &lastShaderID, &lastVBOBuffer, &lastIBOByffer, SHADOW_DEPTH_TEHNICS);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glDisable(GL_CULL_FACE);
    glDisable(GL_POLYGON_OFFSET_FILL);
    
    depthFrameBuffer->unbind();
 
}




BaseRenderObject* Renderer::pickObject(float coordinatsX,float coordinatsY) {
	nodesForPickObject.clear();
	if (pickFrameBuffer == nullptr && pickTexture == nullptr) {
		pickFrameBuffer = new FrameBuffer();
		pickTexture = new Texture();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		pickTexture->createClearTexture((float)pickTextureWidth, (float)pickTextureHeight, GL_TEXTURE_2D, GL_RGBA, GL_RED, GL_FLOAT);
#else
    	pickTexture->createClearTexture((float)pickTextureWidth, (float)pickTextureHeight, GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_HALF_FLOAT_OES);
#endif

		pickFrameBuffer->create((float)pickTextureWidth, (float)pickTextureHeight, BUFFER_FORMAT_DEPTH);
    	pickFrameBuffer->attachTexture(pickTexture);
	}

	SceneChildsController*ch = defaultSceneGraph->childController;
    XQuery*querys = ch->querys;
    for (unsigned int i = 0; i<ch->sizeQuerys; i++) {
        if (querys[i].queryID == BACK_BUFFER_TEHNICS) {
            renderBackBufferPICKObject(&querys[i]);
        }
    };
	pickFrameBuffer->bind();
	glViewport(0, 0, (int)pickTextureWidth, (int)pickTextureHeight);


    float pixel = -1;

	float findX = coordinatsX*pickTextureWidth;
	float findY = coordinatsY*pickTextureHeight;
	findY = pickTextureHeight-findY;
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	glReadPixels((int)findX, (int)findY, 1, 1, GL_RED, GL_FLOAT, &pixel);
    #else
    float pixel_tmp[4];
    glReadPixels((int)findX, (int)findY, 1, 1, GL_RGBA, GL_HALF_FLOAT_OES, &pixel_tmp);
    pixel = pixel_tmp[0];
    #endif

	pickFrameBuffer->unbind();

    // restore 
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	uint index = int(pixel*255.0);
	printf("picked object:%d \n",index);
	if (index!=0 && index-1<nodesForPickObject.size()) {
		return nodesForPickObject[index-1];
	}else {
		return nullptr;
	}
}

void Renderer::render() {
#if SHOW_AR
    glGetIntegerv(GL_VIEWPORT, viewport);
#endif
    
    SceneChildsController*ch = defaultSceneGraph->childController;
    XQuery*querys = ch->querys;
    for (unsigned int i = 0; i<ch->sizeQuerys; i++) {
        (this->*action[querys[i].renderID][querys[i].queryID])(&querys[i]);
    }
}