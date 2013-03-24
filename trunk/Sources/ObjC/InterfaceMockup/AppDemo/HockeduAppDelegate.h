//
//  HockeduDelegate.h
//  Hockedu
//

#import <UIKit/UIKit.h>

@class MenuViewController;
@class EAGLViewController;
@class LoadMapViewController;

@interface HockeduAppDelegate : NSObject <UIApplicationDelegate>

-(void) afficherVueAnimee;
-(void) showMapList;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet MenuViewController *menuViewController;
@property (nonatomic, retain) IBOutlet EAGLViewController *eaglViewController;
@property (nonatomic, retain) IBOutlet LoadMapViewController *loadMapViewController;

@end
