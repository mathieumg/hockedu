//
//  EditorStateAbstract.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import <Foundation/Foundation.h>
#import "Model.h"
@interface EditorStateAbstract : NSObject
-(void)touchesBegan:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model;
-(void)touchesMoved:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model;
-(void)touchesEnded:(UITouch *)touch:(CGPoint)coordVirt:(Model*)model;
@end
