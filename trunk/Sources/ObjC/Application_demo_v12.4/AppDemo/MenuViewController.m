//
//  MenuViewController.m
//  AppDemo
//

#import "MenuViewController.h"
#import "AppDemoAppDelegate.h"

@implementation MenuViewController

@synthesize boutonGenerateur;

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

- (IBAction)afficherVueAnimee
{
    AppDemoAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    [delegate afficherVueAnimee];
}

- (IBAction)afficherVueRouge
{
    AppDemoAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    [delegate afficherVueRouge];
}

- (IBAction)genererValeur
{
    NSUInteger valeurEntiere = 1 + (arc4random() % 100);
    NSString *chaineValeur = [NSString stringWithFormat:@"%d", valeurEntiere];
    [boutonGenerateur setTitle:chaineValeur forState:UIControlStateNormal];
    
    
}

@end
