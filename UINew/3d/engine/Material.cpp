//
//  Material.cpp
//  ShadowMapsTestProject
//
//  Created by Easy.proger on 14.04.11.
//  Copyright 2011 Immortal Engine. All rights reserved.
//

#include "Material.h"
#include "Texture.h"

Material::Material(bool useBaseTexture) {
    for (int i = 0; i < MAX_MATERIALS; i++) {
        flags[i] = false;
    }

    color.x = 0.0;
	color.y = 1.0;
    color.z = 0.0;

    specularPower = 0.1;
    
	flags[MATERIAL_OBJECT_POSITION] = true;
    flags[MATERIAL_PROJECT] = true;
	diffuseTexture = nullptr;
    normalMapTexture = nullptr;
    specularTexture = nullptr;

	updateUDIDMaterial();

    shader_id = 0;
    if (useBaseTexture) {
        // XTODO : create texture Cache !!!! and get cached base texture here !!! 
        
    }
}

float Material::getSpecularPower() {
    return specularPower;
}
void Material::setSpecularPower(float val) {
    specularPower = val;
    updateUDIDMaterial();
}

bool Material::getFlagVal(uint flagID) {
	if (flagID >= MAX_MATERIALS) return false;
	return flags[flagID];
}

void Material::setFlag(uint flagID,bool val) {
	if (flagID >= MAX_MATERIALS) return;
	flags[flagID] = val;
	updateUDIDMaterial();
}


void Material::updateUDIDMaterial() {
	matFlagsString = "";
	for (int i = 0; i < MAX_MATERIALS; i++) {
		if (flags[i]) {
			char buf[256];
			sprintf(buf, "%d", i);
			matFlagsString += buf;
		}
	}
    
    char buf[256];
    sprintf(buf, "%f", specularPower);
    matFlagsString += buf;
}

bool Material::validate() {
    bool matValid = true;
    if (this->diffuseTexture) {
        matValid = matValid && !this->diffuseTexture->isDamaged();
    }
    if (this->specularTexture) {
        matValid = matValid && !this->specularTexture->isDamaged();
    }
    if (this->normalMapTexture) {
        matValid = matValid && !this->normalMapTexture->isDamaged();
    }
    return matValid;
}

void Material::reloadTextures() {
    if (this->diffuseTexture && this->diffuseTexture->isDamaged()) {
        this->diffuseTexture->reloadTexture();
    }
    if (this->specularTexture && this->specularTexture->isDamaged()) {
        this->specularTexture->reloadTexture();
    }
    if (this->normalMapTexture && this->normalMapTexture->isDamaged()) {
        this->normalMapTexture->reloadTexture();
    }
}

