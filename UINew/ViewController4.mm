//
//  ViewController4.m
//  UINew
//
//  Created by  Roman on 27.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "ViewController4.h"

#import "LocalizationSystem.h"

@interface ViewController4 ()

@end

@implementation ViewController4

- (void)viewDidLoad {
    self.title = @"ViewController4";
    _scrollView.maximumZoomScale = 5.0f;
    _scrollView.delegate = self;
    _imageChanged = YES;
    [super viewDidLoad];

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
    

    
   [_imageView setContentMode:UIViewContentModeScaleAspectFill];
    

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

-(void)viewWillAppear:(BOOL)animated
{
     NSLog(@"Multiguide Will appear called");

    _vwMenu.hidden = YES;
    _buttons = [[NSMutableArray alloc] init];
    _btnTapped = NO;
   //  MainController *myController = [MainController sharedController];
   // myController.objectId = [myController.objectStack lastObject];
   // [myController ObjectIdUpdated:nil];
    [self UpdateButtons];
    [self DrawButtons];
    [self UpdateMap];

}

-(void)viewWillDisappear:(BOOL)animated
{
    NSLog(@"Will disappear");
    _vwMenu.hidden = YES;
    for ( int i = 0; i < _buttons.count; i++)
    {
        [(UIButton*)[_buttons objectAtIndex:i] removeFromSuperview];
    }
    if ( _buttons)
        [_buttons removeAllObjects];
}

-(void)UpdateButtons
{
    dispatch_async(dispatch_get_main_queue(), ^{
    [[_btn3D imageView] setContentMode: UIViewContentModeScaleAspectFit];
    [_btn3D setImage:[UIImage imageNamed:@"3D.png"] forState:UIControlStateNormal];
        _btn3D.tag = 155;
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
    [_btnAllObjects setTitle:AMLocalizedString(@"AllObjects", @"") forState:UIControlStateNormal];
    [_btnBack setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
    [_btnBack setTitle:AMLocalizedString(@"Back", @"") forState:UIControlStateNormal];
    
     MainController *myController = [MainController sharedController];
    [_lblObjectTitle setText:AMLocalizedString(myController.caption, @"") ];
        
    UIEdgeInsets insets = { .left = -14, .right = 0, .top = 0, .bottom = 0 };
    _btnBack.imageEdgeInsets =insets;
        _btnBack.titleEdgeInsets = UIEdgeInsetsMake(0, 14, 0, 0);
        
    [_btn3D setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnContacts setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnMainMenu setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnRemind setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnMap setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnAllObjects setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];
    [_lblNovgorod setText:AMLocalizedString(@"Novgorod", @"")];
    [_lblStateMuseum setText:AMLocalizedString(@"StateMuseum", @"")];
    [_lblRouteDescr setText:AMLocalizedString(@"RouteDescr", @"")];
        
    [_btn3D setEnabled: [myController Determine3DStatus]];
     });
}

- (IBAction)btnAllObjectsPressed:(id)sender {
    [self performSegueWithIdentifier:@"pushToList" sender:self];
}

-(void)DrawButtons
{
     dispatch_async(dispatch_get_main_queue(), ^{
    MainController *myController = [MainController sharedController];
    NSInteger btnNum = myController.mapImages.count;
    if ( btnNum == 1) return;
    
    CGFloat btnHeight = 30.0f;
    CGFloat space = 1.0f;
    CGFloat totalWidth = btnNum * _vwButtonZone.frame.size.width * 0.125f + (btnNum - 1) * space;
    
    CGFloat offset = (_vwButtonZone.frame.size.width - totalWidth) * 0.5f;
    
    for ( int i = 0; i < btnNum; i++ )
    {
        UIButton *button = [[UIButton alloc] initWithFrame:CGRectMake(offset, 0.0f, _vwButtonZone.frame.size.width * 0.125f, btnHeight)];
        [button addTarget:self
                   action:@selector(onBtnSelectTap:)forControlEvents:UIControlEventTouchUpInside];
        
        button.tag = i;

        [_vwButtonZone addSubview:button];
        if ( i == myController.currentFloor)
        {
            [button setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateNormal];
        }
        else
        {
            [button setBackgroundImage:[UIImage imageNamed:@"backgroundButton"] forState:UIControlStateNormal];
        }
        offset += _vwButtonZone.frame.size.width * 0.125f + space;
    
        if ([myController.objectId isEqualToString:@"151"])
        {
            if ( i == 0 )
            {
                [[button imageView] setContentMode: UIViewContentModeScaleAspectFit];
                [button setImage:[UIImage imageNamed:@"podval.png"] forState:UIControlStateNormal];
            }
            else
            {
                [button setTitle:[NSString stringWithFormat:@"%d", i] forState:UIControlStateNormal];
            }
        }
        else
        {
            [button setTitle:[NSString stringWithFormat:@"%d", i + 1] forState:UIControlStateNormal];
        }
    }
     });
}

-(void)UpdateMap
{
    MainController *myController = [MainController sharedController];
    
    dispatch_async(dispatch_get_main_queue(),^{
        
  /*
        if ([myController.mapImages count] > myController.currentFloor) {
            
        }else {
            myController.currentFloor = 0;
        }
        
    */
        [_imageView setImage:[UIImage imageWithContentsOfFile:myController.mapImages[myController.currentFloor]]];

        for ( int i = 0; i < _buttons.count; i++)
        {
            NSLog(@"buttons number %lu", _buttons.count);
            UIButton * btn =(UIButton*) _buttons[i];
            NSLog(@"Btn to remove tag:%lu", btn.tag);
           [btn removeFromSuperview];
        }
 
 
        if ( _buttons)
        {
            [_buttons removeAllObjects];
        }
       

        [_imageView layoutSubviews];
        [_imageView setNeedsLayout];
        
        [_imageView layoutIfNeeded];
        
        CGRect screenBounds = [[UIScreen mainScreen] bounds];
        CGFloat screenScale = [[UIScreen mainScreen] scale];
        CGSize screenSize = CGSizeMake(screenBounds.size.width * screenScale, screenBounds.size.height * screenScale);
        CGFloat imageWidth = _imageView.image.size.width;
        _scrollView.zoomScale = _scrollView.minimumZoomScale = (screenSize.width / imageWidth) / screenScale;
        
        for ( int i = 0; i < myController.objectList.count; i++)
        {
            Exhibit *exhibit = (Exhibit*)[myController.objectList objectAtIndex:i];
            if ( exhibit->floor != myController.currentFloor )
                continue;
            
            NSInteger w = imageWidth / 25;
            UIButton *button = [[UIButton alloc] initWithFrame:[self InsertButtonAtX:exhibit->coord.dx andY:exhibit->coord.dy withWidth:w andHeight:w] ];
            [_imageView addSubview:button];

            switch (exhibit->state)
            {
                case NOT_VISITED:
                    if ( !([myController.objectId isEqualToString:@"151"] || [myController.objectId isEqualToString:@"152"]) )
                    {
                        [button setBackgroundImage:[UIImage imageNamed:@"DefaultButton"] forState:UIControlStateNormal ];
                        [button setTitle:[NSString stringWithFormat:@"%i", i+1] forState:UIControlStateNormal ];
                    }
                    else
                    {
                        if (exhibit->hasMarker == false)
                        {
                            [button setBackgroundImage:[UIImage imageNamed:@"NoMarkerButton"] forState:UIControlStateNormal ];
                        }
                        else
                        {
                            [button setBackgroundImage:[UIImage imageNamed:@"Not visited.png"] forState:UIControlStateNormal ];
                        }
                        if (exhibit->isUnesco)
                        {
                            [button setBackgroundImage:[UIImage imageNamed:@"Not visited unesco"] forState:UIControlStateNormal ];
                        }
                    }
                    break;
                    
                case VISITED:
                    [button setBackgroundImage:[UIImage imageNamed:@"Visited.png"] forState:UIControlStateNormal ];
                    break;
                    
                case LAST_VISITED:
                    [button setBackgroundImage:[UIImage imageNamed:@"Last visited.png"] forState:UIControlStateNormal ];
                    break;
            }

            [button addTarget:self
                action:@selector(onBtnTap:)forControlEvents:UIControlEventTouchUpInside];
            button.userInteractionEnabled=YES;
            button.tag = i;
            [_buttons addObject:button];
        }
    });
}

- (void)onBtnSelectTap:(UIButton*)sender
{
    NSLog (@"tag: %ld", sender.tag);
    MainController *myController = [MainController sharedController];
    if (myController.currentFloor == sender.tag)
        return;
    myController.currentFloor = sender.tag;
    
    [self UpdateButtons];
    [self DrawButtons];
    [self UpdateMap];
}

-(CGRect) InsertButtonAtX:(CGFloat)x andY:(CGFloat)y withWidth:(CGFloat)w andHeight:(CGFloat)h
{
    return CGRectMake( _imageView.frame.size.width * x - w/2, _imageView.frame.size.height * y - h/2, w, h );
}

- (void)onBtnTap:(UIButton*)sender
{
    MainController *myController = [MainController sharedController];
    myController.prevObjectId = myController.objectId;
    Exhibit* exc = (Exhibit*)myController.objectList[sender.tag];
    myController.objectId = exc->Id;
    _btnTapped = YES;
    [myController ObjectIdUpdated:self];
}


-(void)loadEnd {
    MainController *myController = [MainController sharedController];
    NSLog(@"object count: %lu", (unsigned long)myController.objectList.count);
    if (myController.hasNestedObjects == YES && _btnTapped == YES)
    {
        NSLog(@"push to list");
        [myController PushToStack:myController.objectId];
        [self performSegueWithIdentifier:@"pushToList" sender:self ];
    }
    else
    {
        [myController PushToStack:myController.objectId];
        [self performSegueWithIdentifier:@"pushExcursion" sender:self];
    }
}


-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView
{
    return _contentView;
}

- (CGRect)zoomRectForScrollView:(UIScrollView *)scrollView withScale:(float)scale withCenter:(CGPoint)center
{
    CGRect zoomRect;
    zoomRect.size.height = scrollView.frame.size.height / scale;
    zoomRect.size.width  = scrollView.frame.size.width  / scale;
    
    zoomRect.origin.x = center.x - (zoomRect.size.width  / 2.0);
    zoomRect.origin.y = center.y - (zoomRect.size.height / 2.0);
    
    return zoomRect;
}

- (IBAction)btnMenuPressed:(UIButton *)sender
{
    NSLog ( @"Menu pressed" );
    _vwMenu.hidden = !_vwMenu.hidden;
}


- (IBAction)btnBackPressed:(id)sender
{
    MainController *myController = [MainController sharedController];
    //myController.objectId;// = [myController.objectStack lastObject];
    //[myController ObjectIdUpdated:self];
    
    [self performSegueWithIdentifier:@"pushExcursion" sender:self];;
}
@end
