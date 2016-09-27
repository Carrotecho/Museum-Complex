//
//  CustomDowloadCell.h
//  dwldDemo
//
//  Created by  Roman on 03.12.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CustomDowloadCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UIButton *btnAction;
@property (weak, nonatomic) IBOutlet UILabel *lblAction;
@property (strong, nonatomic) IBOutlet UILabel *lblStatus;


@end
