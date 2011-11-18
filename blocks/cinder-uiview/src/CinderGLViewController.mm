#import "CinderGLViewController.h"

#include "cinder/Area.h"
#include "cinder/Vector.h"

using namespace ci;


@implementation CinderGLViewController

@synthesize context = _context;


- (id)init
{
    self = [super init];
    if(self){
        self->mSketch = NULL;
    }
    return self;
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES2];
    self.preferredFramesPerSecond = 60;

    if(!self.context){
        NSLog(@"Failed to create ES2 context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    if(mSketch)
        mSketch->tearDown();
    
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


- (void)setSketch:(CinderGLSketch *)sketch
{
    mSketch = sketch;
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    if(mSketch){
        mSketch->update();
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    [EAGLContext setCurrentContext: self.context];
    
    if(mSketch){
        GLKView *view = (GLKView *)self.view;
        
        Vec2i size(view.drawableWidth, view.drawableHeight);
        if(size != mSketch->getSize())
            mSketch->setSize(size);
        
        if(mSketch->mNeedsSetup){
            mSketch->setup();
            mSketch->mNeedsSetup = false;
        }
        
        mSketch->draw(Area(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height));
    }
}

@end
