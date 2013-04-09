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
#import <CommonCrypto/CommonDigest.h>
#import "Model.h"

@implementation MenuViewController

@synthesize _userMaps;
@synthesize userId;
@synthesize mapId;

-(NSString*) sha1:(NSString*)input
{
    const char *cstr = [input cStringUsingEncoding:NSUTF8StringEncoding];
    NSData *data = [NSData dataWithBytes:cstr length:input.length];
    
    uint8_t digest[CC_SHA1_DIGEST_LENGTH];
    
    CC_SHA1(data.bytes, data.length, digest);
    
    NSMutableString* output = [NSMutableString stringWithCapacity:CC_SHA1_DIGEST_LENGTH * 2];
    
    for(int i = 0; i < CC_SHA1_DIGEST_LENGTH; i++)
        [output appendFormat:@"%02x", digest[i]];
    
    return output;
    
}

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
    [self.view addSubview:mainMenuLoginView];
    [self.view addSubview:mainMenuMiddleView];
    
    mainMenuLoginView.frame = mainMenuMiddleView.frame;
    
    //mainMenuView.hidden = YES;
    loadMapView.hidden = YES;
    saveMapView.hidden = YES;
    mainMenuLoginView.hidden = YES;
    
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
    [loadMapButton setBackgroundImage:buttonImageDisabled forState:UIControlStateDisabled];
    
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
    
    [signInButton2 setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [signInButton2 setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [registerButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [registerButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [backToMainFromLogin setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [backToMainFromLogin  setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    userId = 0;
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

- (IBAction)touchBackToMainFromLogin:(UIButton *)sender
{
    mainMenuMiddleView.hidden = NO;
    mainMenuLoginView.hidden = YES;
}

- (IBAction)touchRegisterButton:(UIButton *)sender
{
    NSURL *url = [NSURL URLWithString:@"http://hockedu.com/register"];
    [[UIApplication sharedApplication] openURL:url];
}

- (IBAction)touchMainSignInButton:(UIButton *)sender
{
    if( userId == 0 )
    {
        mainMenuLoginView.hidden = NO;
        mainMenuMiddleView.hidden = YES;
    }
    else{
        
        [signInButton setTitle:@"Sign In" forState:UIControlStateNormal];
        userId = 0;
        authKey = @"";
        [Model saveLogin:0 : @""];
        [loadMapButton setEnabled:NO];
    }
}

- (IBAction)touchBackMainMenuButton:(UIButton *)sender
{
    mainMenuMiddleView.hidden = NO;
    mainMenuView.hidden = NO;
    loadMapView.hidden = YES;
    mainMenuLoginView.hidden = YES;
}

- (IBAction)touchBackEditorButton:(UIButton *)sender
{
    HockeduAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
    [delegate afficherVueAnimee];
}

- (IBAction)touchSaveMapButton:(UIButton *)sender
{    
    if( userId == 0 )
    {
        [[[UIAlertView alloc] initWithTitle:@"Error" message:@"You must be logged in to save a map." delegate:nil cancelButtonTitle:nil otherButtonTitles:@"Ok", nil] show];
    }
    else{
        HockeduAppDelegate* delegate = [[UIApplication sharedApplication] delegate];
        [delegate showFieldSaved:textMapName.text : textMapDescription.text : switchMapPublic.on];
    }
}

- (IBAction)touchDiscardMainMenuButton:(UIButton *)sender
{
    mainMenuView.hidden = NO;
    mainMenuMiddleView.hidden = NO;
    saveMapView.hidden = YES;
}

- (IBAction)loadMapButton:(UIButton *)sender
{
     //NSLog(@"%@",authKey);
    
    [Map listMaps:^(NSArray *maps, NSError *error) {
        if (error) {
            [[[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Error", nil) message:[error localizedDescription] delegate:nil cancelButtonTitle:nil otherButtonTitles:NSLocalizedString(@"OK", nil), nil] show];
        } else {
            _userMaps = [[NSArray alloc] initWithArray:maps];
            
            [self.tableView reloadData];
            mainMenuView.hidden = YES;
            loadMapView.hidden = NO;
            mainMenuLoginView.hidden = YES;
            mainMenuMiddleView.hidden = YES;
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

- (IBAction)touchSignIn2:(UIButton *)sender
{
    
    /*
    AFHTTPClient *httpClient = [[AFHTTPClient alloc] initWithBaseURL:[NSURL URLWithString:@"http://hockedu.com"]];
    [httpClient setParameterEncoding:AFFormURLParameterEncoding];
    NSMutableURLRequest *request = [httpClient requestWithMethod:@"POST"
                                                            path:@"/remote/authenticate"
                                                      parameters:@{@"username":usernameField.text,@"password":[self sha1: passwordField.text]}];
    
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [httpClient registerHTTPOperationClass:[AFHTTPRequestOperation class]];
    [httpClient setDefaultHeader:@"Accept" value:@"application/json"];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        // Print the response body in text
        //NSLog(@"Response: %@", [[NSString alloc] initWithData:responseObject encoding:NSUTF8StringEncoding]);
        
        
        id error1 = [responseObject valueForKey:@"error"];
        if (!error1) {
            userId = [[responseObject valueForKeyPath:@"id_user"] integerValue];
            [Model saveLogin:userId : [responseObject valueForKeyPath:@"auth_key"]];
        }
        else
        {
            UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Error" message:@"Invalid login credentials." delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
            [alert show];
        }
        
     
     
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        //NSLog(@"Error: %@", error);
    }];
    [operation start];
    
    */
    
    NSURL *url = [NSURL URLWithString:@"http://hockedu.com"];
    AFHTTPClient *httpClient = [[AFHTTPClient alloc] initWithBaseURL:url];
    [httpClient setDefaultHeader:@"Accept" value:@"application/json"];
    [httpClient registerHTTPOperationClass:[AFJSONRequestOperation class]];
    NSDictionary *params = [NSDictionary dictionaryWithObjectsAndKeys:
                            usernameField.text, @"username",
                            [self sha1: passwordField.text], @"password",
                            nil];
    
    [httpClient postPath:@"/remote/authenticate" parameters:params success:^(AFHTTPRequestOperation *operation, id responseObject) {
        //_userMaps = responseObject;
        
        if( [responseObject valueForKeyPath:@"error"] != nil )
        {
            UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Error" message:@"Invalid login credentials." delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
            [alert show];
        }
        else{
            userId = [[responseObject valueForKeyPath:@"id_user"] integerValue];
            authKey = [NSString stringWithFormat:@"%@",[responseObject valueForKeyPath:@"auth_key"]];
           
            [Model saveLogin:userId : [responseObject valueForKeyPath:@"auth_key"]];
            [signInButton setTitle:@"Sign Out" forState:UIControlStateNormal];
            [loadMapButton setEnabled:YES];
            [self showEditor];
        }
        
        //NSLog(@"Auth key: %@", [responseObject valueForKeyPath:@"auth_key"]);
        //NSLog(@"Error: %@", [responseObject valueForKeyPath:@"error"]);
        //NSLog(@"Auth key: %@", [responseObject valueForKeyPath:@"auth_key"]);
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        //NSLog(@"[HTTPClient Error]: %@", error.localizedDescription);
    }];
    [httpClient release];
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
    [mainMenuMiddleView release];
    [mainMenuLoginView release];
    [signInButton release];
    [registerButton release];
    [backToMainFromLogin release];
    [signInButton2 release];
    [usernameField release];
    [passwordField release];
    [super dealloc];
}
@end
