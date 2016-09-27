//
//  ViewController2.h
//  UINew
//
//  Created by  Roman on 24.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "MainController.h"

@interface ViewController2 : UIViewController <UITableViewDelegate, UITableViewDataSource, UIGestureRecognizerDelegate, AVAudioPlayerDelegate,MainControllerDelegate>
@property (strong, nonatomic) IBOutlet UIGestureRecognizer *gestureRecognizer;
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *btnMultiguide;
@property (weak, nonatomic) IBOutlet UIButton *btnRemind;
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnToMainMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnContacts;
@property (weak, nonatomic) IBOutlet UIButton *btnHowTo;
@property (weak, nonatomic) IBOutlet UIButton *btn3D;
@property (weak, nonatomic) IBOutlet UIButton *btnMap;
@property (weak, nonatomic) IBOutlet UIButton *btnShare;
@property (weak, nonatomic) IBOutlet UIButton *btnInstagram;
@property (weak, nonatomic) IBOutlet UIButton *btnVK;
@property (weak, nonatomic) IBOutlet UIButton *btnFB;
@property (weak, nonatomic) IBOutlet UIButton *btnTwitter;
@property (weak, nonatomic) IBOutlet UIView *vwSocials;
- (IBAction)btnSharePressed:(id)sender;


@property (nonatomic) BOOL refreshed;
@property (strong, nonatomic) IBOutlet AVAudioPlayer *player;
- (IBAction)extraButton2Tap:(id)sender;
@property (weak, nonatomic) IBOutlet UIView *vwMenu;
- (IBAction)btnMenuPressed:(UIButton *)sender;
- (IBAction)backgroundTap:(id)sender;
- (IBAction)btnTap:(id)sender;
- (IBAction)ExtraButtonPressed:(id)sender;
- (IBAction)btnInstagramPressed:(id)sender;
- (IBAction)btnVKPressed:(id)sender;
- (IBAction)btnFBPressed:(id)sender;
- (IBAction)btnTwitterPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;

@end
