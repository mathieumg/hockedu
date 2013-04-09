//
//  AppDemoAppDelegate.m
//  AppDemo
//

#import "HockeduAppDelegate.h"
#import "MenuViewController.h"
#import "EAGLViewController.h"
#import "LoadMapViewController.h"

@implementation HockeduAppDelegate

@synthesize window = _window;
@synthesize menuViewController = _menuViewController;
@synthesize eaglViewController = _eaglViewController;

- (void) afficherVueAnimee
{    
    self.window.rootViewController = self.eaglViewController;
    self.menuViewController->mainMenuMiddleView.hidden = YES;
}

- (void) showFieldSaved:(NSString*) mapName : (NSString*) mapDescription : (int) mapPublic
{
    [self.eaglViewController->mModel setMapFields:mapName :mapDescription :mapPublic];
    
    [self afficherVueAnimee];
    [self.eaglViewController->mModel saveField];
}

- (void) showMenu
{

    self.window.rootViewController = self.menuViewController;
}

- (void) showMenuLoad
{
    self.menuViewController->loadMapView.hidden = NO;
    self.menuViewController->mainMenuView.hidden = YES;
    self.menuViewController->saveMapView.hidden = YES;
    self.menuViewController->mainMenuLoginView.hidden = YES;
    self.menuViewController->mainMenuMiddleView.hidden = YES;
    [self showMenu];
}

- (void) showMenuSave
{
    self.menuViewController->saveMapView.hidden = NO;
    self.menuViewController->mainMenuView.hidden = YES;
    self.menuViewController->mainMenuLoginView.hidden = YES;
    self.menuViewController->mainMenuMiddleView.hidden = YES;
    self.menuViewController->loadMapView.hidden = YES;
    [self showMenu];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    self.window.rootViewController = self.menuViewController;
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
}

- (void)dealloc
{
    [_window release];
    [_menuViewController release];
    [_eaglViewController release];
    [super dealloc];
}

@end
