//
//  MarkerMSGs.m
//  UltraVision
//
//  Created by Easy Proger on 25.05.15.
//  Copyright (c) 2015 easy proger. All rights reserved.
//

#import "MarkerMSGs.h"

@implementation MarkerMSGs

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

-(void)inizialize {
    lastMSGID = -1;
    moveDeviceBG.hidden  = 0;
    moveDeviceOK.hidden  = 0;
    markerMSGBG.hidden   = 0;
    markerMSGText.hidden = 0;
    markerMSGOK.hidden   = 0;

    moveDeviceBG.alpha  = 0;
    moveDeviceOK.alpha  = 0;
    markerMSGBG.alpha   = 0;
    markerMSGText.alpha = 0;
    markerMSGOK.alpha   = 0;
}


-(void)wrongDevicePosition:(int)indexID {
    switch (indexID) {
        case FRAME_MARKER_AUTOFOCUSING:
            [markerMSGText setText:@"Камера стабилизирует фокус пожалуйста подождите"];
            break;
        case FRAME_MARKER_NEED_ZOOM_IN:
            [markerMSGText setText:@"Приблизте устройство к маркеру"];
            break;
        case FRAME_MARKER_NOT_ALL_MARKER_IN_FRAME:
            [markerMSGText setText:@"Не весь маркер попадает в камеру, "];
            break;
        case FRAME_MARKER_NOT_READY:
            [markerMSGText setText:@"FRAME_MARKER_NOT_READY"];
            break;
        default:
            break;
    }
}


-(void)showMsgID:(int)msgID indexMsg:(int)indexMsg {
    if (msgID == SHOW_MSG_WRONG_DEVICE_POSITION) {
        [self wrongDevicePosition:indexMsg];
    }
    
    if (lastMSGID == msgID) {
        return;
    }
    
    
    //Disappear
    [UIView animateWithDuration:0.3
     
    animations:^(void) {
        switch (lastMSGID) {
            case SHOW_MSG_MOVE_DEVICE_ON_MARKER:
                moveDeviceBG.alpha = 0;
                moveDeviceOK.alpha = 0;
                break;
            case SHOW_MSG_WRONG_DEVICE_POSITION:
                markerMSGBG.alpha   = 0;
                markerMSGText.alpha = 0;
                markerMSGOK.alpha   = 0;
                break;
            default:
                break;
        }
        
        switch (msgID) {
            case SHOW_MSG_MOVE_DEVICE_ON_MARKER:
                moveDeviceBG.alpha = 1;
                moveDeviceOK.alpha = 1;
                break;
            case SHOW_MSG_WRONG_DEVICE_POSITION:
                markerMSGBG.alpha   = 1;
                markerMSGText.alpha = 1;
                markerMSGOK.alpha   = 1;
                break;
            default:
                break;
        }
    }
    completion:^(BOOL finished){
        switch (lastMSGID) {
            case SHOW_MSG_MOVE_DEVICE_ON_MARKER:
                moveDeviceBG.alpha = 0;
                moveDeviceOK.alpha = 0;
                break;
            case SHOW_MSG_WRONG_DEVICE_POSITION:
                markerMSGBG.alpha   = 0;
                markerMSGText.alpha = 0;
                markerMSGOK.alpha   = 0;
                break;
            default:
                break;
        }
        
        switch (msgID) {
            case SHOW_MSG_MOVE_DEVICE_ON_MARKER:
                //moveDeviceBG.alpha = 1;
                //moveDeviceOK.alpha = 1;
                break;
            case SHOW_MSG_WRONG_DEVICE_POSITION:
                markerMSGBG.alpha   = 1;
                markerMSGText.alpha = 1;
                markerMSGOK.alpha   = 1;
                break;
            default:
                break;
        }
    }];
    
    
    
    lastMSGID = msgID;
}

@end
