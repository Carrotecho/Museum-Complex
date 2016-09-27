//
//  DownloadViewController.m
//  Novgorod Museum Complex
//
//  Created by  Roman on 07.12.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import "DownloadViewController.h"
#import "CustomDowloadCell.h"
#import "MainController.h"

@interface DownloadViewController ()

@end

@implementation DownloadViewController

-(void)UpdateButtons
{
    
    [_btnBack setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
    [_btnBack setTitle:AMLocalizedString(@"Back",@"") forState:UIControlStateNormal];
    [_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    
    [_lblNovgorod setText:AMLocalizedString(@"Novgorod", @"")];
    [_lblStateMuseum setText:AMLocalizedString(@"StateMuseum", @"")];
}

-(void)update:(id)sender
{
    MainController *myController = [MainController sharedController];
    NSLog(@"update call");
    DownloadableObject *obj = (DownloadableObject*)sender;
   // [_table reloadData];
    if (_table.hidden == NO)
    for (int i = 0; i < [myController.downloadObjects count]; i++)
    {
        if ((DownloadableObject*)myController.downloadObjects[i] == obj)
        {
            
            [_table reloadSections:[NSIndexSet indexSetWithIndex:i] withRowAnimation:UITableViewRowAnimationNone];

           // NSLog(@"%@",self.table);
            break;
        }
    }
    
}
-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:NO];
    _table.hidden = NO;
    _table.delegate = self;
    _table.dataSource = self;
    [_table reloadData];
}
- (void)viewDidLoad {
    [super viewDidLoad];
    NSLog(@"loaded");
    // Do any additional setup after loading the view.
    MainController *myController = [MainController sharedController];
    for (int i = 0; i < [myController.downloadObjects count]; i++)
    {
       // ((DownloadableObject*)[myController.downloadObjects objectAtIndex:i]).table = _table;
        ((DownloadableObject*)[myController.downloadObjects objectAtIndex:i]).senderForCallbackLoad = self;
       // ((DownloadableObject*)[myController.downloadObjects objectAtIndex:i]).section = i;
    }
/*
    NSArray *arr = [[NSArray alloc] initWithObjects:@"151",@"152", @"999", @"1", @"32", @"93", @"61_0",nil ];
    for (int i =0; i < [arr count]; i++)
    {
        [((DownloadableObject*)[myController.downloadObjects objectAtIndex:i]) RefreshWithData:[myController getAllResourcesForID:arr[i]]];
    }
  */  
    _table.delegate = self;
    _table.dataSource = self;
    [_table reloadData];
    
    [self UpdateButtons];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSLog(@"drawing a cell");
    CustomDowloadCell* cell = [tableView dequeueReusableCellWithIdentifier:@"cell"];
    
    MainController *myController = [MainController sharedController];
    cell.btnAction.tag = indexPath.section;
    [cell.btnAction addTarget:self action:@selector(btnActionPressed:) forControlEvents:UIControlEventTouchUpInside];
    DownloadableObject *obj = myController.downloadObjects[ indexPath.section ];
    [[cell.btnAction imageView] setContentMode:UIViewContentModeScaleAspectFit];
    switch (obj.status)
    {
        case STOPPED:
        case NOT_DOWNLOADED:
            cell.lblAction.text = AMLocalizedString(@"Download", @"");
            cell.lblStatus.text = [NSString stringWithFormat:@"%.2f / %.2f Mb", obj.mbytesDowloaded, obj.size];
            [cell.btnAction  setImage:[UIImage imageNamed:@"download"] forState:UIControlStateNormal];
            break;
            
        case DOWNLOADING:
            cell.lblAction.text = AMLocalizedString(@"Downloading", @"");
            cell.lblStatus.text = [NSString stringWithFormat:@"%.2f / %.2f Mb", obj.mbytesDowloaded, obj.size];
            [cell.btnAction  setImage:[UIImage imageNamed:@"stop"] forState:UIControlStateNormal];
            break;
            
        case DOWNLOADED:
            cell.lblAction.text = AMLocalizedString(@"Downloaded", @"");
            cell.lblStatus.text = AMLocalizedString(@"DownloadSucess", @"");
            [cell.btnAction  setImage:[UIImage imageNamed:@"downloaded"] forState:UIControlStateNormal];
            break;
        default:
            break;
    }
    return cell;
}

-(void)loadEnd:(id)sender
{
    DownloadableObject *obj = (DownloadableObject*)sender;
    obj.status = DOWNLOADED;
    MainController *myController = [MainController sharedController];

    for (int i = 0; i < [myController.downloadObjects count]; i++)
    {
        if ((DownloadableObject*)myController.downloadObjects [i] == obj)
        {
            [_table reloadSections:[NSIndexSet indexSetWithIndex:i] withRowAnimation:UITableViewRowAnimationNone];
            break;
        }
    }
}

-(void) btnActionPressed:(id)sender
{
    NSLog(@"click");
    UIButton* button = (UIButton*)sender;
    MainController *myController = [MainController sharedController];
    DownloadableObject* obj = myController.downloadObjects[button.tag];
    
    switch (obj.status)
    {
        case STOPPED:
        case NOT_DOWNLOADED:
            ((DownloadableObject*)[myController.downloadObjects objectAtIndex:button.tag]).status = DOWNLOADING;
            [((DownloadableObject*)[myController.downloadObjects objectAtIndex:button.tag]) StartDownloadWithSender:self];
            [_table reloadSections:[NSIndexSet indexSetWithIndex:button.tag] withRowAnimation:UITableViewRowAnimationNone];
        break;
            
        case DOWNLOADING:
            obj.status = STOPPED;
            [obj CancelDownload];
            [_table reloadSections:[NSIndexSet indexSetWithIndex:button.tag] withRowAnimation:UITableViewRowAnimationNone];

            break;
            
        default:
            break;
    }
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    MainController *myController = [MainController sharedController];
    DownloadableObject* obj = myController.downloadObjects[section];
    
    return AMLocalizedString(obj.name, @"");
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    MainController *myController = [MainController sharedController];
    return myController.downloadObjects.count;
}
/*
-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    
    // calculate height of UILabel
    DownloadableObject* obj =  _dowloadObjects[section];
    NSLog(@"height method called %@", obj.name);
    UILabel *lblSectionName = [[UILabel alloc] init];
    lblSectionName.text = obj.name;
    //lblSectionName.textColor = [UIColor lightGrayColor];
    lblSectionName.numberOfLines = 0;
    lblSectionName.lineBreakMode = NSLineBreakByWordWrapping;
    lblSectionName.backgroundColor = [UIColor grayColor];
    
    [lblSectionName sizeToFit];
    
    return lblSectionName.frame.size.height;
}
*/
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
-(void)viewWillDisappear:(BOOL)animated
{
    _table.hidden = YES;
    
}
- (IBAction)btnBackPressed:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}
@end
