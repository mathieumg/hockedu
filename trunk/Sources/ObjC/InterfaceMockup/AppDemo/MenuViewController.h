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
    
    IBOutlet UIButton *backMainMenuButton;
    IBOutlet UIButton *backEditorButton;
    IBOutlet UIButton *saveMapButton;
    IBOutlet UIButton *discardMainMenuButton;
    IBOutlet UISwitch *switchMapPublic;
    IBOutlet UITextView *textMapDescription;
    IBOutlet UITextField *textMapName;
    
    @public
    IBOutlet UIView *mainMenuView;
    IBOutlet UIView *loadMapView;
    IBOutlet UIView *saveMapView;
    
    NSArray* _userMaps;
    
    NSInteger userId;
    NSInteger mapId;
}

@property (retain,nonatomic) IBOutlet UITableView *tableView;
@property (retain) NSArray* _userMaps;
@property NSInteger userId;
@property NSInteger mapId;

- (IBAction)touchBackMainMenuButton:(UIButton *)sender;
- (IBAction)touchBackEditorButton:(UIButton *)sender;
- (IBAction)touchSaveMapButton:(UIButton *)sender;
- (IBAction)touchDiscardMainMenuButton:(UIButton *)sender;
- (IBAction)loadMapButton:(UIButton *)sender;
-(IBAction) showEditor;
@end
