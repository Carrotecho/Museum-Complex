//
//  DownloadViewController.h
//  Novgorod Museum Complex
//
//  Created by  Roman on 07.12.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DownloadableObject.h"

@interface DownloadViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, DownloadableObjectDelegate>
@property (retain, strong) IBOutlet UITableView *table;

@property (weak, nonatomic) IBOutlet UIButton *btnBack;
- (IBAction)btnBackPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;


@end
