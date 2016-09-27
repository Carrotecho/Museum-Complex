//
//  MapViewController.m
//  UINew
//
//  Created by  Roman on 28.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "KremlinMapViewController.h"
#import "LocalizationSystem.h"



@interface KremlinMapViewController ()
@property (nonatomic, strong) NSArray  *annotations;

@end

@implementation KremlinMapViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:@"MapViewController" bundle:nibBundleOrNil];
    if (self) {
        
    }
    return self;
}
-(void)UpdateButtons
{
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [[_btn3D imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btn3D setImage:[UIImage imageNamed:@"3D.png"] forState:UIControlStateNormal];
        
        [[_btnContacts imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnContacts setImage:[UIImage imageNamed:@"Contacts.png"] forState:UIControlStateNormal];
        
        [[_btnRemind imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnRemind setImage:[UIImage imageNamed:@"Remind.png"] forState:UIControlStateNormal];
        
        [[_btnMainMenu imageView] setContentMode: UIViewContentModeScaleAspectFit];
        [_btnMainMenu setImage:[UIImage imageNamed:@"ToMainMenu.png"] forState:UIControlStateNormal];
        [[_btnMenu imageView] setContentMode: UIViewContentModeScaleAspectFit];
         [_btnMenu setImage:[UIImage imageNamed:@"MainMenuSmall"] forState:UIControlStateNormal];
        [_btnContacts setTitle:AMLocalizedString(@"Contacts", @"") forState:UIControlStateNormal];
        [_btnRemind setTitle:AMLocalizedString(@"Remind", @"") forState:UIControlStateNormal];
        [_btnMainMenu setTitle:AMLocalizedString(@"MainMenu", @"") forState:UIControlStateNormal];
        [_btnAllObjects setTitle:AMLocalizedString(@"AllObjects", @"") forState:UIControlStateNormal];
        [_btnBack setTitle:AMLocalizedString(@"Back", @"") forState:UIControlStateNormal];
        [_btnBack setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
        [_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnContacts setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnMainMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnRemind setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnAllObjects setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_lblNovgorod setText:AMLocalizedString(@"Novgorod", @"")];
        [_lblStateMuseum setText:AMLocalizedString(@"StateMuseum", @"")];
    });
}


- (void)viewDidLoad {
    [super viewDidLoad];
    gestureRecognizers = [NSArray array];
    
    MainController *myController = [MainController sharedController];
    myController.prevObjectId = nil;
    myController.objectId = @"999";
    
    myController.currentFloor = 0;
    
    [myController ObjectIdUpdated:nil];
    
    for (int i = 0; i <myController.objectList.count; i++)
    {
        gestureRecognizers = [gestureRecognizers arrayByAddingObject:[[UITapGestureRecognizer alloc] initWithTarget:self
                                                                                                            action:@selector(handleSingleTap:)]];
    }
    
    for ( int i = 0; i < myController.objectList.count; i++)
    {
        [ (UITapGestureRecognizer*)[gestureRecognizers objectAtIndex:i] setNumberOfTouchesRequired:1];
        [ (UITapGestureRecognizer*)[gestureRecognizers objectAtIndex:i] setDelegate:self];
    }
    
    switch (myController.currentLang)
    {
        case 0:
            LocalizationSetLanguage(@"ru");
            break;
        case 1:
            LocalizationSetLanguage(@"en");
            break;
    }

    
    [self UpdateButtons];
    [self configureMapView];
    [self configureAndInstallAnnotations];
}

-(void)handleSingleTap:(UITapGestureRecognizer *)sender
{
	NSLog(@"%ld", sender.view.tag);
	MainController *myController = [MainController sharedController];
	myController.prevObjectId = myController.objectId;

    Exhibit* exhibit = myController.objectList[sender.view.tag];

	myController.objectId = exhibit->Id;//excursion type should be Short

    [myController ObjectIdUpdated:self ];
	
}


-(void)loadEnd {
    [self performSegueWithIdentifier:@"pushToInfoFromKremlinMap" sender:self];
}

#pragma mark - YMKMapViewDelegate

-(YMKAnnotationView *)mapView:(YMKMapView *)aMapView viewForAnnotation:(id<YMKAnnotation>)anAnnotation
{
	static NSString * identifier = @"pointAnnotation";
	YMKPinAnnotationView * view = (YMKPinAnnotationView *)[aMapView dequeueReusableAnnotationViewWithIdentifier:identifier];
	if (view == nil)
		view = [[YMKPinAnnotationView alloc] initWithAnnotation:anAnnotation reuseIdentifier:identifier];

	[self configureAnnotationView:view forAnnotation:anAnnotation];
	return view;
}


-(void)configureAndInstallAnnotations
{
    _annotations = [NSArray array];
     MainController *myController = [MainController sharedController];
    for (int i = 0; i <myController.objectList.count; i++ )
    {
        PointAnnotation *annotation = [PointAnnotation pointAnnotation];
        Exhibit *exh = (Exhibit*)myController.objectList[ i ];
        
        annotation.coordinate = YMKMapCoordinateMake(exh->coord.dx, exh->coord.dy);
        
        _annotations = [_annotations arrayByAddingObject:annotation];
        [self.mapView addAnnotation:_annotations[i]];
    }
}

- (void)configureAnnotationView:(YMKPinAnnotationView *)aView forAnnotation:(id<YMKAnnotation>)anAnnotation
{
    MainController *myController = [MainController sharedController];
    NSLog(@"myController: %@",myController);
    for (int i = 0; i <_annotations.count; i++ )
    {
        PointAnnotation *pointAn =  (PointAnnotation*)_annotations[i];
        if (anAnnotation == pointAn)
        {
            Exhibit *exh = (Exhibit*)myController.objectList[ i ];
            aView.image = [UIImage imageWithContentsOfFile:exh->icon];
            aView.selectedImage = nil;
            aView.tag = i;
            [ aView addGestureRecognizer:(UITapGestureRecognizer*)[gestureRecognizers objectAtIndex:i] ];
        }
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
    [self configureAndInstallMapView];
    self.mapView.showsUserLocation = YES;
    self.mapView.showTraffic = NO;
    [self.mapView setCenterCoordinate:YMKMapCoordinateMake(58.521264, 31.275871)
                          atZoomLevel:16
                             animated:NO];
}

#pragma mark - Memory Management

- (void)dealloc
{
    self.mapView.delegate = nil;
}

- (IBAction)btnMenuClick:(id)sender {
    _vwMenu.hidden = !_vwMenu.hidden;
}

- (IBAction)someAction:(id)sender {
    
    NSLog( @"tap");
}
- (IBAction)btnBackPressed:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}
@end
