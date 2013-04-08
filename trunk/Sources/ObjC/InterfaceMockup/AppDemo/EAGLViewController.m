//
//  EAGLViewController.m
//  AppDemo
//

#import <QuartzCore/QuartzCore.h>

#import "EAGLViewController.h"
#import "EAGLView.h"
#define degreesToRadian(x) (M_PI * (x) / 180.0)
#import "Enum_Declarations.h"
#import "Facade.h"
int const LARGEUR_FENETRE = 1024;
int const HAUTEUR_FENETRE = 768;
// Uniform index.
enum {
    UNIFORM_TRANSLATE,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum {
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};


@implementation CarouselElement

@synthesize LabelValue;
@synthesize ImageName;

@end

@interface EAGLViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, retain) EventManager* mEventManager;
@property (nonatomic, retain) EAGLView *theEAGLView;
@property (nonatomic, retain) IBOutlet UIView *mGLView;
@property (retain, nonatomic) IBOutlet UIView *mSideBarView;
@property (retain, nonatomic) IBOutlet UIView *mTopBarView;
@property (retain, nonatomic) IBOutlet UIView *undoRedoView;

// View pour le panel de modification des proprietes
@property (retain, nonatomic) IBOutlet UIImageView *mPropertyView;
@property (retain, nonatomic) IBOutlet UIView *mTablePropertyView;
@property (retain, nonatomic) IBOutlet UIView *mBoostPropertyView;
@property (retain, nonatomic) IBOutlet UIView *mPortalPropertyView;
@property (retain, nonatomic) IBOutlet UIView *mPuckPropertyView;
@property (retain, nonatomic) IBOutlet UIView *mMalletPropertyView;
@property (retain, nonatomic) IBOutlet UIView *mWallPropertyView;
@property (retain, nonatomic) IBOutlet UIView *mControlPointPropertyView;
@property (retain, nonatomic) IBOutlet UIView *mBonusPropertyView;
@property (retain, nonatomic) IBOutlet UIView *applyView;


@property (nonatomic, assign) CADisplayLink *displayLink;
@property (nonatomic, assign) BOOL wrap;
@property (nonatomic, assign) BOOL clipsToBounds;
@property (nonatomic, retain) NSMutableArray *items;
- (void) itemSelected:(PieMenuItem *)item;
- (void) setupPieMenu;
@end

@implementation EAGLViewController

@synthesize animating;
@synthesize theEAGLView;
@synthesize mSideBarView;
@synthesize mTopBarView;
@synthesize mGLView;

// Panel de modification des proprietes
@synthesize mPropertyView;
@synthesize mTablePropertyView;
@synthesize mBoostPropertyView;
@synthesize mPortalPropertyView;
@synthesize mPuckPropertyView;
@synthesize mMalletPropertyView;
@synthesize mWallPropertyView;
@synthesize mControlPointPropertyView;
@synthesize mBonusPropertyView;
@synthesize applyView;

@synthesize undoRedoView;
@synthesize context;
@synthesize displayLink;
@synthesize cube;
@synthesize mEventManager;
@synthesize carousel;
@synthesize items;
@synthesize carouselElements;
@synthesize textBoxCollection;
@synthesize sliderCollection;
@synthesize stepperCollection;
// Pie menu
@synthesize pieMenu;
@synthesize labelPieMenu;

- (void)awakeFromNib
{
    self.items = [NSMutableArray array];
    for (int i = 0; i < 6; i++)
    {
        [items addObject:[NSNumber numberWithInt:i]];
    }
    
    [self.view init];
    theEAGLView = [[EAGLView alloc] initWithFrame:CGRectMake(0.0, 0.0, self.mGLView.bounds.size.height, self.mGLView.bounds.size.width)];
    
    if (!theEAGLView)
        NSLog(@"Failed to create ES view");
    
    theEAGLView.opaque = YES;
    
    mModel = [[Model alloc]init];
    [mModel resizeWindow:0 :0 :LARGEUR_FENETRE :HAUTEUR_FENETRE];
    mEventManager = [[EventManager alloc]init:mModel:self];
    translationX = 0.0;
    translationY = 0.0;
    zoomFactor = 0.5;
    
    [self.mGLView addSubview:theEAGLView];
    [self.mGLView addSubview:mSideBarView];
    [self.mGLView addSubview:mTopBarView];
    [self.mGLView addSubview:undoRedoView];
    [self.mGLView addSubview:mPropertyView];
    [self.theEAGLView setFramebuffer];
    
    [Facade registerController:self];
    
    // On cache la bar en dehors a droite
    self.mPropertyView.center = CGPointMake(mPropertyView.center.x + mPropertyView.bounds.size.width, mPropertyView.center.y);
    propertyBarHidden = YES;
    
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
    
    buttonImage = [[UIImage imageNamed:@"blueButton@2x.png"]
                   resizableImageWithCapInsets:UIEdgeInsetsMake(18, 18, 18, 18)];
    buttonImageHighlight = [[UIImage imageNamed:@"blueButtonHighlight@2x.png"]
                            resizableImageWithCapInsets:UIEdgeInsetsMake(18, 18, 18, 18)];
    
    buttonImagePressed = [[UIImage imageNamed:@"blueButtonPressed@2x.png"]
                          resizableImageWithCapInsets:UIEdgeInsetsMake(18, 18, 18, 18)];
    
    carouselBackgroundImage = [UIImage imageNamed:@"carouselBackground.png"];
    carouselBackgroundSelected = [UIImage imageNamed:@"carouselBackgroundHighlight.png"];
    
    // Set the background for any states you plan to use
    [saveButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [saveButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [selectButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [selectButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [moveButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [moveButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [rotationButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [rotationButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [scaleButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [scaleButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [duplicateButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [duplicateButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [editionButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [editionButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [deleteButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [deleteButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [cameraButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [cameraButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    [applyButton setBackgroundImage:buttonImage forState:UIControlStateNormal];
    [applyButton setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    
    
    
    CarouselElement *mailletCarousel = [[CarouselElement alloc] init];
    mailletCarousel->ModifType=EDITOR_STATE_AJOUTER_MAILLET;
    mailletCarousel.LabelValue = @"Mallet";
    mailletCarousel.ImageName = @"outil_maillet2";
    
    CarouselElement *rondelleCarousel = [[CarouselElement alloc] init];
    rondelleCarousel->ModifType=EDITOR_STATE_AJOUTER_RONDELLE;
    rondelleCarousel.LabelValue = @"Puck";
    rondelleCarousel.ImageName = @"outil_rondelle";
    
    CarouselElement *muretCarousel = [[CarouselElement alloc] init];
    muretCarousel->ModifType=EDITOR_STATE_AJOUTER_MURET;
    muretCarousel.LabelValue = @"Wall";
    muretCarousel.ImageName = @"outil_muret";
    
    CarouselElement *accelerateurCarousel = [[CarouselElement alloc] init];
    accelerateurCarousel->ModifType=EDITOR_STATE_AJOUTER_ACCELERATEUR;
    accelerateurCarousel.LabelValue = @"Booster";
    accelerateurCarousel.ImageName = @"outil_accelerateur";
    
    CarouselElement *portailCarousel = [[CarouselElement alloc] init];
    portailCarousel->ModifType=EDITOR_STATE_AJOUTER_PORTAIL;
    portailCarousel.LabelValue = @"Portal";
    portailCarousel.ImageName = @"outil_portail";
    
    CarouselElement *bonusCarousel = [[CarouselElement alloc] init];
    bonusCarousel->ModifType=EDITOR_STATE_AJOUTER_BONUS;
    bonusCarousel.LabelValue = @"Bonus";
    bonusCarousel.ImageName = @"outil_bonus";
    
    carouselElements = [[NSArray alloc] initWithObjects:mailletCarousel,rondelleCarousel,muretCarousel,accelerateurCarousel,portailCarousel,bonusCarousel,nil];
    [self pressButtonUI:selectButton];
    
    [buttonImage retain];
    [buttonImageHighlight retain];
    [buttonImagePressed retain];
    
}

- (void)dealloc
{
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    [mEventManager release];
    [mGLView release];
    [mSideBarView release];
    [mTopBarView release];
    carousel.delegate = nil;
    carousel.dataSource = nil;
    
    [carousel release];
    [items release];
    
    //[saveExitButton release];
    [saveButton release];
    [selectButton release];
    [moveButton release];
    [rotationButton release];
    [scaleButton release];
    [duplicateButton release];
    [deleteButton release];
    [cameraButton release];
    [editionButton release];
    
    // View pour le panel de modif
    [mPropertyView release];
    [mTablePropertyView release];
    [mBoostPropertyView release];
    [mPortalPropertyView release];
    [mPuckPropertyView release];
    [mMalletPropertyView release];
    [mWallPropertyView release];
    [mControlPointPropertyView release];
    [mBonusPropertyView release];
    
    [leftArrowButton release];
    [rightArrowButton release];
    [carouselBackground release];
    [applyView release];
    [applyButton release];
    [super dealloc];
}

//With this and the next method, we only allow the landscaperight orientation when on this view
-(BOOL)shouldAutorotate {
    return YES;
}
- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [mPropertyView  addSubview:mTablePropertyView];
    
    // SETUP DES GESTURES
    UIRotationGestureRecognizer *rotationGesture = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotationDetectee:)];
    UILongPressGestureRecognizer *longPressGesture = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPressDetected:)];
    //UILongPressGestureRecognizer *longPressGesture = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTapDetected:)];
    
    UIPinchGestureRecognizer *pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchDetected:)];
    
    UISwipeGestureRecognizer *swipeGesture = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeDetected:)];
    
    [rotationGesture setDelegate:self];
    //[longPressGesture setDelegate:self];
    //[longPressGesture setNumberOfTapsRequired:2];
    [longPressGesture setDelegate:self];
    //[longPressGesture setCancelsTouchesInView:YES];
    [pinchGesture setDelegate:self];
    [swipeGesture setDelegate:self];
    
    [mGLView addGestureRecognizer:rotationGesture];
    //[mGLView addGestureRecognizer:longPressGesture];
    [mGLView addGestureRecognizer:longPressGesture];
    [mGLView addGestureRecognizer:pinchGesture];
    [carousel addGestureRecognizer:swipeGesture];
    
    [rotationGesture release];
    //[longPressGesture release];
    [longPressGesture release];
    [pinchGesture release];
    [swipeGesture release];
    
    // FIN SETUP DES GESTURES
    
    // Set le carousel
    carousel.type = iCarouselTypeLinear;
    // Set le pie menu, voir methode
    [self setupPieMenu];
}

- (IBAction)undoButtonTouched:(UIButton *)sender
{
    [mModel undo];
}
- (IBAction)redoButtonTouched:(UIButton *)sender
{
    [mModel redo];
}

- (void) propertiesMenuButtonTouched:(PieMenuItem *)item {
    
    FullPropertiesApple prop;// = [[FullPropertiesApple alloc]init];
    [mModel getProperties: &prop];
    // On enleve lancienne view
    for(UIView* subview in [mPropertyView subviews])
    {
        [subview removeFromSuperview];
    }
    
    // On set la nouvelle view selon la selection
    switch ([mModel getSelectedNodesType]) {
        case RAZER_KEY_BONUS:
            [self refreshProperty];
            [mPropertyView addSubview:mBonusPropertyView];
            [mBonusPropertyView addSubview:applyView];
            applyView.center = CGPointMake( mBonusPropertyView.frame.size.width/2,mBonusPropertyView.frame.size.height - (applyView.frame.size.height/2) - 7);
            //[mBonusPropertyView setNeedsDisplay];
            
            break;
        case RAZER_KEY_BOOST:
            [self refreshProperty];
            [mPropertyView addSubview:mBoostPropertyView];
            [mBoostPropertyView addSubview:applyView];
            applyView.center = CGPointMake( mBoostPropertyView.frame.size.width/2,mBoostPropertyView.frame.size.height - (applyView.frame.size.height/2) - 3);
            break;
        case RAZER_KEY_CONTROL_POINT:
            // WALLS
            [self refreshProperty];
            [mPropertyView addSubview:mWallPropertyView];
            [mWallPropertyView addSubview:applyView];
            applyView.center = CGPointMake( mWallPropertyView.frame.size.width/2,mWallPropertyView.frame.size.height - (applyView.frame.size.height/2) - 3);
            break;
        case RAZER_KEY_MALLET:
            [self refreshProperty];
            [mPropertyView addSubview:mMalletPropertyView];
            [mMalletPropertyView addSubview:applyView];
            applyView.center = CGPointMake( mMalletPropertyView.frame.size.width/2,mMalletPropertyView.frame.size.height - (applyView.frame.size.height/2) - 3);
            break;
        case RAZER_KEY_PORTAL:
            [self refreshProperty];
            [mPropertyView addSubview:mPortalPropertyView];
            [mPortalPropertyView addSubview:applyView];
            applyView.center = CGPointMake( mPortalPropertyView.frame.size.width/2,mPortalPropertyView.frame.size.height - (applyView.frame.size.height/2) - 3);
            break;
        case RAZER_KEY_PUCK:
            [self refreshProperty];
            [mPropertyView addSubview:mPuckPropertyView];
            [mPuckPropertyView addSubview:applyView];
            applyView.center = CGPointMake( mPuckPropertyView.frame.size.width/2,mPuckPropertyView.frame.size.height - (applyView.frame.size.height/2) - 3);
            break;
        case RAZER_KEY_NONE:
            [self refreshProperty];
            [mPropertyView addSubview:mTablePropertyView];
            [mTablePropertyView addSubview:applyView];
            applyView.center = CGPointMake( mTablePropertyView.frame.size.width/2,mTablePropertyView.frame.size.height - (applyView.frame.size.height/2) - 7);
            break;
        case RAZER_KEY_TABLE_CONTROL_POINT:
            [self refreshProperty];
            [mPropertyView addSubview:mControlPointPropertyView];
            [mControlPointPropertyView addSubview:applyView];
            applyView.center = CGPointMake( mControlPointPropertyView.frame.size.width/2,mControlPointPropertyView.frame.size.height - (applyView.frame.size.height/2) - 3);
            break;
        default:
            break;
    }
    [UIView beginAnimations:@"MenuAnimationShow" context:NULL];
    [UIView setAnimationDuration:1];
    self.mPropertyView.center = CGPointMake(mPropertyView.center.x - mPropertyView.bounds.size.width, mPropertyView.center.y);
    [UIView commitAnimations];
    propertyBarHidden = NO;
    
}

- (IBAction) sliderValueChanged:(UISlider*)sender;
{
    float value = [sender value];
    NSString *stringValue = [NSString stringWithFormat:@"%.2f",value];
    
    
    
    for(UITextField *textField in textBoxCollection){
        if(textField.tag==sender.tag){
            [textField setText:stringValue];
        }
    }
}
- (IBAction) textFieldValueChanged:(UITextField*)sender
{
    
    float value = [sender.text floatValue];
    
    
    for(UISlider *slider in sliderCollection){
        if(sender.tag==slider.tag){
            [slider setValue:value];
        }
    }
}

- (IBAction) textFieldValueChangedStepper:(UITextField*)sender
{
    int value = [sender.text intValue];
    
    
    for(UIStepper *stepper in stepperCollection){
        if(sender.tag==stepper.tag){
            [stepper setValue:value];
        }
    }
}

- (IBAction) stepperValueChanged:(UIStepper*)sender
{
    int value = [sender value];
    NSString *stringValue = [NSString stringWithFormat:@"%d",value];
    
    
    
    for(UITextField *textField in textBoxCollection){
        if(textField.tag==sender.tag){
            [textField setText:stringValue];
        }
    }
}

- (void)refreshProperty
{
    FullPropertiesApple prop;
    [mModel getProperties: &prop];
    
    // On set la nouvelle view selon la selection
    switch ([mModel getSelectedNodesType]) {
        case RAZER_KEY_BONUS:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 39:
                        // Pos X
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionX];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 40:
                        // Pos Y
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionY];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 23:
                        // Scale
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mScale];
                        [self textFieldValueChanged:textField];
                        break;
                    case 24:
                        // Angle
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mAngle];
                        [self textFieldValueChanged:textField];
                        break;
                        
                    default:
                        break;
                }
            }
            
            break;
        case RAZER_KEY_BOOST:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 27:
                        // Pos X
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionX];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 28:
                        // Pos Y
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionY];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 10:
                        // Scale
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mScale];
                        [self textFieldValueChanged:textField];
                        break;
                    case 11:
                        // Accel
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mAcceleration];
                        [self textFieldValueChanged:textField];
                        break;
                    case 12:
                        // Angle
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mAngle];
                        [self textFieldValueChanged:textField];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_CONTROL_POINT:
            // WALLS
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 35:
                        // Pos X
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionX];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 36:
                        // Pos Y
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionY];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 20:
                        // Scale
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mScale];
                        [self textFieldValueChanged:textField];
                        break;
                    case 21:
                        // Angle
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mAngle];
                        [self textFieldValueChanged:textField];
                        break;
                    case 22:
                        // Rebound
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mRebound];
                        [self textFieldValueChanged:textField];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_MALLET:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 33:
                        // Pos X
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionX];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 34:
                        // Pos Y
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionY];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 18:
                        // Scale
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mScale];
                        [self textFieldValueChanged:textField];
                        break;
                    case 19:
                        // Angle
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mAngle];
                        [self textFieldValueChanged:textField];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_PORTAL:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 29:
                        // Pos X
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionX];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 30:
                        // Pos Y
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionY];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 13:
                        // Scale
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mScale];
                        [self textFieldValueChanged:textField];
                        break;
                    case 14:
                        // Attraction
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mAttraction];
                        [self textFieldValueChanged:textField];
                        break;
                    case 15:
                        // Angle
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mAngle];
                        [self textFieldValueChanged:textField];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_PUCK:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 31:
                        // Pos X
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionX];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 32:
                        // Pos Y
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionY];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 16:
                        // Scale
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mScale];
                        [self textFieldValueChanged:textField];
                        break;
                    case 17:
                        // Angle
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mAngle];
                        [self textFieldValueChanged:textField];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_NONE:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 0:
                        // Friction
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mFriction];
                        [self textFieldValueChanged:textField];
                        break;
                    case 25:
                        // Pos X
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mZoneEditionX];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 26:
                        // Pos Y
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mZoneEditionY];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 1:
                        // LeftTop
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mRinkRebound1];
                        [self textFieldValueChanged:textField];
                        break;
                    case 2:
                        // TopLeft
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mRinkRebound2];
                        [self textFieldValueChanged:textField];
                        break;
                    case 3:
                        // TopRight
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mRinkRebound3];
                        [self textFieldValueChanged:textField];
                        break;
                    case 4:
                        // RightTop
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mRinkRebound4];
                        [self textFieldValueChanged:textField];
                        break;
                    case 5:
                        // RightBot
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mRinkRebound5];
                        [self textFieldValueChanged:textField];
                        break;
                    case 6:
                        // BotRight
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mRinkRebound6];
                        [self textFieldValueChanged:textField];
                        break;
                    case 41:
                        // BotLeft
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mRinkRebound7];
                        [self textFieldValueChanged:textField];
                        break;
                    case 7:
                        // TopLeft
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mRinkRebound8];
                        [self textFieldValueChanged:textField];
                        break;
                    case 8:
                        // MinSpawn
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mMinBonusSpawnTime];
                        [self textFieldValueChanged:textField];
                        break;
                    case 9:
                        // TopLeft
                        textField.text = [NSString stringWithFormat:@"%.2f",prop.mMaxBonusSpawnTime];
                        [self textFieldValueChanged:textField];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_TABLE_CONTROL_POINT:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 37:
                        // Pos X
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionX];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    case 38:
                        // Pos Y
                        textField.text = [NSString stringWithFormat:@"%d",(int)prop.mPositionY];
                        [self textFieldValueChangedStepper:textField];
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }
}

- (IBAction)applyButtonTouched:(UIButton *)sender
{
    FullPropertiesApple prop;
    //[mModel getProperties: &prop];
    
    // On set la nouvelle view selon la selection
    switch ([mModel getSelectedNodesType]) {
        case RAZER_KEY_BONUS:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 39:
                        // Pos X
                        prop.mPositionX = [textField.text floatValue];
                        break;
                    case 40:
                        // Pos Y
                        prop.mPositionY = [textField.text floatValue];
                        break;
                    case 23:
                        // Scale
                        prop.mScale = [textField.text floatValue];
                        break;
                    case 24:
                        // Angle
                        prop.mAngle = [textField.text floatValue];
                        break;
                        
                    default:
                        break;
                }
            }
            
            break;
        case RAZER_KEY_BOOST:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 27:
                        // Pos X
                        prop.mPositionX = [textField.text floatValue];
                        break;
                    case 28:
                        // Pos Y
                        prop.mPositionY = [textField.text floatValue];
                        break;
                    case 10:
                        // Scale
                        prop.mScale = [textField.text floatValue];
                        break;
                    case 11:
                        // Accel
                        prop.mAcceleration = [textField.text floatValue];
                        break;
                    case 12:
                        // Angle
                        prop.mAngle = [textField.text floatValue];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_CONTROL_POINT:
            // WALLS
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 35:
                        // Pos X
                        prop.mPositionX = [textField.text floatValue];
                        break;
                    case 36:
                        // Pos Y
                        prop.mPositionY = [textField.text floatValue];
                        break;
                    case 20:
                        // Scale
                        prop.mScale = [textField.text floatValue];
                        break;
                    case 21:
                        // Angle
                        prop.mAngle = [textField.text floatValue];
                        break;
                    case 22:
                        // Rebound
                        prop.mRebound = [textField.text floatValue];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_MALLET:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 33:
                        // Pos X
                        prop.mPositionX = [textField.text floatValue];
                        break;
                    case 34:
                        // Pos Y
                        prop.mPositionY = [textField.text floatValue];
                        break;
                    case 18:
                        // Scale
                        prop.mScale = [textField.text floatValue];
                        break;
                    case 19:
                        // Angle
                        prop.mAngle = [textField.text floatValue];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_PORTAL:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 29:
                        // Pos X
                        prop.mPositionX = [textField.text floatValue];
                        break;
                    case 30:
                        // Pos Y
                        prop.mPositionY = [textField.text floatValue];
                        break;
                    case 13:
                        // Scale
                        prop.mScale = [textField.text floatValue];
                        break;
                    case 14:
                        // Attraction
                        prop.mAttraction = [textField.text floatValue];
                        break;
                    case 15:
                        // Angle
                        prop.mAngle = [textField.text floatValue];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_PUCK:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 31:
                        // Pos X
                        prop.mPositionX = [textField.text floatValue];
                        break;
                    case 32:
                        // Pos Y
                        prop.mPositionY = [textField.text floatValue];
                        break;
                    case 16:
                        // Scale
                        prop.mScale = [textField.text floatValue];
                        break;
                    case 17:
                        // Angle
                        prop.mAngle = [textField.text floatValue];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_NONE:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 0:
                        // Friction
                        prop.mFriction = [textField.text floatValue];
                        break;
                    case 25:
                        // Pos X
                        prop.mZoneEditionX = [textField.text floatValue];
                        break;
                    case 26:
                        // Pos Y
                        prop.mZoneEditionY = [textField.text floatValue];
                        break;
                    case 1:
                        // LeftTop
                        prop.mRinkRebound1 = [textField.text floatValue];
                        break;
                    case 2:
                        // TopLeft
                        prop.mRinkRebound2 = [textField.text floatValue];
                        break;
                    case 3:
                        // TopRight
                        prop.mRinkRebound3 = [textField.text floatValue];
                        break;
                    case 4:
                        // RightTop
                        prop.mRinkRebound4 = [textField.text floatValue];
                        break;
                    case 5:
                        // RightBot
                        prop.mRinkRebound5 = [textField.text floatValue];
                        break;
                    case 6:
                        // BotRight
                        prop.mRinkRebound6 = [textField.text floatValue];
                        break;
                    case 41:
                        // BotLeft
                        prop.mRinkRebound7 = [textField.text floatValue];
                        break;
                    case 7:
                        // TopLeft
                        prop.mRinkRebound8 = [textField.text floatValue];
                        break;
                    case 8:
                        // MinSpawn
                        prop.mMinBonusSpawnTime = [textField.text floatValue];
                        break;
                    case 9:
                        // TopLeft
                        prop.mMaxBonusSpawnTime = [textField.text floatValue];
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        case RAZER_KEY_TABLE_CONTROL_POINT:
            for(UITextField *textField in textBoxCollection){
                switch (textField.tag) {
                    case 37:
                        // Pos X
                        prop.mPositionX = [textField.text floatValue];
                        break;
                    case 38:
                        // Pos Y
                        prop.mPositionY = [textField.text floatValue];
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }
[mModel setProperties:&prop];
[self refreshProperty];
}

- (void) setupPieMenu
{
    // SETUP DU PIE MENU
	self.pieMenu = [[[PieMenu alloc] init] autorelease];
	PieMenuItem *itemA = [[PieMenuItem alloc] initWithTitle:@"Select"
													  label:nil
													 target:self
												   selector:@selector(selectToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"cursor_24x24.png"]];
    
	PieMenuItem *itemB = [[PieMenuItem alloc] initWithTitle:@"Move"
													  label:nil
													 target:self
												   selector:@selector(moveToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"move_alt2_24x24.png"]];
	
	PieMenuItem *itemC = [[PieMenuItem alloc] initWithTitle:@"Rotate"
													  label:nil
													 target:self
												   selector:@selector(rotationToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"curved_arrow_24x18.png"]];
	
	PieMenuItem *itemD = [[PieMenuItem alloc] initWithTitle:@"Scale"
													  label:nil
													 target:self
												   selector:@selector(scaleToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"fullscreen_24x24.png"]];
	
	
	PieMenuItem *itemE = [[PieMenuItem alloc] initWithTitle:@"Duplicate"
													  label:nil
													 target:self
												   selector:@selector(duplicateToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"fork_21x24.png"]];
	
	PieMenuItem *itemF = [[PieMenuItem alloc] initWithTitle:@"Delete"
													  label:nil
													 target:self
												   selector:@selector(deleteToolButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"x_alt_24x24.png"]];
	
	PieMenuItem *itemG = [[PieMenuItem alloc] initWithTitle:@"Properties"
													  label:nil
													 target:self
												   selector:@selector(propertiesMenuButtonTouched:)
												   userInfo:nil
													   icon:[UIImage imageNamed:@"cog_24x24.png"]];
	
	
	//[itemA addSubItem:itemE];
	//[itemA addSubItem:itemB];
	//[itemA addSubItem:itemD];
	[pieMenu addItem:itemA];
	[pieMenu addItem:itemB];
    [pieMenu addItem:itemC];
    [pieMenu addItem:itemD];
	[pieMenu addItem:itemE];
	[pieMenu addItem:itemF];
	[pieMenu addItem:itemG];
    
    [pieMenu setFingerSize:PieMenuFingerSizeBig];
	
	[itemA release];
	[itemB release];
	[itemC release];
	[itemD release];
	[itemE release];
	[itemF release];
	[itemG release];
}

#warning Fonction a mettre dans les settings!!!!!
- (IBAction) fingerSizeAction:(id)sender {
	UISegmentedControl* segCtl = sender;
	pieMenu.fingerSize = [segCtl selectedSegmentIndex];
}
#warning Fonction a mettre dans les settings!!!!!
- (IBAction) leftHandedAction:(id)sender {
	UISwitch *swit = (UISwitch *)sender;
	pieMenu.leftHanded = swit.on;
}

//- (UIResponder *)nextResponder {
//	if (pieMenu.on) {
//		return [pieMenu view];
//	} else {
//		return [super nextResponder];
//	}
//}


- (IBAction)pinchDetected:(id)sender
{
    // Rich man pinch
    if([(UIPinchGestureRecognizer*)sender state] == UIGestureRecognizerStateEnded)
    {
        __previousScale = 1.0;
        return;
    }
    float currentScale = [(UIPinchGestureRecognizer*)sender scale];
    float diff = currentScale-__previousScale;
    if(diff > 1.1 || diff < 0.9)
    {
        [mModel zoom:diff];
        __previousScale = currentScale;
    }
    
    
}

- (IBAction)swipeDetected:(id)sender
{
    NSLog(@"swipe \n");
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch {
    
    //if (![gestureRecognizer isMemberOfClass:[UILongPressGestureRecognizer class]])
    //{
    // Return NO for views that don't support Taps
    UIView* view=touch.view;
    if ((view!=theEAGLView)) {
        return NO;
    }
    
    //}
    return YES;
    
    //return YES;
}

- (IBAction)longPressDetected:(UILongPressGestureRecognizer *)sender
{
    NSLog(@"LONGPRESSDETECTED \n");
    if (sender.state == UIGestureRecognizerStateBegan){
        NSLog(@"UIGestureRecognizerStateBegan.\n");
        //Do Whatever You want on Began of Gesture
        CGPoint p = [sender locationInView:self.mGLView];
        //[touch locationInView:self.view];
        [pieMenu showInView:self.view atPoint:p];
        //[super touchesBegan:[sender touch] withEvent:[sender ]];
        //[mModel eventCancel];
        
    }
    //else if (sender.state == UIGestureRecognizerState) {
    //NSLog(@"UIGestureRecognizerStateEnded");
    //Do Whatever You want on End of Gesture
    //}
    
    
}


- (void)viewDidUnload
{
	[super viewDidUnload];
	
    if (program) {
        glDeleteProgram(program);
        program = 0;
    }
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;
    self.carousel = nil;
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating) {
        [self setupView];
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating) {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}


/// DEPRECATED
- (void)updateOrtho
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof( ( -( LARGEUR_FENETRE / 2) + translationX ) * zoomFactor, ( (LARGEUR_FENETRE / 2) + translationX) * zoomFactor, ( -(HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor, ( (HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor, -5, 100);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

- (void)unselectAllTools
{
    // AJOUTER TOUS NOUVEAUX TOOL ICI
    //mMoveTool = false;
    //mSelectTool = false;
}

-(void)pressButtonUI:(UIButton *)sender
{
    if( previouslySelected != nil )
    {
        [previouslySelected setBackgroundImage:buttonImage forState:UIControlStateNormal];
        [previouslySelected setBackgroundImage:buttonImageHighlight forState:UIControlStateHighlighted];
    }
    
    if( sender != nil )
    {
        [sender setBackgroundImage:buttonImagePressed forState:UIControlStateNormal];
        [sender setBackgroundImage:buttonImagePressed forState:UIControlStateHighlighted];
        
        previouslySelected = sender;
        
        [carouselBackground setImage:carouselBackgroundImage];
    }
}

- (void)carouselSelectItem:(NSInteger)index
{
    NSLog(@"%d", index);
    [carouselBackground setImage:carouselBackgroundSelected];
    [self pressButtonUI:nil];
}


- (void)carousel:(iCarousel *)carousel didSelectItemAtIndex:(NSInteger)index
{
    CarouselElement* element = [carouselElements objectAtIndex:index];
    [self carouselSelectItem:index];
    [mEventManager modifyState:element->ModifType];
}

-(IBAction) cameraModeButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_MOVE_WINDOW];
}

- (IBAction)editorModeButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_SELECTION];
}

- (IBAction)leftArrowButton:(UIButton *)sender
{
    [self.carousel scrollByNumberOfItems:-1 duration:0.3f];
}

- (IBAction)rightArrowButton:(UIButton *)sender
{
    [self.carousel scrollByNumberOfItems:1 duration:0.3f];
}

- (IBAction)selectToolButtonTouched:(UIButton *)sender
{
    if(![sender isMemberOfClass:[PieMenuItem class]])
    {
        [self pressButtonUI:sender];
    }
    [mEventManager modifyState:EDITOR_STATE_SELECTION];
}

- (IBAction)moveToolButtonTouched:(UIButton *)sender
{
    if(![sender isMemberOfClass:[PieMenuItem class]])
    {
        [self pressButtonUI:sender];
    }
    [mEventManager modifyState:EDITOR_STATE_TRANSFORMATION_DEPLACEMENT];
}

- (IBAction)rotationToolButtonTouched:(UIButton *)sender
{
    if(![sender isMemberOfClass:[PieMenuItem class]])
    {
        [self pressButtonUI:sender];
    }
    [mEventManager modifyState:EDITOR_STATE_TRANSFORMATION_ROTATION];
}

- (IBAction)scaleToolButtonTouched:(UIButton *)sender
{
    if(![sender isMemberOfClass:[PieMenuItem class]])
    {
        [self pressButtonUI:sender];
    }
    [mEventManager modifyState:EDITOR_STATE_TRANSFORMATION_ECHELLE];
}

- (IBAction)duplicateToolButtonTouched:(UIButton *)sender
{
    if(![sender isMemberOfClass:[PieMenuItem class]])
    {
        [self pressButtonUI:nil];
    }
    [mModel duplicateSelection];
}

- (IBAction)deleteToolButtonTouched:(UIButton *)sender
{
    if(![sender isMemberOfClass:[PieMenuItem class]])
    {
        [self pressButtonUI:nil];
    }
    [mModel deleteSelection];
}

- (IBAction)portalButtonTouched:(UIButton *)sender
{
    [mEventManager modifyState:EDITOR_STATE_AJOUTER_MAILLET];
}

-(IBAction) saveAndExitButtonTouched:(UIButton *)sender
{
    [mModel saveField];
}


///DEPRECATED
- (CGPoint)convertScreenCoordToVirtualCoord:(CGPoint)pointToConvert;
{
    int AXIS_X_MIN = ( -( LARGEUR_FENETRE / 2) + translationX ) * zoomFactor;
    int AXIS_X_MAX = ( (LARGEUR_FENETRE / 2) + translationX) * zoomFactor;
    int AXIS_Y_MIN = ( -(HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor;
    int AXIS_Y_MAX = ( (HAUTEUR_FENETRE / 2) + translationY ) * zoomFactor;
    
    CGPoint pointVirt;
    
    pointVirt.x = pointToConvert.x / (double) LARGEUR_FENETRE * (AXIS_X_MAX-AXIS_X_MIN) + AXIS_X_MIN;
    pointVirt.y = AXIS_Y_MAX - (pointToConvert.y / (double) HAUTEUR_FENETRE * (AXIS_Y_MAX-AXIS_Y_MIN));
    
    return pointVirt;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    
    
    
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint positionCourante = [touch locationInView:theEAGLView];
    
    UIView* viewTouched = [touch view];
    if (![viewTouched isMemberOfClass:[UIImageView class]] && ![viewTouched isMemberOfClass:[UIView class]]) {
        if(!propertyBarHidden)
        {
            [UIView beginAnimations:@"MenuAnimationShow" context:NULL];
            [UIView setAnimationDuration:1];
            self.mPropertyView.center = CGPointMake(mPropertyView.center.x + mPropertyView.bounds.size.width, mPropertyView.center.y);
            [UIView commitAnimations];
            propertyBarHidden = YES;
        }
        [mEventManager touchesBegan:positionCourante];
    }
    
    //CGPoint touchCoordVirt = [self convertScreenCoordToVirtualCoord:[touch locationInView:theEAGLView]];
    //[mEventManager touchesBegan:touch:positionCourante];
    
    //    NSLog(@"Position de tous les doigts venant de commencer à toucher l'écran");
    //    for(UITouch* touch in touches) {
    //        CGPoint positionCourante = [touch locationInView:theEAGLView];
    //        NSLog(@"x: %f y: %f", positionCourante.x, positionCourante.y);
    //    }
    //    NSLog(@"Position de tous les doigts sur l'écran");
    //    NSSet *allTouches = [event allTouches];
    //    for(UITouch* touch in allTouches) {
    //        CGPoint positionCourante = [touch locationInView:theEAGLView];
    //        NSLog(@"x: %f y: %f", positionCourante.x, positionCourante.y);
    //    }
    //    NSLog(@"\n\n");
}


-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    
    if ([[event allTouches] count] == 1)
    {
        UITouch *touch = [[event allTouches] anyObject];
        CGPoint positionCourante = [touch locationInView:theEAGLView];
        UIView * viewTouched = touch.view;
        if (![viewTouched isMemberOfClass:[UIImageView class]] && ![viewTouched isMemberOfClass:[UIView class]]) {
            //CGPoint touchCoordVirt = [self convertScreenCoordToVirtualCoord:[touch locationInView:theEAGLView]];
            [mEventManager touchesMoved:positionCourante];
        }
        
        //CGPoint positionCourante = [touch locationInView:theEAGLView];
        //        if (mCreationMode) {
        //            // Si on est en mode creation et touchMoved, on update la position de limage
        //            //imageObjectToAdd.center = [touch locationInView:theEAGLView];
        //            CGPoint coordVirt = [self convertScreenCoordToVirtualCoord:positionCourante];
        //            [mModel eventModification:FIELD_MODIFICATION_EVENT_MOVE:coordVirt];
        //
        //        }
        //        else if (mSelectionMode && mMoveTool)
        //        {
        //            CGPoint positionPrecedente = [touch previousLocationInView:theEAGLView];
        //            translationX -= (positionCourante.x - positionPrecedente.x);
        //            translationY += (positionCourante.y - positionPrecedente.y);
        //
        //            // Set boundaries for the editing grid, currently 1000x1000, centered at 0,0.
        //            /*
        //             if( translationX < ( -500 / zoomFactor ) )
        //             {
        //             translationX = (int)( -500 / zoomFactor );
        //             }
        //             */
        //            if( translationX < -500 )
        //            {
        //                translationX = -500;
        //            }
        //            else if( translationX > 500 )
        //            {
        //                translationX = 500;
        //            }
        //
        //            if( translationY < -500 )
        //            {
        //                translationY = -500;
        //            }
        //            else if( translationY > 500 )
        //            {
        //                translationY = 500;
        //            }
        //
        //            [self updateOrtho];
        //
        //        }
        //        else if (mSelectionMode && mSelectTool)
        //        {
        //            touchMoved = true;
        //        }
    }
    else if([[event allTouches] count] == 2) {
        
    }
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    
    
    if ([[event allTouches] count] == 1)
    {
        UITouch *touch = [[event allTouches] anyObject];
        CGPoint positionCourante = [touch locationInView:theEAGLView];
        
        //CGPoint touchCoordVirt = [self convertScreenCoordToVirtualCoord:[touch locationInView:theEAGLView]];
        [mEventManager touchesEnded:positionCourante];
        //
        //        if (mCreationMode) {
        //            // Destruction de limage de lobjet qui suit la position du doigt
        //
        //                // On drop lobjet
        //                CGPoint coordVirt = [self convertScreenCoordToVirtualCoord:positionCourante];
        //            // On drop le noeud a la position finale
        //            [mModel eventModification:FIELD_MODIFICATION_EVENT_CLICK:coordVirt];
        //            // On enleve le prochain noeud qui apparait pour sajouter, utilise dans c++
        //            [mModel eventCancel];
        //                //[imageObjectToAdd removeFromSuperview];
        //                //[imageObjectToAdd release];
        //        }
        
        //        if(mSelectionMode && mSelectTool)
        //        {
        //
        //
        //            CGPoint posVirtuelle = [self convertScreenCoordToVirtualCoord:positionCourante];
        //
        //            int CV_X_NOW = posVirtuelle.x;
        //            int CV_Y_NOW = posVirtuelle.y;
        //
        //
        //            int CV_X_OLD;
        //            int CV_Y_OLD;
        //            if(touchMoved)
        //            {
        //                CGPoint firstCornerVirt;
        //                firstCornerVirt = [self convertScreenCoordToVirtualCoord:firstCorner];
        //                CV_X_OLD = firstCornerVirt.x;
        //                CV_Y_OLD = firstCornerVirt.y;
        //            }
        //            else
        //            {
        //                CV_X_OLD = CV_X_NOW-2;
        //                CV_X_NOW += 2;
        //                CV_Y_OLD = CV_Y_NOW+2;
        //                CV_Y_NOW-=2;
        //            }
        //            int nbNoeudsSelectionnes = [mModel acceptSelectionVisitor:CV_X_OLD:CV_Y_OLD:CV_X_NOW:CV_Y_NOW];
        //            if(nbNoeudsSelectionnes==1)
        //            {
        //                // Si on a un seul noeud selectionne, on ouvre un popovercontroller contenant les proprietes modifiables du noeud
        //
        //                //UITableViewController *tableController = [[UITableViewController alloc]initWithStyle:UITableViewStylePlain];
        //
        //                //UITabBarController *tabController = [[UITabBarController alloc] init];
        //
        //                //UINavigationController *navController = [[UINavigationController alloc]initWithRootViewController:tableController];
        //
        //                //UIPopoverController *popOverController = [[UIPopoverController alloc]initWithContentViewController:navController];
        //                //navController.tabBarController = tabController;
        //
        //                //[popOverController presentPopoverFromRect:CGRectMake(150, 300, 450, 300) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
        //
        //            }
        //        }
    }
}

// Event Callback du c++ pour update du UI
- (void) enablePuckCreation
{
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Ben coliss" message:@"Ca marche" delegate:nil cancelButtonTitle:@"Gotcha" otherButtonTitles:nil];
    [alert show];
}
- (void)disablePuckCreation
{
    
}
- (void)enableMalletCreation
{
    
}
- (void)disableMalletCreation
{
    
}
- (void)thereAreNodesSelected
{
    
}
- (void)thereAreNoNodesSelected
{
    // Disable le delete
}
- (void)canUndo
{
    
}
- (void)cannotUndo
{
    
}
- (void)canRedo
{
    
}
- (void)cannotRedo
{
    
}

-(void)rotationDetectee:(UIGestureRecognizer *)gestureRecognizer
{
    if ([gestureRecognizer numberOfTouches] == 2)
    {
        CGPoint position = [gestureRecognizer locationInView:theEAGLView];
        NSLog(@"ROTATION GESTURE : Centre de rotation x: %f y: %f\n",position.x, position.y);
    }
}


// DEPRECATED
-(void)setupView
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    
    
    CGRect rect = theEAGLView.bounds;
    
    [self updateOrtho];
    
    
    glViewport(0, 0, rect.size.width, rect.size.height);
    
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClearColor(0.3765, 0.4039, 0.4862, 1.0); //Background color for the editing area.
    
    glGetError(); // Clear error codes
}


- (void)drawFrame
{
    [(EAGLView *)theEAGLView setFramebuffer];
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    [mModel render];
    
    [(EAGLView *)theEAGLView presentFramebuffer];
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

#pragma mark -
#pragma mark iCarousel methods

- (NSUInteger)numberOfItemsInCarousel:(iCarousel *)carousel
{
    //return the total number of items in the carousel
    return [items count];
}

- (void) carouselDidEndScrollingAnimation:(iCarousel *)carousel;
{
    NSInteger currentIndex = carousel.currentItemIndex;
    CarouselElement* element = [carouselElements objectAtIndex:currentIndex];
    [self carouselSelectItem:currentIndex];
    [mEventManager modifyState:element->ModifType];
}

- (UIView *)carousel:(iCarousel *)carousel viewForItemAtIndex:(NSUInteger)index reusingView:(UIView *)view
{
    UILabel *label = nil;
    CarouselElement* element = [carouselElements objectAtIndex:index];
    
    //create new view if no view is available for recycling
    if (view == nil)
    {
        //don't do anything specific to the index within
        //this `if (view == nil) {...}` statement because the view will be
        //recycled and used with other index values later
        
        view = [[[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 98.0f, 85.0f)] autorelease];
        view.contentMode = UIViewContentModeTop;
        CGRect labelPosition = view.bounds;
        labelPosition.origin.y += 25;
        UILabel* label = [[[UILabel alloc] initWithFrame:labelPosition] autorelease];
        label.backgroundColor = [UIColor clearColor];
        label.textAlignment = NSTextAlignmentCenter;
        //label.font = [label.font fontWithSize:20];
        label.font = [UIFont fontWithName:@"Helvetica-Bold" size:20];
        //label.textColor = [UIColor colorWithRed:0.1922f green: 0.4745f blue: 0.6784f alpha:1.0f];
        label.textColor = [UIColor whiteColor];
        //label.shadowOffset = ;
        //label
        label.shadowColor = [UIColor blackColor];
        label.shadowOffset = CGSizeMake(1, 1);
        label.tag = 1;
        label.text = element.LabelValue;
        ((UIImageView *)view).image = [UIImage imageNamed:element.ImageName];
        [view addSubview:label ];
    }
    else
    {
        //get a reference to the label in the recycled view
        label = (UILabel *)[view viewWithTag:1];
    }
    
    //set item label
    //remember to always set any properties of your carousel item
    //views outside of the `if (view == nil) {...}` check otherwise
    //you'll get weird issues with carousel item content appearing
    //in the wrong place in the carousel
    //label.text = [[items objectAtIndex:index] stringValue];
    
    ((UIImageView *)view).image = [UIImage imageNamed:element.ImageName];
    label.text = element.LabelValue;
    
    return view;
}

- (CGFloat)carousel:(iCarousel *)carousel valueForOption:(iCarouselOption)option withDefault:(CGFloat)value
{
    switch( option )
    {
        case iCarouselOptionSpacing:
            return value * 1.1f;
            
        case iCarouselOptionWrap:
            return YES;
        default :
            break;
    }
    return value;
}

@end
