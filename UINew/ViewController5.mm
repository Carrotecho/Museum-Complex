//
//  ViewController5.m
//  UINew
//
//  Created by  Roman on 30.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "ViewController5.h"
#import "LocalizationSystem.h"



@interface ViewController5 ()

@end

@implementation ViewController5


- (void)viewDidLoad
{
	[super viewDidLoad];
	
	// Do any additional setup after loading the view.
	_tableView.tag = 1;
	_tableView.dataSource = self;
	_tableView.delegate = self;
	_gestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTap:)];
	_gestureRecognizer.delegate = self;
	_tableView.rowHeight = UITableViewAutomaticDimension;
	[self UpdateButtons];
}

-(void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
	// Dispose of any resources that can be recreated.
}

-(void)viewWillAppear:(BOOL)animated
{
	NSLog(@"Will appear called");
    

MainController *myController = [MainController sharedController];
    _vwMenu.hidden = YES;
	_objects = [[NSMutableArray alloc] init];
	for ( int i = 0; i < myController.objectList.count; i++ )
	{
		Exhibit *exhibit =(Exhibit*)[myController.objectList objectAtIndex:i];
		if ( exhibit->floor == myController.currentFloor )
			[_objects addObject:exhibit];
	}
}

-(void)UpdateButtons
{
	dispatch_async(dispatch_get_main_queue(), ^{

	[[_btn3D imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btn3D setImage:[UIImage imageNamed:@"3D.png"] forState:UIControlStateNormal];

	[[_btnMap imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btnMap setImage:[UIImage imageNamed:@"Map.png"] forState:UIControlStateNormal];

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
	[_btnMap setTitle:AMLocalizedString(@"Map", @"") forState:UIControlStateNormal];
	[_btnMainMenu setTitle:AMLocalizedString(@"MainMenu", @"") forState:UIControlStateNormal];
	[_btnBack setTitle:AMLocalizedString(@"Back", @"") forState:UIControlStateNormal];
	[_btnInfo setTitle:AMLocalizedString(@"Information", @"") forState:UIControlStateNormal];

	[_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnContacts setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnMainMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnRemind setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_lblNovgorod setText:AMLocalizedString(@"Novgorod", @"")];
	[_lblStateMuseum setText:AMLocalizedString(@"StateMuseum", @"")];
        
    [_btnBack setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
     MainController *myController = [MainController sharedController];   
    [_btn3D setEnabled: [myController Determine3DStatus]];
	});
}

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	// Return the number of sections.
	return 1;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	return _objects.count;
}

-(void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
}

-(IBAction)btnMenuPressed:(UIButton *)sender
{
	_vwMenu.hidden = !_vwMenu.hidden;
}

-(void)viewWillDisappear:(BOOL)animated
{
    _vwMenu.hidden = YES;
}

-(IBAction)btnBackPressed:(id)sender {
    MainController *myController = [MainController sharedController];

    myController.prevObjectId = myController.objectId;
    if ( myController.hasNestedObjects && (myController.objectStack.count > 1) )
    {
        [myController.objectStack removeLastObject];
        myController.objectId = [myController.objectStack lastObject];
        
    }
  //  [myController.objectStack removeLastObject];
    else
    {
        myController.objectId = [myController.objectStack lastObject];
        if (myController.objectStack.count > 1 )
            [myController.objectStack removeLastObject];
    }
    [myController ObjectIdUpdated:nil];
	[self.navigationController popViewControllerAnimated:YES];
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
	if ( _vwMenu.hidden == NO )
	{
		_vwMenu.hidden = YES;
		return;
	}
	MainController *myController = [MainController sharedController];
	
	myController.prevObjectId = myController.objectId;
	Exhibit *exhibit = (Exhibit*)_objects[indexPath.row];
	myController.objectId = exhibit->Id;
    
    [myController ObjectIdUpdated:self];
	
}


-(void)loadEnd {
    MainController *myController = [MainController sharedController];
    if ( ( myController.hasNestedObjects == YES ) && !( ([myController.objectId isEqualToString:@"1000"]) || ([myController.objectId isEqualToString:@"999"]) ) )
    {
        NSLog(@"push to list");
        [myController PushToStack:myController.objectId];
        UIViewController *vc = [self.storyboard instantiateViewControllerWithIdentifier:@"ViewController5"];
        [self.navigationController pushViewController:vc animated:YES];
        //[self performSegueWithIdentifier:@"pushToList" sender:self ];
    }
    else if ( ([myController.objectId isEqualToString:@"1000"]) || ([myController.objectId isEqualToString:@"999"]))
    {
        [self performSegueWithIdentifier:@"pushToMainMenu" sender:self];
    }
    else
    {
        [myController PushToStack:myController.objectId];
        [self performSegueWithIdentifier:@"pushToInfoFromList" sender:self];
    }
}


-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
   
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cell"];
	Exhibit *exhibit =_objects[indexPath.row];
	cell.textLabel.text = exhibit->caption;

	return cell;
}

-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
	return ![NSStringFromClass([touch.view class]) isEqualToString:@"UITableViewCellContentView"];
}

-(void)handleTap:(UITapGestureRecognizer *)sender
{
	if ( _vwMenu.hidden == NO )
	{
		_vwMenu.hidden = YES;
		return;
	}

	if (sender.state == UIGestureRecognizerStateEnded)
		NSLog( @"tap!");
}


-(IBAction)btnInfoPressed:(id)sender {
	//Make sure that objectId is prevObjectID if it's been set

	MainController *myController = [MainController sharedController];
   //myController.prevObjectId = myController.objectId;
    if ( ([myController.objectId isEqualToString:@"1000"]) || ([myController.objectId isEqualToString:@"999"]))
    {
    }
    else
    {
        myController.objectId = [myController.objectStack firstObject];
    }
    //[myController.objectStack removeObjectAtIndex:0];
    
    [myController ObjectIdUpdated:self];
    NSLog(@"clicked info");
	//[self performSegueWithIdentifier:@"pushToInfoFromList" sender:self];
}
@end
