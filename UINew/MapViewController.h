//
//  MapViewController.h
//  UINew
//
//  Created by  Roman on 28.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "YandexMapKit.h"
#import "YMKMapLayerInfo.h"
#import "PointAnnotation.h"

#import "CacheControll.h"
#import "MainController.h"

@interface MapViewController : UIViewController <YMKMapViewDelegate, UIGestureRecognizerDelegate,MainControllerDelegate>
{
    NSArray *gestureRecognizers;
}
- (IBAction)btnBackPressed:(id)sender;




@property (weak, nonatomic) IBOutlet YMKMapView *mapView;
@property (weak, nonatomic) IBOutlet UIButton *btnBack;
@property (weak, nonatomic) IBOutlet UIButton *btn3D;
@property (weak, nonatomic) IBOutlet UIButton *btnContacts;
@property (weak, nonatomic) IBOutlet UIButton *btnRemind;
@property (weak, nonatomic) IBOutlet UIButton *btnMainMenu;
@property (weak, nonatomic) IBOutlet UIView *vwMenu;
@property (weak, nonatomic) IBOutlet UIButton *btnMenu;
- (IBAction)btnMenuClick:(id)sender;
@property (weak, nonatomic) IBOutlet UIButton *btnAllObjects;
@property (weak, nonatomic) IBOutlet UILabel *lblNovgorod;
@property (weak, nonatomic) IBOutlet UILabel *lblStateMuseum;
@property (strong, nonatomic) NSString *tmpID;
@property (nonatomic) BOOL shown;
@property (nonatomic) NSInteger tmpFloor;
@end
