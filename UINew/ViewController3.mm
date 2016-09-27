//
//  ViewController3.m
//  UINew
//
//  Created by  Roman on 26.09.15.
//  Copyright (c) 2015  Roman. All rights reserved.
//

#import "ViewController3.h"

#import "LocalizationSystem.h"

@implementation ViewController3

-(void)GetAllImages
{
    MainController *myController = [MainController sharedController];
    NSMutableArray *fullImageNames = [[NSMutableArray alloc]init];
    for (int i=0; i < myController.imagesO.count; i++)
    {
        NSString *fullImageName = [myController.imagesO[i] stringByReplacingOccurrencesOfString:@".jpg" withString:@"_full.jpg"];
        if (![CacheControll isCachedAtPath:fullImageName])
          {
             [fullImageNames addObject:fullImageName];
          }
        
    }
    

        if (fullImageNames.count)
        {
            [myController loadFiles:fullImageNames sender:self];
        }
        else
        {
            //[myController updateLoaderDelegate:100.0 sender:nil];
            [self loadEnd];
        }
}

-(void)viewWillDisappear:(BOOL)animated
{
    [self.images removeAllObjects];
}
-(void)viewWillAppear:(BOOL)animated
{
    self.images = [[NSMutableArray alloc] init];
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
    
    [self GetAllImages];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    MainController *myController = [MainController sharedController];
    _scrollView.delegate = self;
    _scrollView.maximumZoomScale = 5.0f;
    _scrollView.minimumZoomScale = 1.0f;
    
    [_scrollView setContentOffset:CGPointMake(0,0)];
    [self.view addSubview:self.imageView ];
    [_imageView setUserInteractionEnabled:YES];
    _imageView.contentMode = UIViewContentModeScaleAspectFit;
    UISwipeGestureRecognizer *swipeLeft = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipe:)];
    UISwipeGestureRecognizer *swipeRight = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipe:)];
    
    // Setting the swipe direction.
    [swipeLeft setDirection:UISwipeGestureRecognizerDirectionLeft];
    [swipeRight setDirection:UISwipeGestureRecognizerDirectionRight];
    
    // Adding the swipe gesture on image view
    [_imageView addGestureRecognizer:swipeLeft];
    [_imageView addGestureRecognizer:swipeRight];
    
   

     _imageView.contentMode = UIViewContentModeScaleAspectFit;

}

- (void)handleSwipe:(UISwipeGestureRecognizer *)swipe {
    
     MainController *myController = [MainController sharedController];
    if (swipe.direction == UISwipeGestureRecognizerDirectionLeft) {
        _activeIndex++;
        if (_activeIndex>myController.imagesO.count-1)
        {
            _activeIndex = 0;
        }
    }
    
    if (swipe.direction == UISwipeGestureRecognizerDirectionRight) {
        _activeIndex--;
        if (_activeIndex < 0)
        {

            _activeIndex = myController.imagesO.count - 1;
        }
    }
    [UIView transitionWithView:_imageView duration:0.5
                       options:UIViewAnimationOptionTransitionCrossDissolve animations:^{
                           _imageView.image = _images[_activeIndex];
                       } completion:nil];
    [_descr setText:myController.descriptions[_activeIndex]];
    
}

-(void)loadEnd  {
        MainController *myController = [MainController sharedController];
    NSLog(@"LOAD END CALL");
    for (int i = 0; i<myController.images.count; i++)
    {
        NSString *fullImageName = [myController.images[i] stringByReplacingOccurrencesOfString:@".jpg" withString:@"_full.jpg"];
    
        
        UIImage *image = [UIImage imageWithContentsOfFile:fullImageName];
        //[_imageView setImage:image];
        if (image)
            [self.images addObject:image];
    }
    
    self.activeIndex = myController.index;
    if (self.images.count>0)
    {
        [_imageView setImage:self.images[self.activeIndex]];
        [_descr setText:myController.descriptions[self.activeIndex]];
    }
    [self UpdateButtons];
}

-(void)UpdateButtons
{
    
    [_btnBack setImage:[UIImage imageNamed:@"back.png"] forState:UIControlStateNormal];
    [_btnBack setTitle:AMLocalizedString(@"Back",@"") forState:UIControlStateNormal];
    [_btnBack setBackgroundImage:[UIImage imageNamed:@"highlight.png"] forState:UIControlStateHighlighted];

}


- (IBAction)onTap:(id)sender
{
    [self.navigationController popViewControllerAnimated:YES ];
}

- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView
{
    return _imageView;
}

-(CGRect)zoomRectForScrollView:(UIScrollView *)scrollView withScale:(float)scale withCenter:(CGPoint)center
{
    CGRect zoomRect;
    zoomRect.size.height = scrollView.frame.size.height / scale;
    zoomRect.size.width  = scrollView.frame.size.width  / scale;
    
    zoomRect.origin.x = center.x - (zoomRect.size.width  / 2.0);
    zoomRect.origin.y = center.y - (zoomRect.size.height / 2.0);
    
    return zoomRect;
}

-(void)FlipToImage:(UIImage*)secondImage
{
    [UIView transitionWithView:_imageView duration:0.5
                       options:UIViewAnimationOptionTransitionFlipFromRight animations:^{
                           _imageView.image = secondImage;
                       } completion:nil];
}



@end
