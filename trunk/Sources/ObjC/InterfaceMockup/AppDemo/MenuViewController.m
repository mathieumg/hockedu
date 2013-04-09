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
    [self.view addSubview:saveMapView];
    
    //mainMenuView.hidden = YES;
    loadMapView.hidden = YES;
    saveMapView.hidden = YES;
    
    UIImage *buttonImage = [[UIImage imageNamed:@"blueButton@2x.png"]
                            resizableImageWithCapInsets:UIEdgeInsetsMake(18, 18, 18, 18)];
    UIImage *buttonImageHighlight = [[UIImage imageNamed:@"blueButtonHighlight@2x.png"]
                                     resizableImageWithCapInsets:UIEdgeInsetsMake(18, 18, 18, 18)];
    
    UIImage *buttonImageDisabled = [[UIImage imageNamed:@"blueButtonDisabled@2x.png"]
                                     resizableImageWithCapInsets:UIEdgeInsetsMake(18, 18, 18, 18)];
    
    [newMapButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [newMapButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [loadMapButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [loadMapButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [settingsButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [settingsButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    [settingsButton setBackgroundImage:buttonImageDisabled forState:UIControlStateDisabled];
    
    [signInButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [signInButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [backMainMenuButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [backMainMenuButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [backEditorButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [backEditorButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [saveMapButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [saveMapButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [discardMainMenuButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [discardMainMenuButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    userId = 12;
    mapId = 0;
    
    [buttonImage retain];
    [buttonImageHighlight retain];
    [buttonImageDisabled retain];
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

- (IBAction)touchBackMainMenuButton:(UIButton *)sender
{
    mainMenuView.hidden = NO;
    loadMapView.hidden = YES;
}

- (IBAction)touchBackEditorButton:(UIButton *)sender
{
    HockeduAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    [delegate afficherVueAnimee];
}

- (IBAction)touchSaveMapButton:(UIButton *)sender
{    
    HockeduAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    [delegate showFieldSaved:textMapName.text:textMapDescription.text:switchMapPublic.state];
}

- (IBAction)touchDiscardMainMenuButton:(UIButton *)sender
{
    mainMenuView.hidden = NO;
    saveMapView.hidden = YES;
}

- (IBAction)loadMapButton:(UIButton *)sender
{
    [Map listMaps:^(NSArray *maps, NSError *error) {
        if (error) {
            [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Error", nil) message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:NSLocalizedString(@"OK", nil), nil] show];
        } else {
            _userMaps = [[NSArray alloc] initWithArray:maps];
            
            [self.tableView reloadData];
            mainMenuView.hidden = YES;
            loadMapView.hidden = NO;
        }
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
    Map* map = [_userMaps objectAtIndex:indexPath.row];
    mapId = map.mapID;
    
    [Map downloadMap:^(NSArray *maps, NSError *error) {
        if (error) {
            [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Error", nil) message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:NSLocalizedString(@"OK", nil), nil] show];
        } else {
            //_userMaps = [[NSArray alloc] initWithArray:maps];
            //NSLog(@"Mofo");
            
            //[self showEditor];
            
            //mainMenuView.hidden = NO;
            //loadMapView.hidden = YES;
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
    [backMainMenuButton release];
    [saveMapView release];
    [backEditorButton release];
    [saveMapButton release];
    [switchMapPublic release];
    [textMapDescription release];
    [textMapName release];
    [discardMainMenuButton release];
    [super dealloc];
}
@end
