//
//  Object.hpp
//  UltraVision
//
//  Created by Easy Proger on 01.08.15.
//  Copyright © 2015 easy proger. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>


enum CameraTypes {
	CAMERA_FREE
};


enum LightTypes {
	DIRECT_LIGHT
};

enum ShadowTypes {
	NO_SHADOW,
	ORTHO_SHADOW_MAPS
};


#define FileObjectTypes_MESH 0
#define FileObjectTypes_Particle 1

enum ObjectToolbarTypes {
	NO_OBJECT, 
	CAMERA_OBJECT, 
	// mesh objects
	MESH_OBJECT, 
	PARTICLES_OBJECT,
	LIGHT_OBJECT, 
    // manipulators controller
	MANIPULATE_AXIS_X, 
	MANIPULATE_AXIS_Y,
	MANIPULATE_AXIS_Z,

	MANIPULATE_AXIS_XY,
	MANIPULATE_AXIS_XZ,
	MANIPULATE_AXIS_YZ,

	MANIPULATE_AXIS_XYZ,
	// count types
	MAX_OBJECT_TYPES
};


class Object {
public:


    Object(){typeObject=ObjectToolbarTypes::NO_OBJECT;};
    ~Object(){};
	ObjectToolbarTypes typeObject;
};

#endif /* Object_hpp */
