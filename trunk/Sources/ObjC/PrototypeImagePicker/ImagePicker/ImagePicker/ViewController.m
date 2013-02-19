//
//  ViewController.m
//  ImagePicker
//
//  Created by Samuel Ledoux on 2013-01-30.
//  Modified by Mathieu M-Gosselin on 2013-02-05.
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

- (void)actionSheet:(UIActionSheet *)actionSheet didDismissWithButtonIndex:(NSInteger)buttonIndex
{      
    //picker.delegate = self;
    UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
    
    switch( buttonIndex )
    {
        case 0:
            // Use camera.
            [imagePicker setSourceType:UIImagePickerControllerSourceTypeCamera];
            
            [imagePicker setDelegate:self];
            
            [self presentViewController:imagePicker animated:YES completion:nil];
            break;
        
        case 1:
            // Use library.
            [imagePicker setSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
            
            popOverController = [[UIPopoverController alloc] initWithContentViewController:imagePicker];
            popOverController.delegate = self;

            [popOverController presentPopoverFromRect:CGRectMake(150.0, 300.0, 450.0, 300.0)
                                     inView:self.view
                   permittedArrowDirections:UIPopoverArrowDirectionAny
                                   animated:YES];
            
            [imagePicker setDelegate:self];
            break;
    }
}


-(IBAction) buttonClicked{
    
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
    {       
        sheet = [[UIActionSheet alloc] initWithTitle:@"Select a picture source"
                                        delegate:self
                               cancelButtonTitle:@"Cancel"
                          destructiveButtonTitle:nil
                               otherButtonTitles:@"Take a new picture", @"Browse my photo library", nil];
        // Show the sheet
        [sheet showInView:self.view];
    } else
    {
        UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
        
        [imagePicker setSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
        
        popOverController = [[UIPopoverController alloc] initWithContentViewController:imagePicker];
        popOverController.delegate = self;
        
        [popOverController presentPopoverFromRect:CGRectMake(150.0, 300.0, 450.0, 300.0)
                                           inView:self.view
                         permittedArrowDirections:0
                                         animated:YES];
        
        [imagePicker setDelegate:self];
    }
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    // Inspired by: http://stackoverflow.com/questions/8564833/ios-upload-image-and-text-using-http-post
    
    // Code here to work with media
    // Get the image from the result
    UIImage* image = [info valueForKey:@"UIImagePickerControllerOriginalImage"];
    
    selectedImage.image = image;
    
    // Dictionary that holds post parameters. You can set your post parameters that your server accepts or programmed to accept.
    NSMutableDictionary* _params = [[NSMutableDictionary alloc] init];
    
    [_params setObject:[NSString stringWithFormat:@"%d",image.imageOrientation] forKey:@"orientation"];
    //[_params setObject:[NSString stringWithString:@"en"] forKey:[NSString stringWithString:@"lan"]];
    //[_params setObject:[NSString stringWithFormat:@"%d", userId] forKey:[NSString stringWithString:@"userId"]];
    //[_params setObject:[NSString stringWithFormat:@"%@",title] forKey:[NSString stringWithString:@"title"]];
    
    
    // the boundary string : a random string, that will not repeat in post data, to separate post data fields.
    NSString *BoundaryConstant = @"----------V2ymHFg03ehbqgZCaKO6jy";
    
    // string constant for the post parameter 'file'. My server uses this name: `file`. Your's may differ
    NSString* FileParamConstant = @"myfile";
    
    // the server url to which the image (or the media) is uploaded. Use your server url here
    //NSURL* requestURL = [NSURL URLWithString:@"http://posttestserver.com/post.php"];
    NSURL* requestURL = [NSURL URLWithString:@"http://old.hockedu.com/proto.php"];
    
    
    // create request
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
    [request setCachePolicy:NSURLRequestReloadIgnoringLocalCacheData];
    [request setHTTPShouldHandleCookies:NO];
    [request setTimeoutInterval:300];
    [request setHTTPMethod:@"POST"];
    
    // set Content-Type in HTTP header
    NSString *contentType = [NSString stringWithFormat:@"multipart/form-data; boundary=%@", BoundaryConstant];
    [request setValue:contentType forHTTPHeaderField: @"Content-Type"];
    
    // post body
    NSMutableData *body = [NSMutableData data];
    
    // add params (all params are strings)
    for (NSString *param in _params) {
        [body appendData:[[NSString stringWithFormat:@"--%@\r\n", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"\r\n\r\n", param] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"%@\r\n", [_params objectForKey:param]] dataUsingEncoding:NSUTF8StringEncoding]];
    }
    
    // add image data
    NSData *imageData = UIImageJPEGRepresentation(image, 1.0);\
    
    NSLog(@"%d", image.imageOrientation);
    
    if (imageData) {
        [body appendData:[[NSString stringWithFormat:@"--%@\r\n", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"; filename=\"test.jpg\"\r\n", FileParamConstant] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[@"Content-Type: image/jpeg\r\n\r\n" dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:imageData];
        [body appendData:[[NSString stringWithFormat:@"\r\n"] dataUsingEncoding:NSUTF8StringEncoding]];
    }
    
    [body appendData:[[NSString stringWithFormat:@"--%@--\r\n", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];
    
    // setting the body of the post to the request
    [request setHTTPBody:body];
    
    // set the content-length
    NSString *postLength = [NSString stringWithFormat:@"%d", [body length]];
    [request setValue:postLength forHTTPHeaderField:@"Content-Length"];
    
    // set URL
    [request setURL:requestURL];
    
    NSData *returnData = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    NSString *returnString = [[NSString alloc] initWithData:returnData encoding:NSUTF8StringEncoding];
    
    NSLog(@"Image Return String: %@", returnString);
    
    
    // JSON parsing.
    NSError* error;
    NSDictionary* json = [NSJSONSerialization
                          JSONObjectWithData:returnData //1
                          
                          options:kNilOptions
                          error:&error];
    
    
    // We now have the URL of the uploaded image. Copy it to the clipboard.
    NSArray* imageLocation = [json objectForKey:@"location"]; //2
    
    UIPasteboard *pasteBoard = [UIPasteboard generalPasteboard];
    pasteBoard.string = [NSString stringWithFormat:@"%@", imageLocation];
    
    NSLog(@"location: %@", imageLocation); //3
    UIAlertView *successAlert = [[UIAlertView alloc] initWithTitle:@"Upload successful!"
                                                         message:[NSString stringWithFormat:@"Image location: %@\n\nThe address was copied to the clipboard for your convenience.", imageLocation]
                                                        delegate:nil
                                               cancelButtonTitle:@"OK"
                                               otherButtonTitles:nil];
    [successAlert show];
    
    [self dismissViewControllerAnimated:YES completion:nil];
}

-(void)imagePickerControllerDidCancel:
(UIImagePickerController *)picker
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end