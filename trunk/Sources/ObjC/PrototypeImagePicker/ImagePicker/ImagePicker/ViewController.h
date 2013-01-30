//
//  ViewController.h
//  ImagePicker
//
//  Created by Samuel Ledoux on 2013-01-30.
//  Copyright (c) 2013 RazorSharp Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ViewController : UIViewController<UINavigationControllerDelegate, UIImagePickerControllerDelegate, UIPopoverControllerDelegate>
{
    
UIImagePickerController *picker;
UIPopoverController *popOverController;

IBOutlet UIImageView * selectedImage;

}

@property (nonatomic, retain) UIImageView * selectedImage;

- (IBAction) buttonClicked;

@end
