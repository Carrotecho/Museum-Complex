//
//  ViewController2.m
//  UINew
//
//  Created by  Roman on 24.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "ViewController2.h"

#import "TextCell.h"
#import "ImageCell.h"
#import "LocalizationSystem.h"

@interface ViewController2 ()

-(UIImage*)MakeScreenshot;

@end

@implementation ViewController2


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

	[[_btnToMainMenu imageView] setContentMode: UIViewContentModeScaleAspectFit];
	[_btnToMainMenu setImage:[UIImage imageNamed:@"ToMainMenu.png"] forState:UIControlStateNormal];
        
    [[_btnMenu imageView] setContentMode:UIViewContentModeScaleAspectFit];
    [_btnMenu setImage:[UIImage imageNamed:@"MainMenuButton"] forState:UIControlStateNormal];

	MainController *myController = [ MainController sharedController];
        
       if ( [myController.excursionMode isEqualToString:@"Main"] )
       {
        _btnShare.hidden = NO;
        [[_btnShare imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnShare setImage:[UIImage imageNamed:@"ShareIco"] forState:UIControlStateNormal];
        
        [[_btnVK imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnVK setImage:[UIImage imageNamed:@"VK"] forState:UIControlStateNormal];
        
        
        [[_btnFB imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnFB setImage:[UIImage imageNamed:@"FB"] forState:UIControlStateNormal];
        
        [[_btnTwitter imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnTwitter setImage:[UIImage imageNamed:@"Twitter"] forState:UIControlStateNormal];
        
        [[_btnInstagram imageView] setContentMode:UIViewContentModeScaleAspectFit];
        [_btnInstagram setImage:[UIImage imageNamed:@"Instagram"] forState:UIControlStateNormal];
        [_btnInstagram setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnVK setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnFB setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
        [_btnShare setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
       }
	
	if ( [myController.excursionMode isEqualToString:@"Audio"] || [myController.excursionMode isEqualToString:@"3DAudio"] || [myController.excursionMode isEqualToString:@"Short"] ||  [myController.excursionMode isEqualToString:@"3D"] )
	{
		[[_btnHowTo imageView] setContentMode: UIViewContentModeScaleAspectFit];

		if ( [myController.excursionMode isEqualToString:@"Short"] )
		{
			[_btnHowTo setTitle:AMLocalizedString(@"Directions", @"")  forState:UIControlStateNormal];
		}
		else
		{
			[_btnHowTo setImage:[UIImage imageNamed:@"play.png"] forState:UIControlStateNormal];
		}
        
        //[[_btnMultiguide imageView] setContentMode:UIViewCOntentM];
        [_btnMultiguide setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
		[_btnMultiguide setTitle:AMLocalizedString(@"Back", @"")  forState:UIControlStateNormal];
	}

	if ( [myController.excursionMode isEqualToString:@"3D"] )
	{
		[[_btnHowTo imageView] setContentMode: UIViewContentModeScaleAspectFit];
		[_btnMultiguide setTitle:AMLocalizedString(@"Back", @"") forState:UIControlStateNormal];
		[_btnHowTo setEnabled:NO];
		[_btnHowTo setImage:[UIImage imageNamed:@"play.png"] forState:UIControlStateNormal];
	}
	else if ( [myController.excursionMode isEqualToString:@"Main"] )
	{
        [_btnHowTo setImage:nil forState:UIControlStateNormal];
        [_btnMultiguide setImage:nil forState:UIControlStateNormal];
		[_btnMultiguide setTitle:AMLocalizedString(@"Multiguide", @"")  forState:UIControlStateNormal];
		[_btnHowTo setTitle:AMLocalizedString(@"Directions", @"")  forState:UIControlStateNormal];
	}

	[_btnContacts setTitle:AMLocalizedString(@"Contacts", @"") forState:UIControlStateNormal];
	[_btnRemind setTitle:AMLocalizedString(@"Remind", @"") forState:UIControlStateNormal];
	[_btnMap setTitle:AMLocalizedString(@"Map", @"") forState:UIControlStateNormal];
	[_btnToMainMenu setTitle:AMLocalizedString(@"MainMenu", @"") forState:UIControlStateNormal];
	
	[_btnMultiguide setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnHowTo setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnContacts setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnToMainMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnRemind setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
	[_btnMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];

	[_lblNovgorod setText:AMLocalizedString(@"Novgorod", @"")];
	[_lblStateMuseum setText:AMLocalizedString(@"StateMuseum", @"")];
        
    [_btn3D setEnabled: [myController Determine3DStatus]];
	});
}


-(void)viewDidLoad
{
	[super viewDidLoad];

	_tableView.tag = 1;
	_tableView.dataSource = self;
	_tableView.delegate = self;
	_gestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTap:)];
	_gestureRecognizer.delegate = self;

	MainController *myController = [MainController sharedController];

	[self.view addGestureRecognizer:_gestureRecognizer];

	if ([ myController.excursionMode isEqualToString:@"Audio"] || [ myController.excursionMode isEqualToString:@"3DAudio"])
	{
		if ( myController.audio)
		{
			NSURL *backgroundMusicURL = [NSURL fileURLWithPath:myController.audio];
			NSError *error = nil;
			
			_player = [[AVAudioPlayer alloc] initWithContentsOfURL:backgroundMusicURL error:&error];
			_player.delegate = self;
		}
	}

	if ( myController.currentLang == 0 )
		LocalizationSetLanguage ( @"ru" );
	else if ( myController.currentLang == 1 )
		LocalizationSetLanguage ( @"en" );
    
	//[self UpdateButtons];
    
    _tableView.estimatedRowHeight = 44.0;
    _tableView.rowHeight = UITableViewAutomaticDimension;
    
    [_tableView layoutIfNeeded];
    [_tableView reloadData];
}

-(void)viewWillAppear:(BOOL)animated
{
	_tableView.estimatedRowHeight = 44.0;
	_tableView.rowHeight = UITableViewAutomaticDimension;

	[_tableView layoutIfNeeded];
	[_tableView reloadData];
//MainController *myController = [MainController sharedController];
   // NSLog( @"Excursion type: %@", myController.excursionMode);
   // NSLog (myController.objectId);
    
	_vwMenu.hidden = YES;
	_refreshed = NO;
    
    [self UpdateButtons];

}



-(void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
}


-(void)viewWillDisappear:(BOOL)animated
{
    _refreshed = NO;
    _vwMenu.hidden = YES;
    _vwSocials.hidden = YES;
    MainController *myController = [MainController sharedController];
    
    if ([ myController.excursionMode isEqualToString:@"Audio"] || [ myController.excursionMode isEqualToString:@"3DAudio"])
    {
        if ( myController.audio && _player.playing )
        {
            [_player stop];
        }
    }

}

- (IBAction)btnMenuPressed:(UIButton *)sender
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

- (IBAction)btnTap:(id)sender
{
    MainController *myController = [ MainController sharedController];
    
     if ( [myController.excursionMode isEqualToString:@"Main"] )
    {
        //[self performSegueWithIdentifier:@"pushView5" sender:self];
    }
    
}

- (IBAction)ExtraButtonPressed:(id)sender
{
    MainController *myController = [ MainController sharedController];
    if ( [myController.excursionMode isEqualToString:@"Main"] )
    {
        [self performSegueWithIdentifier:@"pushMultiguide" sender:self];
    }
    else //means back
    {
        MainController *myController = [MainController sharedController];
        
        if ([ myController.excursionMode isEqualToString:@"Audio"] || [ myController.excursionMode isEqualToString:@"3DAudio"])
        {
            if ( myController.audio && _player.playing )
            {
                [_player stop];
            }
        }
        
        myController.prevObjectId = myController.objectId;
        //[myController.objectStack removeLastObject];
        //myController.objectId = [myController.objectStack lastObject];
        NSMutableArray *navigationArray = [[NSMutableArray alloc] initWithArray: self.navigationController.viewControllers];
        NSString *title = @"";
        myController.objectId = [myController.objectStack lastObject];
        if ([navigationArray count] > 1)
        {
            title = ((UIViewController*)navigationArray[navigationArray.count - 2]).title;
        }
        if ( [myController.objectStack count] > 1 && !([title isEqualToString:@"3DViewCntrlr"]))
        {
            [myController.objectStack removeLastObject];
            myController.objectId = [myController.objectStack lastObject];
        }
        
        //myController.objectId = [myController.objectStack lastObject];
        
        //[myController PushToStack:myController.prevObjectId];
        [myController ObjectIdUpdated:self];
        
        [self.navigationController popViewControllerAnimated:YES];
    }
}

-(IBAction)btnInstagramPressed:(id)sender
{
	_vwSocials.hidden = YES;
	
	MainController* controller = [MainController sharedController];
	UIImage* screengrab = [self MakeScreenshot];
	[controller.socials PostAdvertising:SOCIALS_INSTAGRAM withScreenshot: screengrab];
}

-(IBAction)btnVKPressed:(id)sender
{
	_vwSocials.hidden = YES;
	
	MainController* controller = [MainController sharedController];
	UIImage* screengrab = [self MakeScreenshot];
	[controller.socials PostAdvertising:SOCIALS_VK withScreenshot: screengrab];
}

-(IBAction)btnFBPressed:(id)sender
{
	_vwSocials.hidden = YES;
	
	MainController* controller = [MainController sharedController];
	UIImage* screengrab = [self MakeScreenshot];
	[controller.socials PostAdvertising:SOCIALS_FACEBOOK withScreenshot: screengrab];
}

-(IBAction)btnTwitterPressed:(id)sender
{
	_vwSocials.hidden = YES;
	
	MainController* controller = [MainController sharedController];
	UIImage* screengrab = [self MakeScreenshot];
	[controller.socials PostAdvertising:SOCIALS_TWITTER withScreenshot: screengrab];
}

-(void)loadEnd {
    //[self.navigationController popViewControllerAnimated:YES];
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 1;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    MainController *myController = [MainController sharedController];
    return 2 + [myController.images count];
}

-(void) tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
    MainController *myController = [MainController sharedController];
   
    NSInteger numRows = 1 + [myController.images count];
    if (!_refreshed)
    {
        if (indexPath.row == numRows) {
            
        [_tableView reloadData];
        _refreshed = YES;
        }
    }
}

-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    return YES;
}
-(void)boldFontForLabel:(UILabel *)label{
    UIFont *currentFont = label.font;
    UIFont *newFont = [UIFont fontWithName:[NSString stringWithFormat:@"%@-Bold",currentFont.fontName] size:currentFont.pointSize];
    label.font = newFont;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    MainController *myController = [MainController sharedController];
    if (indexPath.row == 0)
    {
        TextCell *cell = (TextCell*)[tableView dequeueReusableCellWithIdentifier:@"CellText"];
        cell.label.text = myController.caption;
        NSInteger fontSize;
        UITraitCollection *traitCollection = [[self view] traitCollection];
        if (traitCollection.horizontalSizeClass == UIUserInterfaceSizeClassCompact)
        {
            fontSize = 21;
        }
        else
        {
            fontSize = 23;
        }
        
        [cell.label setFont:[UIFont fontWithName:@"Verdana-Bold" size:fontSize]];
        cell.label.textAlignment = NSTextAlignmentCenter;        //[self boldFontForLabel: cell.label];
        
        return cell;
    }
    if (indexPath.row == 1)
    {
        ImageCell *cell = (ImageCell*)[tableView dequeueReusableCellWithIdentifier:@"CellImage"];
        cell.image.contentMode = UIViewContentModeScaleAspectFit;
        if (myController.images && [myController.images count] > 0) {
            [cell.image setImage:[UIImage imageWithContentsOfFile:myController.images[0]]];
        }else {
            printf("image nil \n");
        }
        

        return cell;
    }
    
    if (indexPath.row == 2)
    {
        TextCell *cell = (TextCell*)[tableView dequeueReusableCellWithIdentifier:@"CellText"];
        cell.label.text = myController.text;
        UITraitCollection *traitCollection = [[self view] traitCollection];
        NSInteger fontSize;
        if (traitCollection.horizontalSizeClass == UIUserInterfaceSizeClassCompact)
        {
            fontSize = 16;
        }
        else
        {
            fontSize = 18;
        }
        
        [cell.label setFont:[UIFont fontWithName:@"Verdana" size:fontSize]];
        cell.label.textAlignment = NSTextAlignmentLeft;
        /*NSMutableParagraphStyle *style =  [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
        style.alignment = NSTextAlignmentLeft;
        style.firstLineHeadIndent = 10.0f;
        cell.label.lineBreakMode = NSLineBreakByWordWrapping;
        cell.label.numberOfLines = 0;
        //style.headIndent = 10.0f;
        //style.tailIndent = -10.0f;
        
        NSAttributedString *attrText = [[NSAttributedString alloc] initWithString:myController.text 
                                                                attributes:@{ NSParagraphStyleAttributeName : style}];
        
        cell.label.attributedText = attrText;
        [cell.label setAdjustsFontSizeToFitWidth:YES];
         */
        return cell;
    }
    else
    {
        ImageCell *cell = (ImageCell*)[tableView dequeueReusableCellWithIdentifier:@"CellImage"];
        cell.image.contentMode = UIViewContentModeScaleAspectFit;
        UIImage*img = [UIImage imageWithContentsOfFile:myController.images[indexPath.row-2]];
        NSLog(@"path: %@", myController.images[indexPath.row-2]);
        [cell.image setImage:img];

        return cell;
    }
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ( _vwMenu.hidden == NO )
    {
        _vwMenu.hidden = YES;
        return;
    }

    
    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
    if ( [ cell.reuseIdentifier isEqualToString:@"CellImage"] )
    {
        MainController *myController = [MainController sharedController];
        if ( indexPath.row == 1)
        {
            myController.index = 0;
        }
        else if ( indexPath.row >= 3 )
        {
            myController.index = indexPath.row - 2;
        }
        
      [self performSegueWithIdentifier:@"segue2" sender:self];
    }
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
    return ![NSStringFromClass([touch.view class]) isEqualToString:@"UITableViewCellContentView"];
}

- (void)handleTap:(UITapGestureRecognizer *)sender
{
    if ( _vwMenu.hidden == NO )
    {
        _vwMenu.hidden = YES;
        return;
    }
    
    if (sender.state == UIGestureRecognizerStateEnded)
    {
        NSLog( @"tap!");
    }
}

- (IBAction)extraButton2Tap:(id)sender
{
    MainController *myController = [ MainController sharedController];
    
    if ( [myController.excursionMode isEqualToString:@"3DAudio"] || [myController.excursionMode isEqualToString:@"Audio"] )
    {
        if ( !_player.playing )
        {
            [_player play];
            [_btnHowTo setImage:[UIImage imageNamed:@"pause.png"] forState:UIControlStateNormal];
        }
        else
        {
            [_player stop];
            [_btnHowTo setImage:[UIImage imageNamed:@"play.png"] forState:UIControlStateNormal];
        }
    }
    else if ( [myController.excursionMode isEqualToString:@"Main"] || [myController.excursionMode isEqualToString:@"Short"]  )
    {
        [self performSegueWithIdentifier:@"pushHowToFind" sender:self];
    }
}

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player
                       successfully:(BOOL)flag
{
    [_btnHowTo setImage:[UIImage imageNamed:@"play.png"] forState:UIControlStateNormal];
}

- (IBAction)btnSharePressed:(id)sender
{
    _vwSocials.hidden = !_vwSocials.hidden;
}

-(UIImage*)MakeScreenshot
{
	CGRect rect;
	rect = CGRectMake ( 0, 0, self.view.frame.size.width, self.view.frame.size.height );
	UIGraphicsBeginImageContext ( rect.size );
	
	CGContextRef context = UIGraphicsGetCurrentContext ();
	[self.view.layer renderInContext:context];
	
	UIImage* image = UIGraphicsGetImageFromCurrentImageContext ();
	UIGraphicsEndImageContext ();
	
	return image;
}

@end
