//
//  MenuViewController.h
//  AppDemo
//

#import <UIKit/UIKit.h>

@interface MenuViewController : UIViewController
{
    IBOutlet UIButton *newMapButton;
    IBOutlet UIButton *loadMapButton;
    IBOutlet UIButton *settingsButton;
    IBOutlet UIButton *signInButton;
    
    
    IBOutlet UIView *mainMenuView;
    IBOutlet UIView *loadMapView;
}

-(IBAction) showEditor;
-(IBAction) openMapList;
@end
