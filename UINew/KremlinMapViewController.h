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
#import "MainController.h"

@interface KremlinMapViewController : UIViewController <YMKMapViewDelegate, UIGestureRecognizerDelegate,MainControllerDelegate>
{
    NSArray *gestureRecognizers;
}

- (IBAction)someAction:(id)sender;
@property (weak, nonatomic) IBOutlet YMKMapView *mapView;
@property (weak, nonatomic) IBOutlet UIButton *btnBack;
@property (weak, nonatomic) IBOutlet UIButton *btn3D;
@property (weak, nonatomic) IBOutlet UIButton *btnContacts;
@property (weak, nonatomic) IBOutlet UIButton *btnRemind;
@property (weak, nonatomic) IBOutlet UIButton *btnMainMenu;
@property (weak, nonatomic) IBOutlet UIView *vwMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;
- (IBAction)btnMenuClick:(id)sender;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;
@property (weak, nonatomic) IBOutlet UIButton *btnAllObjects;
- (IBAction)btnBackPressed:(id)sender;
@end
