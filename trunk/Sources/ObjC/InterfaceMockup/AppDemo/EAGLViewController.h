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

@interface CarouselElement : NSObject
{
    NSString* LabelValue;
    NSString* ImageName;
    @public
    EditorStateName ModifType;
}

@property (readwrite, nonatomic, retain) NSString* LabelValue;
@property (readwrite, nonatomic, retain) NSString* ImageName;

@end

@interface EAGLViewController : UIViewController <UIGestureRecognizerDelegate, iCarouselDataSource, iCarouselDelegate> {
    EAGLContext *context;
    EAGLView* theEAGLView;
    EventManager* mEventManager;
    GLuint program;
    IBOutlet UIView *mGLView;
    IBOutlet UIView *mSideBarView;
    IBOutlet UIView *mTopBarView;
    IBOutlet UIView *undoRedoView;
    IBOutlet UIView *applyView;
    IBOutlet UIImageView *mPropertyView;
    IBOutlet UIView *mTablePropertyView;
    IBOutlet UIView *mBoostPropertyView;
    IBOutlet UIView *mPortalPropertyView;
    IBOutlet UIView *mPuckPropertyView;
    IBOutlet UIView *mMalletPropertyView;
    IBOutlet UIView *mWallPropertyView;
    IBOutlet UIView *mControlPointPropertyView;
    IBOutlet UIView *mBonusPropertyView;
    
    IBOutlet UIButton *editionButton;
    IBOutlet UIButton *cameraButton;
    IBOutlet UIButton *deleteButton;
    IBOutlet UIButton *duplicateButton;
    IBOutlet UIButton *scaleButton;
    IBOutlet UIButton *rotationButton;
    IBOutlet UIButton *moveButton;
    IBOutlet UIButton *selectButton;
    IBOutlet UIButton *saveButton;
    IBOutlet UIButton *leftArrowButton;
    IBOutlet UIButton *rightArrowButton;
    IBOutlet UIButton *applyButton;
    
    NSArray* carouselElements;
    NSArray* textBoxCollection;
    NSArray* sliderCollection;
    NSArray* stepperCollection;
    
    UIButton *previouslySelected;
    
    IBOutlet UIImageView *carouselBackground;
    UIImage *carouselBackgroundImage;
    UIImage *carouselBackgroundSelected;
    UIImage *buttonImage;
    UIImage *buttonImageHighlight;
    UIImage *buttonImagePressed;
    
    BOOL animating;
    BOOL propertyBarHidden;
    
    NSInteger animationFrameInterval;
    CADisplayLink *displayLink;
    
    Model* mModel;
    
    //OpenGLWaveFrontObject *cube;
    
    float translationX;
    float translationY;
    float zoomFactor;
    
    // Utilise pour le pie menu
    PieMenu *pieMenu;
	UILabel *labelPieMenu;
    //
    
    // Pinch et zoom
    CGFloat __scale;
    CGFloat __previousScale;
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
@property (readonly, nonatomic, retain) NSArray* carouselElements;
@property (nonatomic, retain) IBOutletCollection(UITextField) NSArray* textBoxCollection;
@property (nonatomic, retain) IBOutletCollection(UISlider) NSArray* sliderCollection;
@property (nonatomic, retain) IBOutletCollection(UIStepper) NSArray* stepperCollection;


- (IBAction)cameraModeButtonTouched:(UIButton *)sender;
- (IBAction)leftArrowButton:(UIButton *)sender;
- (IBAction)rightArrowButton:(UIButton *)sender;
- (IBAction)selectToolButtonTouched:(UIButton *)sender;
- (IBAction)moveToolButtonTouched:(UIButton *)sender;
- (IBAction)rotationToolButtonTouched:(UIButton *)sender;
- (IBAction)scaleToolButtonTouched:(UIButton *)sender;
- (IBAction)duplicateToolButtonTouched:(UIButton *)sender;
- (IBAction)deleteToolButtonTouched:(UIButton *)sender;
- (IBAction)editorModeButtonTouched:(UIButton *)sender;
- (IBAction)saveAndExitButtonTouched:(UIButton *)sender;
- (IBAction)portalButtonTouched:(UIButton *)sender;
- (IBAction)undoButtonTouched:(UIButton *)sender;
- (IBAction)redoButtonTouched:(UIButton *)sender;
- (IBAction)applyButtonTouched:(UIButton *)sender;
- (void)refreshProperty;
- (void) propertiesMenuButtonTouched:(PieMenuItem *)item;
- (void)unselectAllTools;
- (void)carouselSelectItem:(NSInteger)index;
- (void)pressButtonUI:(UIButton *)sender;
- (void)startAnimation;
- (void)stopAnimation;
- (void)setupView;
- (CGPoint)convertScreenCoordToVirtualCoord:(CGPoint)pointToConvert;
- (void)rotationDetectee:(UIGestureRecognizer *)gestureRecognizer;
- (IBAction)longPressDetected:(UILongPressGestureRecognizer*)sender;
- (IBAction)swipeDetected:(id)sender;
- (IBAction)panDetected:(UIPanGestureRecognizer*)sender;
- (IBAction) sliderValueChanged:(UISlider*)sender;
- (IBAction) textFieldValueChanged:(UITextField*)sender;
- (IBAction) textFieldValueChangedStepper:(UITextField*)sender;
- (IBAction) stepperValueChanged:(UIStepper*)sender;
- (void) enablePuckCreation;
- (void)disablePuckCreation;
- (void)enableMalletCreation;
- (void)disableMalletCreation;
- (void)thereAreNodesSelected;
- (void)thereAreNoNodesSelected;
- (void)canUndo;
- (void)cannotUndo;
- (void)canRedo;
- (void)cannotRedo;
@end
