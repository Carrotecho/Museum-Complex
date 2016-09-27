//
//  Renderer.h
//  XTech
//
//  Created by easy proger on 07.02.14.
//  Copyright (c) 2014 easy proger. All rights reserved.
//

#ifndef __Renderer__
#define __Renderer__

#include <iostream>

#include "SceneGraph.h"
#include "XObject.h"
#include "XRenderTypes.h"


#include "ShaderCache.h"

class Renderer:public XObject {
public:
    Renderer(Camera*camera,std::string _resPath);
    ~Renderer();
    
    uint genericID;
    
    ShaderCache*shaderCache;
    
    std::string resourcePath;
    
    
	BaseRenderObject* pickObject(float coordinatsX,float coordinatsY);
    void render();
    
    void setDefaultSceneGraph(SceneGraph* sceneGraph);
    void setDefaultBuffer(uint bufferID);
    
    void changeShadowRender(int type);
    
    void clear();
    
    
    void (Renderer::*action[MAX_RENDERER][MAX_TEHNICS])(XQuery* query);
private:
    Texture* depthTexture;
	float pickTextureWidth,pickTextureHeight;


	std::vector<SceneNode*> nodesForPickObject;
	GLuint  my_texture,
	   offscreen_framebuffer;


    Camera*_camera;
    FrameBuffer* depthFrameBuffer;
	Texture* pickTexture;
	FrameBuffer* pickFrameBuffer;
    ShaderParamsSetter* shaderParamsSetter;
    SceneGraph *defaultSceneGraph;
    uint mDefaultFrameBuffer; // default frame
    int viewport[4];
    
    void renderBackBufferPICKObject(XQuery* query);
    void renderBackBufferFR(XQuery* query);
	void renderObject(SceneNode * node, Mesh * model, uint * lastShaderID, uint * lastVBOBuffer, uint * lastIBOByffer, uint pass);
	void renderToShadowMap(XQuery* query);
    void nop(XQuery* query);
};

#endif 
