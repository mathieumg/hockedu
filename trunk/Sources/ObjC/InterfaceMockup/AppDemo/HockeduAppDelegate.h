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
-(void) showMenu;
-(void) showMenuLoad;
-(void) showMenuSave;
-(void) showFieldSaved:(NSString*) mapName : (NSString*) mapDescription : (int) mapPublic;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet MenuViewController *menuViewController;
@property (nonatomic, retain) IBOutlet EAGLViewController *eaglViewController;

@end
