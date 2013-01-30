//
//  AppDemoAppDelegate.h
//  AppDemo
//

#import <UIKit/UIKit.h>

@class MenuViewController;
@class EAGLViewController;
@class RougeViewController;

@interface AppDemoAppDelegate : NSObject <UIApplicationDelegate>

-(void) afficherVueAnimee;
-(void) afficherVueRouge;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet MenuViewController *menuViewController;
@property (nonatomic, retain) IBOutlet EAGLViewController *eaglViewController;
@property (nonatomic, retain) IBOutlet RougeViewController *rougeViewController;

@end
