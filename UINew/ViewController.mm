//
//  ViewController.m
//  UINew
//
//  Created by  Roman on 23.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "ViewController.h"
#import "CustomCell.h"
#import "LocalizationSystem.h"



@implementation ViewController

-(void)UpdateButtons
{
	MainController *myController = [MainController sharedController];
	_btnLang.adjustsImageWhenHighlighted = NO;
	[[_btnLang imageView] setContentMode: UIViewContentModeScaleAspectFit];
	switch (myController.currentLang)
	{
		case 0:
			LocalizationSetLanguage(@"ru");
			[_btnLang setImage:[UIImage imageNamed:@"eng.png"] forState:(UIControlStateNormal)];
		break;

		case 1:
			LocalizationSetLanguage(@"en");
			[_btnLang setImage:[UIImage imageNamed:@"rus.png"] forState:(UIControlStateNormal)];
		break;
	}
	_btnLang.adjustsImageWhenHighlighted = NO;
	[[_btn3D imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btn3D setImage:[UIImage imageNamed:@"3D.png"] forState:UIControlStateNormal];

	[[_btnMap imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btnMap setImage:[UIImage imageNamed:@"Map.png"] forState:UIControlStateNormal];
   
    [[_btnDownload imageView] setContentMode: UIViewContentModeScaleAspectFit];
    [_btnDownload setImage:[UIImage imageNamed:@"toDownloadScreen"] forState:UIControlStateNormal];
   
	[[_btnContacts imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btnContacts setImage:[UIImage imageNamed:@"Contacts.png"] forState:UIControlStateNormal];
	
	[[_btnRemind imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btnRemind setImage:[UIImage imageNamed:@"Remind.png"] forState:UIControlStateNormal];
    
	[_btnDownload setTitle:AMLocalizedString(@"DownloadContent", @"") forState:UIControlStateNormal];
	[_btnContacts setTitle:AMLocalizedString(@"Contacts", @"") forState:UIControlStateNormal];
	[_btnRemind setTitle:AMLocalizedString(@"Remind", @"") forState:UIControlStateNormal];
	[_btnMap setTitle:AMLocalizedString(@"Map", @"") forState:UIControlStateNormal];
	[_lblNovgorod setText:AMLocalizedString(@"Novgorod", @"")];
	[_lblStateMuseum setText:AMLocalizedString(@"StateMuseum", @"")];
	[_btnLang setBackgroundImage:[UIImage imageNamed:@"backgroundButton.png"]  forState:UIControlStateNormal];

	[_btnLang setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnContacts setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnRemind setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnDownload setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
}

-(void)viewDidLoad {
    [super viewDidLoad];
    NSLog(@"MainMenu Loaded");
    serverPath = @"http://f.devar.org/";
    contentXMLPath = [serverPath stringByAppendingString:@"content.xml"];
    NSString*dataSetXML =[serverPath stringByAppendingString:@"bases/novgorod/test_novgorod.xml"];
    NSString*dataSetDAT =[serverPath stringByAppendingString:@"bases/novgorod/test_novgorod.dat"];
    
    [CacheControll sharedInstance].isInternetActive = [self avalibleDownloadFiles];
    
    _tableView.scrollEnabled = false;
    
    // Do any additional setup after loading the view, typically from a nib.
    _mainArray = [[NSArray alloc] initWithObjects:@"GranovitayaPalata.png", @"ZdaniePrisutMest.png", @"KremlinMap.png", @"CerkovNaIline.png",@"CerkovNaNeredice.png", @"CerkovNaKovaleve.png", @"CerkovVKoshevnikah.png", nil];
    _mainArrayDescr = [[NSArray alloc] initWithObjects:@"GranovitayaPalata", @"ZdaniePrisutMest", @"KremlinMap", @"CerkovNaIline",@"CerkovNaNeredice", @"CerkovNaKovaleve", @"CerkovVKoshevnikah", nil];
    
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _gestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTap:)];
    _gestureRecognizer.delegate= self;
    [self.view addGestureRecognizer:_gestureRecognizer];
    _tableView.estimatedRowHeight = 55.0;
    _tableView.rowHeight = UITableViewAutomaticDimension;
    
    
    [self UpdateButtons];
    
    UIView* view = self.view;
    
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat screenWidth = screenRect.size.width;
    CGFloat screenHeight = screenRect.size.height;
    
    CGRect frameBack = CGRectMake(0,0,screenWidth , screenHeight);
    
    backgroundProgress = [[UIView alloc] initWithFrame:frameBack];
    backgroundProgress.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.3];
    [view addSubview:backgroundProgress];
    
    progressLoadedContent = [[UIProgressView alloc] initWithProgressViewStyle:UIProgressViewStyleDefault];
    progressLoadedContent.progress = 0;
    
    
    CGRect frame = CGRectMake(screenWidth/2 - 100, screenHeight/2-25, 200, 50);
    
    [progressLoadedContent setFrame:frame];
    [view addSubview:progressLoadedContent];
    
    Loader*loader = [[Loader alloc] init];
    loader.delegate = self;
    [loader load:@[contentXMLPath,dataSetXML,dataSetDAT]];
    
}

-(void)createXMLcontent {
    
    NSData* dataXML = [CacheControll getDataFromCacheAtPath:contentXMLPath];
    NSString*textXML = [[NSString alloc] initWithData:dataXML encoding:NSUTF8StringEncoding];
    
    MainController*controller = [MainController sharedController];
    controller.mainNavController = self.navigationController;
    controller.serverPath = serverPath;
    [controller createXMlContent:textXML];
    
}


-(bool)avalibleDownloadFiles {
    
    NSArray *subviews = [[[[UIApplication sharedApplication] valueForKey:@"statusBar"] valueForKey:@"foregroundView"]subviews];
    NSNumber *dataNetworkItemView = nil;
    
    for (id subview in subviews) {
        if([subview isKindOfClass:[NSClassFromString(@"UIStatusBarDataNetworkItemView") class]]) {
            dataNetworkItemView = subview;
            break;
        }
    }
    
    switch ([[dataNetworkItemView valueForKey:@"dataNetworkType"]integerValue]) {
        case 0:
            return false;
            break;
        case 1:
            return true;
            break;
        case 2:
            return true;
            break;
        case 3:
            return true;
            break;
        case 4:
            return true;
            break;
        case 5:
            return true;
            break;
        default:
            break;
    }
    return false;
}



-(bool)taskWithError:(NSURL *)errorurl {
    return false;
}



-(void)updateLoaderDelegate:(double)progress sender:(id)sender {
    if (progress >= 100.0) {
        _tableView.scrollEnabled = true;
        [backgroundProgress removeFromSuperview];
        [progressLoadedContent removeFromSuperview];
        [self createXMLcontent];
        MainController *myController = [MainController sharedController];
        [myController retrieve3Dlinks];
        [myController retrieveMarkerId];
        
    }else {
        float progressF = progress/100.0;
        [progressLoadedContent setProgress:progressF];
    }
}


-(void)viewWillAppear:(BOOL)animated
{
	_refreshed = false;
	MainController *myController = [MainController sharedController];
    if (myController.objectStack.count > 0)
    {
        [myController.objectStack removeAllObjects];
    }
	[_tableView layoutIfNeeded];
}

-(void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning];
	// Dispose of any resources that can be recreated.
}
-(void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
}

-(void)viewWillDisappear:(BOOL)animated
{
	_refreshed = false;
	_vwMenu.hidden = YES;
}

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 1;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return [_mainArray count];
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
	static NSString *CellIdentifier = @"Cell";
	CustomCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];

	cell.label.text = AMLocalizedString(_mainArrayDescr[indexPath.row], @"");
	cell.image.contentMode = UIViewContentModeScaleAspectFit;
	[cell.image setImage:[UIImage imageNamed:_mainArray[indexPath.row]]];
	return cell;
}

-(IBAction)btnMenuPressed:(UIButton *)sender
{
	_vwMenu.hidden = !_vwMenu.hidden;
}

- (IBAction)backgroundTap:(id)sender
{
    if ( _vwMenu.hidden == NO )
    {
        _vwMenu.hidden = YES;
    }
}

-(IBAction)btnLangTap:(UIButton *)sender
{
    if ( _vwMenu.hidden == NO )
    {
        _vwMenu.hidden = YES;
        return;
    }
    MainController *myController = [MainController sharedController];
    if ( myController.currentLang == 0 )
    {
        LocalizationSetLanguage(@"en");
        myController.currentLang = 1;
    }
    else if ( myController.currentLang == 1 )
    {
        LocalizationSetLanguage(@"ru");
        myController.currentLang = 0;
    }
	
	[myController.socials UpdateLanguage];
    
    [self UpdateButtons];
    [_tableView reloadData];
    [self createXMLcontent];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ( _vwMenu.hidden == NO )
    {
		_vwMenu.hidden = YES;
		return;
	}
	MainController *myController = [ MainController sharedController];
	myController.excursionMode = @"Main";
	
	switch (indexPath.row)
	{
		case 0:
			myController.objectId = @"151";
		break;
			
		case 1:
			myController.objectId = @"152";
		break;
			
		case 2:
			myController.objectId = @"999";
		break;
			
		case 3:
			myController.objectId = @"1";
		break;
			
		case 4:
			myController.objectId = @"32";
		break;
			
		case 5:
			myController.objectId = @"93";
		break;
			
		case 6:
			myController.objectId = @"61_0";
		break;
	}
    
        if ([myController.objectId isEqualToString:@"151"])
        {
            myController.currentFloor = 1;
        }
        else
        {
            myController.currentFloor = 0;
        }
    
  //  myController.prevObjectId = myController.objectId;
    [myController PushToStack:myController.objectId];
    [myController ObjectIdUpdated:self];
    
    /*
	if ( indexPath.row != 2)
		[self performSegueWithIdentifier:@"segue1" sender:self];
	else
		[self performSegueWithIdentifier:@"pushKremlinMap" sender:self];
     */
}


-(void)loadEnd {
    // here event - all pic preloaded ! need refresh table view
    // assept go to next page )
    
    MainController *myController = [ MainController sharedController];
    
    if ([myController.objectId isEqualToString:@"999"]) {
        [self performSegueWithIdentifier:@"pushKremlinMap" sender:self];
    }else {
        [self performSegueWithIdentifier:@"segue1" sender:self];
    }
    
    
}


-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
	return ![NSStringFromClass([touch.view class]) isEqualToString:@"UITableViewCellContentView"];
}

-(void)handleTap:(UITapGestureRecognizer *)sender
{
	if (sender.state == UIGestureRecognizerStateEnded)
		NSLog( @"tap!");
}

-(void) tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
	if (!_refreshed)
	{
		if (indexPath.row == 6)
		{
			[_tableView reloadData];
			_refreshed = true;
		}
	}
}

@end
