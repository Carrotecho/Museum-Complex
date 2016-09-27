//
//  ViewController3.h
//  UINew
//
//  Created by  Roman on 26.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MainController.h"

@interface ViewController3 : UIViewController <UIScrollViewDelegate,MainControllerDelegate>
@property (strong, nonatomic) IBOutlet UIImageView *imageView;
@property (strong, nonatomic) UIImage *secondImage;
@property (weak, nonatomic) IBOutlet UILabel *descr;
@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UIView *contentView;
@property (weak, nonatomic) IBOutlet UIButton *btnBack;
@property (strong) NSMutableArray *images;
@property  NSInteger activeIndex;
//-(void)loadEnd;
-(void)GetAllImages;
- (IBAction)onTap:(id)sender;

@end
