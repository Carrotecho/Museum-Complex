//
//  MapViewControllerClose.h
//  UINew
//
//  Created by  Roman on 02.10.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "YandexMapKit.h"
#import "PointAnnotation.h"

@interface MapViewControllerClose : UIViewController <YMKMapViewDelegate, UIGestureRecognizerDelegate>
{
}

- (IBAction)someAction:(id)sender;
@property (weak, nonatomic) IBOutlet YMKMapView *mapView;
@property (weak, nonatomic) IBOutlet UIButton *btnBack;
@property (weak, nonatomic) IBOutlet UIButton *btnMulitguide;
@property (weak, nonatomic) IBOutlet UIButton *btn3D;
@property (weak, nonatomic) IBOutlet UILabel *lblHowToFind;
@property (weak, nonatomic) IBOutlet UILabel *lblDirections;
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;
- (IBAction)btnMainMenuPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UIView *vwMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnContacts;
@property (weak, nonatomic) IBOutlet UIButton *btnRemind;
@property (weak, nonatomic) IBOutlet UIButton *btnMainMenu;
- (IBAction)btnBackPressed:(id)sender;
@property (weak, nonatomic) IBOutlet UITextView *tvDetails;
- (IBAction)btnMultiguidePressed:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;
@end
