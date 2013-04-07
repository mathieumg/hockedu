//
//  MenuViewController.m
//  AppDemo
//

#import "MenuViewController.h"
#import "LoadMapViewController.h"
#import "HockeduAppDelegate.h"
#import "BackgroundLayer.h"

@implementation MenuViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{      
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    UIImage *buttonImage = [[UIImage imageNamed:@"blueButton@2x.png"]
                            resizableImageWithCapInsets:UIEdgeInsetsMake(18, 18, 18, 18)];
    UIImage *buttonImageHighlight = [[UIImage imageNamed:@"blueButtonHighlight@2x.png"]
                                     resizableImageWithCapInsets:UIEdgeInsetsMake(18, 18, 18, 18)];
    
    [newMapButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [newMapButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [loadMapButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [loadMapButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [settingsButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [settingsButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [signInButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [signInButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [buttonImage retain];
    [buttonImageHighlight retain];
}

- (void)viewWillAppear:(BOOL)animated
{
    CAGradientLayer *bgLayer = [BackgroundLayer blueGradient];
    CGRect invertFrame = self.view.bounds;
    invertFrame.size.height = self.view.bounds.size.width;
    invertFrame.size.width = self.view.bounds.size.height;
    bgLayer.frame = invertFrame;
    [self.view.layer insertSublayer:bgLayer atIndex:0];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
	return YES;
}

- (IBAction)showEditor
{
    HockeduAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    [delegate afficherVueAnimee];
}

-(IBAction) openMapList
{
    HockeduAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    [delegate showMapList];
    
}

- (void)dealloc {
    [newMapButton release];
    [loadMapButton release];
    [settingsButton release];
    [signInButton release];
    [super dealloc];
}
@end
