//
//  MapViewController.m
//  UINew
//
//  Created by  Roman on 28.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "MapViewControllerClose.h"
#import "MainController.h"
#import "LocalizationSystem.h"

@interface MapViewControllerClose ()
@property (nonatomic, strong) PointAnnotation  *annotation;
@end

@implementation MapViewControllerClose

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
         NSLog(@"excursion mode: %@", myController.excursionMode );
     if ([myController.excursionMode isEqualToString:@"Main"])
     {
         [_btnMulitguide setTitle:AMLocalizedString(@"Multiguide", @"") forState:UIControlStateNormal];
     }
     else if ([myController.excursionMode isEqualToString:@"Short"])
     {
         [_btnMulitguide setTitle:AMLocalizedString(@"ToMap", @"") forState:UIControlStateNormal];
     }
         
    [[_btn3D imageView] setContentMode: UIViewContentModeScaleAspectFit];
    [_btn3D setImage:[UIImage imageNamed:@"3D.png"] forState:UIControlStateNormal];
    
    [[_btnContacts imageView] setContentMode: UIViewContentModeScaleAspectFit];
    [_btnContacts setImage:[UIImage imageNamed:@"Contacts.png"] forState:UIControlStateNormal];
    
    [[_btnRemind imageView] setContentMode: UIViewContentModeScaleAspectFit];
    [_btnRemind setImage:[UIImage imageNamed:@"Remind.png"] forState:UIControlStateNormal];
    
    [[_btnMainMenu imageView] setContentMode: UIViewContentModeScaleAspectFit];
    [_btnMainMenu setImage:[UIImage imageNamed:@"ToMainMenu.png"] forState:UIControlStateNormal];
    
    [_btnContacts setTitle:AMLocalizedString(@"Contacts", @"") forState:UIControlStateNormal];
    [_btnRemind setTitle:AMLocalizedString(@"Remind", @"") forState:UIControlStateNormal];
    [_btnMainMenu setTitle:AMLocalizedString(@"MainMenu", @"") forState:UIControlStateNormal];
         
    [[_btnMenu imageView] setContentMode:UIViewContentModeScaleAspectFit];
    [_btnMenu setImage:[UIImage imageNamed:@"MainMenuButton"] forState:UIControlStateNormal];
         
    [_btnBack setTitle:AMLocalizedString(@"Back", @"") forState:UIControlStateNormal];
         
    [_lblHowToFind setText:AMLocalizedString(@"Directions", @"")];
    NSRange match = [myController.details rangeOfString:@"\n"];
         NSString *str1, *str2;
        
    if(match.location != NSNotFound)
         {
             str1 = [myController.details substringWithRange: NSMakeRange (0, match.location + 1)];
             str2 = [myController.details substringWithRange: NSMakeRange (match.location+match.length,(myController.details.length-match.location)-match.length)];
             UIFont *verdanaBold = [UIFont fontWithName:@"verdana-Bold" size:14.0 ];
             NSDictionary *verdanaBoldDict = [NSDictionary dictionaryWithObject: verdanaBold forKey:NSFontAttributeName];
             
             NSMutableAttributedString *vbaAttrString = [[NSMutableAttributedString alloc] initWithString:str1 attributes: verdanaBoldDict];
             
             UIFont *VerdanaFont = [UIFont fontWithName:@"verdana" size:14.0];
             NSDictionary *verdanaDict = [NSDictionary dictionaryWithObject:VerdanaFont forKey:NSFontAttributeName];
             NSMutableAttributedString *vAttrString = [[NSMutableAttributedString alloc]initWithString: str2 attributes:verdanaDict];
             //[vAttrString addAttribute:NSForegroundColorAttributeName value:[UIColor blackColor] range:(NSMakeRange(0, 15))];
             [vbaAttrString appendAttributedString: vAttrString];
             
             //_lblDirections.attributedText = vbaAttrString;
             _tvDetails.attributedText = vbaAttrString;
         }

         

    [_btnBack setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
    [_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnMulitguide setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnContacts setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnMainMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnRemind setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_lblNovgorod setText:AMLocalizedString(@"Novgorod", @"")];
    [_lblStateMuseum setText:AMLocalizedString(@"StateMuseum", @"")];
     });
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:@"MapViewController" bundle:nibBundleOrNil];
    if (self) {
        
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self UpdateButtons];
    [self configureMapView];
    [self configureAndInstallAnnotations];
}
-(void)viewWillDisappear:(BOOL)animated
{
    _vwMenu.hidden = YES;
}


#pragma mark - YMKMapViewDelegate

- (YMKAnnotationView *)mapView:(YMKMapView *)aMapView viewForAnnotation:(id<YMKAnnotation>)anAnnotation
{
    static NSString * identifier = @"pointAnnotation";
    YMKPinAnnotationView * view = (YMKPinAnnotationView *)[aMapView dequeueReusableAnnotationViewWithIdentifier:identifier];
    if (view == nil)
    {
        view = [[YMKPinAnnotationView alloc] initWithAnnotation:anAnnotation
                                                reuseIdentifier:identifier];
    }
    
    [self configureAnnotationView:view forAnnotation:anAnnotation];
    return view;
}


-(void)configureAndInstallAnnotations
{

    MainController *myController = [MainController sharedController];
    PointAnnotation *annotation = [PointAnnotation pointAnnotation];
    
    annotation.coordinate = YMKMapCoordinateMake(myController.mapCoords.dx, myController.mapCoords.dy);
    [self.mapView addAnnotation:annotation];
}

- (void)configureAnnotationView:(YMKPinAnnotationView *)aView forAnnotation:(id<YMKAnnotation>)anAnnotation
{
    PointAnnotation *pointAn =  (PointAnnotation*)_annotation;
    if (anAnnotation == pointAn)
    {
        aView.selectedImage = nil;
    }
}



- (void)viewDidUnload {
    self.mapView = nil;
    
    [super viewDidUnload];
}

#pragma mark - Helpers
-(void)configureAndInstallMapView
{
    self.mapView.delegate = self;
}

-(void)configureMapView
{
    MainController *myController = [MainController sharedController];
    
    [self configureAndInstallMapView];
    self.mapView.showsUserLocation = YES;
    self.mapView.showTraffic = NO;
    [self.mapView setCenterCoordinate:YMKMapCoordinateMake(myController.mapCoords.dx, myController.mapCoords.dy)
                          atZoomLevel:15
                             animated:NO];
}

#pragma mark - Memory Management

- (void)dealloc
{
    self.mapView.delegate = nil;
}


- (IBAction)someAction:(id)sender {
    NSLog( @"tap");
}

- (IBAction)btnMainMenuPressed:(id)sender {
    _vwMenu.hidden = !_vwMenu.hidden;
}
- (IBAction)btnBackPressed:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)btnMultiguidePressed:(id)sender {
    MainController *myController = [MainController sharedController];
    
     if ([myController.excursionMode isEqualToString:@"Main"])
     {
         [self performSegueWithIdentifier:@"pushToMultiguideFromMap" sender:self];
     }
    else if ([myController.excursionMode isEqualToString:@"Short"])
    {
        [self performSegueWithIdentifier:@"pushToGlobalMapFromMap" sender:self];
    }
    else
    {
        NSLog(@"Error: how did you get up there?");
    }
}
@end
