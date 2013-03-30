//
//  EventManager.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EventManager.h"

@implementation EventManager

@synthesize mCurrentState = _mCurrentState;
@synthesize mModel = _mModel;

-(EventManager*) init:(Model*)model
{
    _mModel = model;
    _mCurrentState = [[EditorStateSelection alloc]init]; // Etat par defaut en ouvrant l'editeur
    return self;
}

- (void)dealloc {
    [_mModel release];
    [_mCurrentState release];
    [super dealloc];
}

-(void)touchesBegan:(UITouch *)touch:(CGPoint)coordVirt
{
    [_mCurrentState touchesBegan:touch:coordVirt];
}
-(void)touchesMoved:(UITouch *)touch:(CGPoint)coordVirt
{
    [_mCurrentState touchesMoved:touch:coordVirt];
}
-(void)touchesEnded:(UITouch *)touch:(CGPoint)coordVirt
{
    [_mCurrentState touchesEnded:touch:coordVirt];
}

-(void) modifyState:(EditorStateName)editorState
{
    if(_mCurrentState!=nil)
    {
        [_mCurrentState release];
    }
    switch(editorState)
    {
        case EDITOR_STATE_TRANSFORMATION_ROTATION : _mCurrentState = [[EditorStateTransform alloc] init:FIELD_MODIFICATION_ROTATE];
            break;
        case EDITOR_STATE_TRANSFORMATION_ECHELLE : _mCurrentState = [[EditorStateTransform alloc] init:FIELD_MODIFICATION_SCALE];
            break;
        case EDITOR_STATE_TRANSFORMATION_DEPLACEMENT : _mCurrentState = [[EditorStateTransform alloc] init:FIELD_MODIFICATION_MOVE];
            break;
        case EDITOR_STATE_SELECTION : _mCurrentState = [[EditorStateSelection alloc] init];
            break;
        case EDITOR_STATE_AJOUTER_PORTAIL : _mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_PORTAL];
            break;
        case EDITOR_STATE_AJOUTER_MURET : _mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_WALL];
            break;
        case EDITOR_STATE_AJOUTER_MAILLET : _mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_MALLET];
            break;
        case EDITOR_STATE_AJOUTER_RONDELLE : _mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_PUCK];
            break;
        case EDITOR_STATE_AJOUTER_ACCELERATEUR : _mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_BOOST];
            break;
        case EDITOR_STATE_AJOUTER_BONUS : _mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_BONUS];
            break;
        case EDITOR_STATE_MOVE_WINDOW : _mCurrentState = [[EditorStateView alloc]init];
            break;
        case EDITOR_STATE_ZOOM_PROPORTIONNEL : _mCurrentState = [[EditorStateView alloc]init];
            break;
        default: break;
    }
}



@end
