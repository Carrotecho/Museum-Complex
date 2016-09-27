//
//  ViewController5.h
//  UINew
//
//  Created by  Roman on 30.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MainController.h"

@interface ViewController5 : UIViewController <UITableViewDataSource, UITableViewDelegate,UIGestureRecognizerDelegate,MainControllerDelegate>
@property (strong, nonatomic) IBOutlet UIGestureRecognizer *gestureRecognizer;
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *btnBack;
@property (weak, nonatomic) IBOutlet UIButton *btnInfo;
- (IBAction)btnInfoPressed:(id)sender;

@property (weak, nonatomic) IBOutlet UIButton *btnRemind;
@property (weak, nonatomic) IBOutlet UIButton *btn3D;
@property (weak, nonatomic) IBOutlet UIButton *btnMainMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnContacts;
@property (weak, nonatomic) IBOutlet UIButton *btnHowTo;
@property (weak, nonatomic) IBOutlet UIButton *btnMap;

@property (weak, nonatomic) IBOutlet UIView *vwMenu;
- (IBAction)btnMenuPressed:(UIButton *)sender;
- (IBAction)btnBackPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;
@property (nonatomic) NSMutableArray *objects;
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;



@end
