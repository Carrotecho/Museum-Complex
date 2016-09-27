//
//  ContactsViewController.m
//  UINew
//
//  Created by  Roman on 08.10.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import "ContactsViewController.h"
#import "MainController.h"

@interface ContactsViewController ()

@end


@implementation ContactsViewController

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
        
        [_lblExtra setText:AMLocalizedString(@"RemindExtra", @"")];
        [_lblWelcome setText:AMLocalizedString(@"Welcome", @"")];
        [_lblRemind setText:AMLocalizedString(@"Remind", @"")];
        
        [[_btnMenu imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnMenu setImage:[UIImage imageNamed:@"MainMenuButton"] forState:UIControlStateNormal];
        
        [[_btnToMainMenu imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnToMainMenu setImage:[UIImage imageNamed:@"ToMainMenu.png"] forState:UIControlStateNormal];
        [[_btnContacts imageView] setContentMode: UIViewContentModeScaleAspectFit];
        
        [[_btnMap imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnMap setImage:[UIImage imageNamed:@"Map"] forState:UIControlStateNormal ];
        [_btnMap  setTitle:AMLocalizedString(@"ToMap", @"") forState:UIControlStateNormal];
        
        [[_btnContacts imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnContacts setImage:[UIImage imageNamed:@"Contacts"] forState:UIControlStateNormal ];
        [_btnContacts  setTitle:AMLocalizedString(@"Contacts", @"") forState:UIControlStateNormal];
        
        [_btnToMainMenu setTitle:AMLocalizedString(@"MainMenu", @"") forState:UIControlStateNormal];
        [_btnBack setTitle:AMLocalizedString(@"Back", @"") forState:UIControlStateNormal];
        [_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnToMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnToTheMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnBack setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
        [_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnToMainMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        
         [_btnToMap setTitle:AMLocalizedString(@"ToMap", @"") forState:UIControlStateNormal];
        [_btnToTheMap setTitle:AMLocalizedString(@"ToMap", @"") forState:UIControlStateNormal];
    });
}

- (IBAction)btnMenuPressed:(id)sender {
    _vwMenu.hidden = !_vwMenu.hidden;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    _images = @[@"", @"menu", @"langEng", @"3d-1", @"", @"share", @"multiguide", @"howToGet", @"", @"mapBtn",@"", @"takeAPic", @"", @"" ];
    _texts = @[@"remind1", @"remind2_1", @"remind2_2", @"remind2_3", @"remind3_1", @"remind3_2", @"remind3_4", @"remind3_5", @"remind4_1", @"remind4_2", @"remind5_1", @"remind5_2", @"remind5_3", @"remind5_4"];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.estimatedRowHeight = 175.0;
    _tableView.rowHeight = UITableViewAutomaticDimension;
    [self UpdateButtons];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewWillAppear:(BOOL)animated
{
   _vwMenu.hidden = YES;
}

-(void)viewDidAppear:(BOOL)animated
{
    
    [_tableView reloadData];
}

-(void)viewWillDisappear:(BOOL)animated
{
    _vwMenu.hidden = YES;
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (IBAction)btnBackPressed:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnToMapPressed:(id)sender {
}

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 5;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    switch (section) {
        case 0:
            return 1;
            break;
        case 1:
            return 3;
            break;
        case 2:
            return 5;
            break;
        case 3:
            return 2;
            break;
        case 4:
            return 4;
            break;
        default:
            return  0;
    }
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ( _vwMenu.hidden == NO )
    {
        _vwMenu.hidden = YES;
        return;
    }

    
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    NSArray *title = @[@"Remind_1", @"Remind_2", @"Remind_3", @"Remind_4", @"Remind_5"];
    NSString *resString = AMLocalizedString(title[section],  @"");
    return resString;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"MyCell"];
    
    
    switch (indexPath.section) {
        case 0:
            cell.textLabel.text = AMLocalizedString(_texts[ 0 ], @"");//1
             cell.imageView.image = [UIImage imageNamed:_images[ indexPath.row ]];
            break;
        case 1:
            cell.textLabel.text = AMLocalizedString(_texts[ 1 + indexPath.row ], @"");//3
            cell.imageView.image = [UIImage imageNamed:_images[ 1 + indexPath.row ]];
            break;
        case 2:            cell.textLabel.text = AMLocalizedString(_texts [ 4 + indexPath.row ], @"");//5
            cell.imageView.image = [UIImage imageNamed:_images[ 4 + indexPath.row ]];
            break;
        case 3:
            cell.textLabel.text = AMLocalizedString(_texts [ 8 + indexPath.row ], @"");//2
            cell.imageView.image = [UIImage imageNamed:_images[ 8 + indexPath.row  ]];
            break;
        case 4:
            cell.textLabel.text = AMLocalizedString(_texts [ 10 + indexPath.row ], @"");//2
            cell.imageView.image = [UIImage imageNamed:_images[ 10 + indexPath.row ]];
            if ( indexPath.row == 3 )
            {
                cell.textLabel.textAlignment = NSTextAlignmentCenter;
            }
            break;

        default:
            break;
    }
    
    return cell;
}


-(void) tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath{
 //   [[cell textLabel] setFont:[UIFont systemFontOfSize:12.0]];
}


@end
