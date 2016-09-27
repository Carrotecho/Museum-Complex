//
//  ContactsViewController.h
//  UINew
//
//  Created by  Roman on 08.10.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ContactsViewController : UIViewController <UITableViewDataSource, UITableViewDelegate>

@property (weak, nonatomic) IBOutlet UIButton *btnBack;
@property (weak, nonatomic) IBOutlet UIButton *btnMap;
@property (weak, nonatomic) IBOutlet UIButton *btn3D;
- (IBAction)btnBackPressed:(id)sender;
- (IBAction)btnToMapPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UITableView *tableView;
@property (weak, nonatomic) IBOutlet UIButton *btnToMainMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnToMap;
@property NSArray *images;
@property NSArray *texts;
-(void)UpdateButtons;
- (IBAction)btnMenuPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UIView *vwMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnContacts;
@property (weak, nonatomic) IBOutlet UIButton *btnToTheMap;

@property (weak, nonatomic) IBOutlet UILabel *lblExtra;
@property (weak, nonatomic) IBOutlet UILabel *lblWelcome;
@property (weak, nonatomic) IBOutlet UILabel *lblRemind;

@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;

@end
