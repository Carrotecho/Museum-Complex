//
//  AppDelegate.h
//  UINew
//
//  Created by  Roman on 23.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "Loader.h"
#import "CacheControll.h"
@interface AppDelegate : UIResponder <UIApplicationDelegate> {
    UIProgressView*progressLoadedContent;
}

@property (strong, nonatomic) UIWindow *window;


@end

