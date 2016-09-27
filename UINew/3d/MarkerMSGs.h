//
//  MarkerMSGs.h
//  UltraVision
//
//  Created by Easy Proger on 25.05.15.
//  Copyright (c) 2015 easy proger. All rights reserved.
//

#import <UIKit/UIKit.h>
#define SHOW_MSG_MOVE_DEVICE_ON_MARKER 1
#define SHOW_MSG_WRONG_DEVICE_POSITION 2

#define FRAME_MARKER_TAKED 0
#define FRAME_MARKER_AUTOFOCUSING 1
#define FRAME_MARKER_NEED_ZOOM_IN 2
#define FRAME_MARKER_NOT_ALL_MARKER_IN_FRAME 3
#define FRAME_MARKER_NOT_READY 4

@interface MarkerMSGs : UIView {
    IBOutlet UIImageView* moveDeviceBG;
    IBOutlet UIImageView* moveDeviceOK;
    
    IBOutlet UIImageView* markerMSGBG;
    IBOutlet UILabel*  markerMSGText;
    IBOutlet UIImageView* markerMSGOK;
    int lastMSGID;
}
-(void)inizialize;
-(void)showMsgID:(int)msgID indexMsg:(int)indexMsg ;
@end
