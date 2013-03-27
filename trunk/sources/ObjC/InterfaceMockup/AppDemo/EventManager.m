//
//  EventManager.m
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-25.
//
//

#import "EventManager.h"

@implementation EventManager

@synthesize mCurrentState;

-(EventManager*) init
{
    mCurrentState = [[EditorStateSelection alloc]init]; // Etat par defaut en ouvrant l'editeur
    return self;
}

-(void)touchesBegan:(UITouch *)touch
{
    [mCurrentState touchesBegan:touch];
}
-(void)touchesMoved:(UITouch *)touch
{
    [mCurrentState touchesMoved:touch];
}
-(void)touchesEnded:(UITouch *)touch
{
    [mCurrentState touchesEnded:touch];
}

-(void) modifyState:(EditorStateName)editorState
{
    if(mCurrentState!=nil)
    {
        [mCurrentState release];
    }
    switch(editorState)
    {
        case EDITOR_STATE_TRANSFORMATION_ROTATION : mCurrentState = [[EditorStateTransform alloc] init:FIELD_MODIFICATION_ROTATE];
            break;
        case EDITOR_STATE_TRANSFORMATION_ECHELLE : mCurrentState = [[EditorStateTransform alloc] init:FIELD_MODIFICATION_SCALE];
            break;
        case EDITOR_STATE_TRANSFORMATION_DEPLACEMENT : mCurrentState = [[EditorStateTransform alloc] init:FIELD_MODIFICATION_MOVE];
            break;
        case EDITOR_STATE_SELECTION : mCurrentState = [[EditorStateSelection alloc] init];
            break;
        case EDITOR_STATE_AJOUTER_PORTAIL : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_PORTAL];
            break;
        case EDITOR_STATE_AJOUTER_MURET : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_WALL];
            break;
        case EDITOR_STATE_AJOUTER_MAILLET : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_MALLET];
            break;
        case EDITOR_STATE_AJOUTER_RONDELLE : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_PUCK];
            break;
        case EDITOR_STATE_AJOUTER_ACCELERATEUR : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_BOOST];
            break;
        case EDITOR_STATE_AJOUTER_BONUS : mCurrentState = [[EditorStateAdd alloc] init:FIELD_MODIFICATION_ADD_BONUS];
            break;
        case EDITOR_STATE_MOVE_WINDOW : mCurrentState = [[EditorStateView alloc]init];
            break;
        case EDITOR_STATE_ZOOM_PROPORTIONNEL : mCurrentState = [[EditorStateView alloc]init];
            break;
        default: break;
    }
}

@end
