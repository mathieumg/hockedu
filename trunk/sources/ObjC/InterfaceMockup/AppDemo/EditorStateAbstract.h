//
//  EditorStateAbstract.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import <Foundation/Foundation.h>

@interface EditorStateAbstract : NSObject
-(void)touchesBegan:(UITouch *)touch;
-(void)touchesMoved:(UITouch *)touch;
-(void)touchesEnded:(UITouch *)touch;
@end
