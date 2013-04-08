//
//  MenuViewController.m
//  AppDemo
//

#import "MenuViewController.h"
#import "LoadMapViewController.h"
#import "HockeduAppDelegate.h"
#import "BackgroundLayer.h"
#import "AFJSONRequestOperation.h"
#import "AFHTTPClient.h"
#import "Map.h"
#import "MapTableViewCell.h"

@implementation MenuViewController

@synthesize _userMaps;
@synthesize userId;
@synthesize mapId;

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
    
    [self.view addSubview:mainMenuView];
    [self.view addSubview:loadMapView];
    
    //mainMenuView.hidden = YES;
    loadMapView.hidden = YES;
    
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
    
    userId = 12;
    mapId = 17;
    
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

- (IBAction)loadMapButton:(UIButton *)sender
{
    [Map globalTimelinePostsWithBlock:^(NSArray *maps, NSError *error) {
        if (error) {
            [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Error", nil) message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:NSLocalizedString(@"OK", nil), nil] show];
        } else {
            _userMaps = [[NSArray alloc] initWithArray:maps];
            
            [self.tableView reloadData];
            mainMenuView.hidden = YES;
            loadMapView.hidden = NO;
        }
        
        //[_activityIndicatorView stopAnimating];
        //self.navigationItem.rightBarButtonItem.enabled = YES;
    }:userId];
    
    /*
    NSURL *url = [NSURL URLWithString:@"http://hockedu.com"];
    AFHTTPClient *httpClient = [[AFHTTPClient alloc] initWithBaseURL:url];
    [httpClient setDefaultHeader:@"Accept" value:@"application/json"];
    [httpClient registerHTTPOperationClass:[AFJSONRequestOperation class]];
    NSDictionary *params = [NSDictionary dictionaryWithObjectsAndKeys:
                            @"testies", @"username",
                            @"608b9a09de61fea254bbebdcadc0fe8c38ae2ccb", @"password",
                            nil];
    
    [httpClient postPath:@"/remote/listmaps" parameters:params success:^(AFHTTPRequestOperation *operation, id responseObject) {
        //_userMaps = responseObject;
        
        
        //NSLog(@"Auth key: %@", [responseObject valueForKeyPath:@"auth_key"]);
        //NSLog(@"Error: %@", [responseObject valueForKeyPath:@"error"]);
        //NSLog(@"Auth key: %@", [responseObject valueForKeyPath:@"auth_key"]);
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"[HTTPClient Error]: %@", error.localizedDescription);
    }];
    [httpClient release];
    */
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [_userMaps count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *CellIdentifier = @"Cell";
    
    MapTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (!cell) {
        cell = [[MapTableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier];
    }
    
    cell.map = [_userMaps objectAtIndex:indexPath.row];
    
    return cell;
}

#pragma mark - UITableViewDelegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return [MapTableViewCell heightForCellWithMap:[_userMaps objectAtIndex:indexPath.row]];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    //Map* map = [_userMaps objectAtIndex:indexPath.row];
    
    
    [Map downloadMap:^(NSArray *maps, NSError *error) {
        if (error) {
            [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Error", nil) message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:NSLocalizedString(@"OK", nil), nil] show];
        } else {
            //_userMaps = [[NSArray alloc] initWithArray:maps];
            
            
            mainMenuView.hidden = NO;
            loadMapView.hidden = YES;
        }}:mapId];
    
    
}

- (IBAction)showEditor
{
    HockeduAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    [delegate afficherVueAnimee];
}

- (void)dealloc {
    [newMapButton release];
    [loadMapButton release];
    [settingsButton release];
    [signInButton release];
    [mainMenuView release];
    [loadMapView release];
    //[mapListTableView release];
    [super dealloc];
}
@end
