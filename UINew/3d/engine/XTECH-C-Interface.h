//
//  XTECH-C-Interface.h
//  UltraVision
//
//  Created by Easy Proger on 18.05.15.
//  Copyright (c) 2015 easy proger. All rights reserved.
//

#ifndef UltraVision_XTECH_C_Interface_h
#define UltraVision_XTECH_C_Interface_h

#include "XTypes.h"

void stopSoundFromCPP (std::string aParameter);
int playSoundFromCPP (std::string aParameter,bool loop);
TextureInfo* loadTextureCPP (char *aParameter);


#endif
