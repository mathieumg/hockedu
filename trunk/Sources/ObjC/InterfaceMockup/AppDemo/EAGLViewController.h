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
#import "EventManager.h"
#import "iCarousel.h"
#import "PieMenu.h"

extern int const LARGEUR_FENETRE;
extern int const HAUTEUR_FENETRE;

@class EAGLView;



@interface EAGLViewController : UIViewController <UIGestureRecognizerDelegate, iCarouselDataSource, iCarouselDelegate> {
    EAGLContext *context;
    EAGLView* theEAGLView;
    EventManager* mEventManager;
    GLuint program;
    IBOutlet UIView *mGLView;
    IBOutlet UIView *mSideBarView;
    IBOutlet UIView *mTopBarView;
    
    IBOutlet UIButton *editionButton;
    IBOutlet UIButton *cameraButton;
    IBOutlet UIButton *deleteButton;
    IBOutlet UIButton *duplicateButton;
    IBOutlet UIButton *scaleButton;
    IBOutlet UIButton *rotationButton;
    IBOutlet UIButton *moveButton;
    IBOutlet UIButton *selectButton;
    IBOutlet UIButton *saveButton;
    // Used to know if we are selecting or creating
    BOOL mSelectionMode;
    BOOL mCreationMode;
    BOOL mSelectTool;
    BOOL mMoveTool;
    UIImageView *imageObjectToAdd;
    NSInteger itemToBeAdded;
    
    BOOL animating;
    
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
    
    Model* mModel;
    
    //OpenGLWaveFrontObject *cube;
    
    float translationX;
    float translationY;
    float zoomFactor;
    // Utilise pour le rectangle de selection
    CGPoint firstCorner;
    BOOL touchMoved;
    
    // Utilise pour le pie menu
    PieMenu *pieMenu;
	UILabel *labelPieMenu;
    //
}



// Utilise pour le pie menu
@property (nonatomic, retain) PieMenu *pieMenu;
@property (nonatomic, retain) IBOutlet UILabel *labelPieMenu;

- (IBAction) fingerSizeAction:(id)sender;
- (IBAction) leftHandedAction:(id)sender;
//

@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) NSInteger animationFrameInterval;
@property (nonatomic, retain) OpenGLWaveFrontObject *cube;
@property (nonatomic, retain) IBOutlet iCarousel *carousel;

typedef enum{
    PORTAL,
    PUCK,
    MALLET,
    WALL,
    ACCELERATOR,
    BONUS
} ItemToAdd;

- (IBAction)selectionModeButtonTouched:(UIButton *)sender;
- (IBAction)selectToolButtonTouched:(UIButton *)sender;
- (IBAction)moveToolButtonTouched:(UIButton *)sender;
- (IBAction)rotationToolButtonTouched:(UIButton *)sender;
- (IBAction)scaleToolButtonTouched:(UIButton *)sender;
- (IBAction)duplicateToolButtonTouched:(UIButton *)sender;
- (IBAction)deleteToolButtonTouched:(UIButton *)sender;
- (IBAction)creationModeButtonTouched:(UIButton *)sender;
- (IBAction)saveAndExitButtonTouched:(UIButton *)sender;
- (IBAction)portalButtonTouched:(UIButton *)sender;
- (void)unselectAllTools;
- (void)startAnimation;
- (void)stopAnimation;
- (void)setupView;
- (CGPoint)convertScreenCoordToVirtualCoord:(CGPoint)pointToConvert;
- (void)rotationDetectee:(UIGestureRecognizer *)gestureRecognizer;
- (IBAction)longPressDetected:(UILongPressGestureRecognizer*)sender;

@end
