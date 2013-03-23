//
//  EAGLViewController.h
//  AppDemo
//

#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "OpenGLWaveFrontObject.h"
#import "Model.h"
extern int const LARGEUR_FENETRE;
extern int const HAUTEUR_FENETRE;

@class EAGLView;

@interface EAGLViewController : UIViewController <UIGestureRecognizerDelegate> {
    EAGLContext *context;
    EAGLView* theEAGLView;
    UIImageView *imageObjectToAdd;
    GLuint program;
    IBOutlet UIView *mGLView;
    IBOutlet UIView *mSideBarView;
    IBOutlet UIView *mTopBarView;
    
    BOOL mSelectionMode;
    BOOL mCreationMode;
    BOOL animating;
    NSInteger itemToBeAdded;
    
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
    
    //OpenGLWaveFrontObject *cube;
    Model* mModel;
    float translationX;
    float translationY;
    float zoomFactor;
    CGPoint firstCorner;
    BOOL touchMoved;
}

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;
@property (nonatomic, retain) OpenGLWaveFrontObject *cube;

typedef enum{
    PORTAL,
    PUCK,
    MALLET,
    WALL,
    ACCELERATOR,
    BONUS
} ItemToAdd;

- (IBAction)selectionModeButtonTouched:(UIButton *)sender;
- (IBAction)creationModeButtonTouched:(UIButton *)sender;
- (IBAction)saveAndExitButtonTouched:(UIButton *)sender;
- (IBAction)portalButtonTouched:(UIButton *)sender;
- (void)startAnimation;
- (void)stopAnimation;
- (void)setupView;
- (CGPoint)convertScreenCoordToVirtualCoord:(CGPoint)pointToConvert;
- (void)rotationDetectee:(UIGestureRecognizer *)gestureRecognizer;

@end
