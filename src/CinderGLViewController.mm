#import "CinderGLViewController.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Vector.h"
#include "cinder/Matrix.h"

using namespace ci;


@interface CinderGLViewController(){
    gl::GlslProg program;
}

@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end


@implementation CinderGLViewController

@synthesize context = _context;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES2];

    if(!self.context){
        NSLog(@"Failed to create ES2 context");
    }
    
    GLKView *view = (GLKView*)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if([EAGLContext currentContext] == self.context){
        [EAGLContext setCurrentContext: nil];
    }
	self.context = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    if([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone)
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    else
        return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext: self.context];
    
    glEnable(GL_DEPTH_TEST);
    
    // Create Shaders and such
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext: self.context];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    // Step
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Draw
}

@end
