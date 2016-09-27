//
//  ViewController4.h
//  UINew
//
//  Created by  Roman on 27.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MainController.h"

@interface ViewController4 : UIViewController <UIScrollViewDelegate, UIGestureRecognizerDelegate,MainControllerDelegate>

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UIView *contentView;
@property (strong, nonatomic) IBOutlet UIButton *button;
- (void)onBtnTap:(UIButton*)sender;
@property (weak, nonatomic) IBOutlet UIImageView *imageView;
-(CGRect) InsertButtonAtX:(CGFloat)x andY:(CGFloat)y withWidth:(CGFloat)w andHeight:(CGFloat)h;
@property (weak, nonatomic) IBOutlet UIButton *btn3D;
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;
-(void)UpdateMap;
@property (weak, nonatomic) IBOutlet UIView *vwMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnRemind;
@property (weak, nonatomic) IBOutlet UIButton *btnMainMenu;
@property (weak, nonatomic) IBOutlet UILabel *lblRouteDescr;
@property (weak, nonatomic) IBOutlet UIButton *btnContacts;
@property (weak, nonatomic) IBOutlet UIButton *btnMap;
@property (weak, nonatomic) IBOutlet UIButton *btnAllObjects;
@property (weak, nonatomic) IBOutlet UIButton *btnBack;
- (IBAction)btnBackPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;
@property (weak, nonatomic) IBOutlet UIView *vwButtonZone;
@property (nonatomic) BOOL imageChanged;
@property (strong, nonatomic)  NSMutableArray *buttons;
- (IBAction)btnAllObjectsPressed:(id)sender;
@property BOOL btnTapped;
-(void)DrawButtons;
@property (weak, nonatomic) IBOutlet UILabel *lblObjectTitle;

@end
