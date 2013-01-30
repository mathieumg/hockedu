//
//  MenuViewController.h
//  AppDemo
//

#import <UIKit/UIKit.h>

@interface MenuViewController : UIViewController
{
    IBOutlet UIButton *boutonGenerateur;
}

-(IBAction) afficherVueAnimee;
-(IBAction) afficherVueRouge;
-(IBAction) genererValeur;

@property(nonatomic,retain) UIButton *boutonGenerateur;

@end
