//
//  MapViewController.m
//  UINew
//
//  Created by  Roman on 28.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "MapViewController.h"

#import "LocalizationSystem.h"


@interface MapViewController ()
@property (nonatomic, strong) NSArray  *annotations;


@end

@implementation MapViewController

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
	[[_btn3D imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btn3D setImage:[UIImage imageNamed:@"3D.png"] forState:UIControlStateNormal];

	[[_btnContacts imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btnContacts setImage:[UIImage imageNamed:@"Contacts.png"] forState:UIControlStateNormal];
	
	[[_btnRemind imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btnRemind setImage:[UIImage imageNamed:@"Remind.png"] forState:UIControlStateNormal];
	
	[[_btnMainMenu imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btnMainMenu setImage:[UIImage imageNamed:@"ToMainMenu.png"] forState:UIControlStateNormal];
        
    [[_btnMenu imageView] setContentMode:UIViewContentModeScaleAspectFit];
    [_btnMenu setImage:[UIImage imageNamed:@"MainMenuButton"] forState:UIControlStateNormal];

	[_btnContacts setTitle:AMLocalizedString(@"Contacts", @"") forState:UIControlStateNormal];
	[_btnRemind setTitle:AMLocalizedString(@"Remind", @"") forState:UIControlStateNormal];
	[_btnMainMenu setTitle:AMLocalizedString(@"MainMenu", @"") forState:UIControlStateNormal];
	[_btnAllObjects setTitle:AMLocalizedString(@"AllObjects", @"") forState:UIControlStateNormal];
    [_btnBack setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
	[_btnBack setTitle:AMLocalizedString(@"Back", @"") forState:UIControlStateNormal];
	[_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnAllObjects setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnContacts setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnMainMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnRemind setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	});
}

-(id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
	self = [super initWithNibName:@"MapViewController" bundle:nibBundleOrNil];

	return self;
}

-(void)viewDidLoad {
	[super viewDidLoad];
    NSLog(@"Map view load");
    MainController *myController = [MainController sharedController];
    _tmpID = myController.objectId;
    myController.objectId = @"1000";
    _tmpFloor = myController.currentFloor;
    myController.currentFloor = 0;
    myController.tempIndex = 1;
    if (![_tmpID isEqualToString:[myController.objectStack lastObject]])
    {
                [myController PushToStack:_tmpID];
    }
    [myController ObjectIdUpdated:self];
    
    gestureRecognizers = [NSArray array];
    for (int i = 0; i <myController.objectList.count; i++)
    {
        gestureRecognizers = [gestureRecognizers arrayByAddingObject:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleSingleTap:)]];
    }
    
    for ( int i = 0; i < myController.objectList.count; i++)
    {
        [(UITapGestureRecognizer*)[gestureRecognizers objectAtIndex:i] setNumberOfTouchesRequired:1];
        [(UITapGestureRecognizer*)[gestureRecognizers objectAtIndex:i] setDelegate:self];
    }
    
    [self UpdateButtons];
    
    [self configureMapView];
    [self configureAndInstallAnnotations];
    
}

-(void)viewWillAppear:(BOOL)animated {
    MainController *myController = [MainController sharedController];
    
   if ( ![myController.objectId isEqualToString:@"1000"] )
   {
       myController.objectId = @"1000";
       [myController ObjectIdUpdated:nil];
   }

}


-(void) viewWillDisappear:(BOOL)animated
{
    _vwMenu.hidden = YES;
}

-(void)handleSingleTap:(UITapGestureRecognizer *)sender
{
	NSLog(@" %ld ", sender.view.tag);
    
    MainController *myController = [MainController sharedController];
    myController.prevObjectId = myController.objectId;
    
    Exhibit* exhibit = myController.objectList[sender.view.tag];
    myController.objectId = exhibit->Id;//excursion type should be Short
    myController.tempIndex = 0;
    [myController PushToStack:myController.objectId];
    [myController ObjectIdUpdated:self ];
	
}

-(void)loadEnd {
    MainController *myController = [MainController sharedController];
    if (myController.tempIndex == 0) {
        [self performSegueWithIdentifier:@"pushToInfoFromGlobalMap" sender:self];
    }else if (myController.tempIndex == 1) {
        // need refresh
        MainController *myController = [MainController sharedController];
        
        gestureRecognizers = [NSArray array];
        for (int i = 0; i <myController.objectList.count; i++)
        {
            gestureRecognizers = [gestureRecognizers arrayByAddingObject:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleSingleTap:)]];
        }
        
        for ( int i = 0; i < myController.objectList.count; i++)
        {
            [(UITapGestureRecognizer*)[gestureRecognizers objectAtIndex:i] setNumberOfTouchesRequired:1];
            [(UITapGestureRecognizer*)[gestureRecognizers objectAtIndex:i] setDelegate:self];
        }
        
        [self UpdateButtons];
        
        [self configureMapView];
        [self configureAndInstallAnnotations];
        [_mapView setNeedsDisplay];
        NSLog (@"refreshed");
        
    }
    
}


#pragma mark - YMKMapViewDelegate

-(YMKAnnotationView *)mapView:(YMKMapView *)aMapView viewForAnnotation:(id<YMKAnnotation>)anAnnotation
{
	static NSString * identifier = @"pointAnnotation";
	YMKPinAnnotationView * view = (YMKPinAnnotationView *)[aMapView dequeueReusableAnnotationViewWithIdentifier:identifier];
	if (view == nil)
	{
		view = [[YMKPinAnnotationView alloc] initWithAnnotation:anAnnotation reuseIdentifier:identifier];
	}
	
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
    [self.mapView setCenterCoordinate:YMKMapCoordinateMake(58.529868, 31.314633)
                          atZoomLevel:11
                             animated:NO];
}

#pragma mark - Memory Management

- (void)dealloc
{
    self.mapView.delegate = nil;
}


- (IBAction)btnBackPressed:(id)sender {
      MainController *myController = [MainController sharedController];
    
    myController.objectId = [myController.objectStack lastObject];
    [myController.objectStack removeLastObject];
    myController.currentFloor = _tmpFloor;
    [myController ObjectIdUpdated:nil];
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)someAction:(id)sender {
    
    NSLog( @"tap");
}
- (IBAction)btnMenuClick:(id)sender {
    _vwMenu.hidden = !_vwMenu.hidden;
}
@end
