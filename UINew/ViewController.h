//
//  ViewController.h
//  UINew
//
//  Created by  Roman on 28.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Loader.h"
#import "CacheControll.h"
#import "MainController.h"
#include "pugixml.hpp"

@interface ViewController : UIViewController <UITableViewDataSource, UITableViewDelegate,LoaderDelegate,UIGestureRecognizerDelegate,MainControllerDelegate>{
    UIProgressView*progressLoadedContent;
    UIView*backgroundProgress;
    NSString*serverPath;
    NSString*contentXMLPath;
    pugi::xml_document contentXMLObj;
}

@property (weak, nonatomic) IBOutlet UIButton *btnLang;
@property (weak, nonatomic) IBOutlet UIButton *btn3D;
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIView *vwMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnRemind;
@property (weak, nonatomic) IBOutlet UIButton *btnContacts;
@property (weak, nonatomic) IBOutlet UIButton *btnMap;
@property (weak, nonatomic) IBOutlet UIButton *btnDownload;
@property (nonatomic) NSArray* mainArray;
@property (nonatomic) NSArray* mainArrayDescr;
@property (nonatomic) BOOL refreshed;
@property (strong, nonatomic) IBOutlet UIGestureRecognizer *gestureRecognizer;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;


@end
