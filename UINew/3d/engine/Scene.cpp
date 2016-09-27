//
//  Scene.cpp
//  ShadowMapsTestProject
//
//  Created by easy on 25.11.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "Scene.h"
#include "SceneGraph.h"



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <GL/glew.h>
#else
#ifdef __ANDROID__

#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2ext.h>
#else
#include <OpenGLES/ES2/gl.h>
#endif
#endif



#include <math.h>





using namespace Xengine;
using namespace std;
SceneGraph*sceneGraph;
#define PI 3.1415926535897932384626433832795f
Scene::~Scene(){
    
}



Scene::Scene(int theWidth,int theHeight)
{
    
    
    
    
    width = theWidth;
    height = theHeight;
    
    sceneGraph = new SceneGraph();
    
}




void Scene::createList()
{
    sceneGraph->createListObjects();
}



void Scene::addChild(SceneNode *object)
{
    sceneGraph->addObjectinTree(object);
}