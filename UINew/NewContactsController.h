//
//  NewContactsController.h
//  UINew
//
//  Created by  Roman on 12.10.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NewContactsController : UIViewController <UITableViewDataSource, UITableViewDelegate>

@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *btnBack;
@property (weak, nonatomic) IBOutlet UIButton *btn3D;
@property (weak, nonatomic) IBOutlet UIButton *btnToMap;
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;

@property (weak, nonatomic) IBOutlet UIView *vwMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnRemind;
@property (weak, nonatomic) IBOutlet UIButton *btnMap;
@property (weak, nonatomic) IBOutlet UIButton *btnToMainMenu;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;

- (IBAction)btnMenuPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *lblContacts;
@property (nonatomic) NSArray* texts;
@property (nonatomic) NSArray* contactsDay;
@property (nonatomic) NSArray* contactsDesr;
@property (nonatomic) NSArray* contactsPos;
@property (nonatomic) NSArray* contactsTime;
@property (nonatomic) NSArray* contactsPhone;
@property (nonatomic) NSArray* contactsTitle;
- (IBAction)btnBackPressed:(id)sender;

@end
