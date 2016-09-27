//
//  ContactsCellTableViewCell.h
//  UINew
//
//  Created by  Roman on 12.10.15.
//  Copyright © 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ContactsCellTableViewCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *cellTitle;
@property (weak, nonatomic) IBOutlet UILabel *cellPositionDesrc;
@property (weak, nonatomic) IBOutlet UILabel *cellDescr;
@property (weak, nonatomic) IBOutlet UIImageView *cellImage1;
@property (weak, nonatomic) IBOutlet UILabel *cellTime;
@property (weak, nonatomic) IBOutlet UILabel *cellDay;
@property (weak, nonatomic) IBOutlet UIImageView *cellPhone;
@property (weak, nonatomic) IBOutlet UILabel *cellNumber;

@end
