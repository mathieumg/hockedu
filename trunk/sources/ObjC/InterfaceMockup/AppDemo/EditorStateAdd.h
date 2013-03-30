//
//  EditorStateAdd.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import <Foundation/Foundation.h>
#import "EditorStateAbstract.h"
#import "Enum_Declarations.h"
#import "Model.h"
@interface EditorStateAdd : EditorStateAbstract
-(EditorStateAdd*) init:(FieldModificationStrategyType)type;
-(void)touchesBegan:(UITouch *)touch :(CGPoint)coordVirt :(Model*)model;
-(void)touchesMoved:(UITouch *)touch :(CGPoint)coordVirt :(Model*)model;
-(void)touchesEnded:(UITouch *)touch :(CGPoint)coordVirt :(Model*)model;
@end
