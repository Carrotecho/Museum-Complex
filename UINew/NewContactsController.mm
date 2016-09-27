//
//  NewContactsController.m
//  UINew
//
//  Created by  Roman on 12.10.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import "NewContactsController.h"
#import "MainController.h"
#import "ContactsCellTableViewCell.h"

@interface NewContactsController ()

@end

@implementation NewContactsController

-(void)UpdateButtons
{
    MainController *myController = [MainController sharedController];
    switch (myController.currentLang)
    {
        case 0:
            LocalizationSetLanguage(@"ru");
            break;
            
        case 1:
            LocalizationSetLanguage(@"en");
            break;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [[_btn3D imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btn3D setImage:[UIImage imageNamed:@"3D.png"] forState:UIControlStateNormal];
        
        [_lblContacts setText:AMLocalizedString(@"Contacts", @"")];
        
        [[_btnToMainMenu imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnToMainMenu setImage:[UIImage imageNamed:@"ToMainMenu.png"] forState:UIControlStateNormal];
        
        
        [[_btnMap imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnMap setImage:[UIImage imageNamed:@"Map"] forState:UIControlStateNormal ];
        [_btnMap  setTitle:AMLocalizedString(@"ToMap", @"") forState:UIControlStateNormal];
        
        [[_btnRemind imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnRemind setImage:[UIImage imageNamed:@"Remind"] forState:UIControlStateNormal ];
        [_btnRemind  setTitle:AMLocalizedString(@"Remind", @"") forState:UIControlStateNormal];
        
        [_btnToMainMenu setTitle:AMLocalizedString(@"MainMenu", @"") forState:UIControlStateNormal];
        [_btnBack setTitle:AMLocalizedString(@"Back", @"") forState:UIControlStateNormal];
        [_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        
        [[_btnMenu imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnMenu setImage:[UIImage imageNamed:@"MainMenuButton"] forState:UIControlStateNormal];
        
        [_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnToMainMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnBack setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
        [_btnToMap setTitle:AMLocalizedString(@"ToMap", @"") forState:UIControlStateNormal];
    });
}

-(void)viewWillDisappear:(BOOL)animated
{
    _vwMenu.hidden = YES;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    _tableView.delegate = self;
    _tableView.dataSource = self;
    
    _tableView.estimatedRowHeight = 175.0;
    _tableView.rowHeight = UITableViewAutomaticDimension;
    self.tableView.tableFooterView = [[UIView alloc] init];

    _texts = @[@"Contacts1", @"Contacts2", @"Contacts3"];
    
    _contactsDay = @[@"ContactsDay1", @"ContactsDay2", @"ContactsDay3", @"ContactsDay4"];
    _contactsDesr = @[@"ContactsDes1", @"ContactsDes2", @"ContactsDes3", @"ContactsDes4"];
    _contactsPhone = @[@"ContactsPhone1", @"ContactsPhone2", @"ContactsPhone3", @"ContactsPhone4"];
    _contactsPos = @[@"ContactsPos1", @"ContactsPos2", @"ContactsPos3", @"ContactsPos4"];
    _contactsTime = @[@"ContactsTime1", @"ContactsTime2", @"ContactsTime3", @"ContactsTime4"];
    _contactsTitle = @[@"ContactsTitle1", @"ContactsTitle2", @"ContactsTitle3", @"ContactsTitle4"];

    [self UpdateButtons];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 7;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ( _vwMenu.hidden == NO )
    {
        _vwMenu.hidden = YES;
        return;
    }
}


-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
   
    if (indexPath.row >= 0 && indexPath.row < 3)
    {
        UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"MyCell"];
        cell.textLabel.text = AMLocalizedString(_texts[ indexPath.row ], @"");
        return cell;
    }
    else if (indexPath.row >= 3 && indexPath.row < 7)
    {
        ContactsCellTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"ContactCell"];
        cell.cellDay.text = AMLocalizedString(_contactsDay[ indexPath.row - 3 ], @"");
        cell.cellDescr.text = AMLocalizedString(_contactsDesr[ indexPath.row - 3], @"");
        cell.cellPositionDesrc.text = AMLocalizedString(_contactsPos[ indexPath.row - 3], @"");
        cell.cellTime.text = AMLocalizedString(_contactsTime[ indexPath.row - 3], @"");
        cell.cellNumber.text = AMLocalizedString(_contactsPhone[ indexPath.row - 3], @"");
        [cell.cellImage1 setImage:[UIImage imageNamed:@"time"]];
        [cell.cellPhone setImage:[UIImage imageNamed:@"phone"]];
        cell.cellTitle.text = AMLocalizedString(_contactsTitle[ indexPath.row - 3], @"");
        return cell;
    }
    else
    {
        
        UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"MyCell"];
        return cell;
    }
  
}


- (IBAction)btnBackPressed:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnMenuPressed:(id)sender {
    NSLog(@"Pressed");
    _vwMenu.hidden = !_vwMenu.hidden;
}


@end
