//
//  ViewController.m
//  ImagePicker
//
//  Created by Samuel Ledoux on 2013-01-30.
//  Copyright (c) 2013 RazorSharp Technologies. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize selectedImage;

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(IBAction) buttonClicked{
    
    picker = [[UIImagePickerController alloc] init];
    picker.delegate = self;
        
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
        
    {
        
        picker.sourceType = UIImagePickerControllerSourceTypeCamera;
        
    } else
        
    {
        
        picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        
    }
    
    popOverController = [[UIPopoverController alloc] initWithContentViewController:picker];
    popOverController.delegate = self;

    
    //[self presentViewController:picker animated:YES completion:nil];
    [popOverController presentPopoverFromRect:CGRectMake(0.0, 0.0, 400.0, 200.0) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    // Code here to work with media
    // Get the image from the result
    UIImage* image = [info valueForKey:@"UIImagePickerControllerOriginalImage"];
    
    selectedImage.image = image;
    [self dismissViewControllerAnimated:YES completion:nil];
    
    // Get the data for the image as a PNG
    NSData* imageData = UIImagePNGRepresentation(image);
    
    // Give a name to the file
    NSString* imageName = @"MyImage.png";
    
    // Now, we have to find the documents directory so we can save it
    // Note that you might want to save it elsewhere, like the cache directory,
    // or something similar.
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* documentsDirectory = [paths objectAtIndex:0];
    
    // Now we get the full path to the file
    NSString* fullPathToFile = [documentsDirectory stringByAppendingPathComponent:imageName];
    
    // and then we write it out
    [imageData writeToFile:fullPathToFile atomically:NO];
    
    
    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:fullPathToFile];
    
    
    if(fileExists)
    {
    UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error"
                                                         message:@"Oui"
                                                        delegate:nil
                                               cancelButtonTitle:@"OK"
                                               otherButtonTitles:nil];
    [errorAlert show];
        }
    
}

-(void)imagePickerControllerDidCancel:
(UIImagePickerController *)picker
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end
