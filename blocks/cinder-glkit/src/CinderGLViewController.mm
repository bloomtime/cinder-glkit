#import "CinderGLViewController.h"

#include <vector>

#include "cinder/Area.h"
#include "cinder/Vector.h"
#include "cinder/app/TouchEvent.h"

using namespace ci;
using namespace ci::app;

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
    return mSketch->supportsInterfaceOrientation( (int)interfaceOrientation );
}


- (void)setSketch:(CinderGLSketch *)sketch
{
    mSketch = sketch;
}


#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    [EAGLContext setCurrentContext: self.context];
    if(mSketch){
        mSketch->privateUpdate__();
    }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    [EAGLContext setCurrentContext: self.context];
    
    if(mSketch){
        GLKView *view = (GLKView *)self.view;
        
        Vec2i size(view.drawableWidth, view.drawableHeight);
        if(size != mSketch->getSize()){
            mSketch->setSize(size);
        }
        
        if(mSketch->mNeedsSetup){
            mSketch->privateSetup__();
            mSketch->mNeedsSetup = false;
        }
        
        mSketch->draw(Area(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height));
    }
}


#pragma mark - Touch Handling

- (uint32_t)addTouchToMap:(UITouch*)touch
{
    // Find the lowest unused touch ID
    uint32_t candidate_id = 0;
    bool found = true;
    while(found){
        candidate_id++;
        found = false;
        for(auto pair : mTouchIds){
            if(pair.second == candidate_id){
                found = true;
                break;
            }
        }
    }
    
    mTouchIds.insert(std::make_pair(touch, candidate_id));
    
    return candidate_id;
}

- (void)removeTouchFromMap:(UITouch*)touch
{
    auto found(mTouchIds.find(touch));
    if(found != mTouchIds.end())
        mTouchIds.erase(found);
}

- (uint32_t)findTouchInMap:(UITouch*)touch
{
    auto found(mTouchIds.find(touch));
    if(found != mTouchIds.end())
        return found->second;
    return 0;
}

- (void)updateActiveTouches
{
    static float content_scale = [self.view respondsToSelector: NSSelectorFromString(@"contentScaleFactor")] ? self.view.contentScaleFactor : 1;
    
    std::vector<TouchEvent::Touch> active_touches;
    for(auto pair : mTouchIds){
        CGPoint pt = [pair.first locationInView: self.view];
        CGPoint pt_prev = [pair.first previousLocationInView: self.view];
        active_touches.push_back(TouchEvent::Touch(
            Vec2f(pt.x, pt.y) * content_scale,
            Vec2f(pt_prev.x, pt_prev.y) * content_scale,
            pair.second,
            [pair.first timestamp],
            (__bridge_retained void*)pair.first
        ));
    }
    mSketch->setActiveTouches(active_touches);
}

- (std::vector<app::TouchEvent::Touch>)convertTouches:(NSSet*)touches andAddToMap:(BOOL)add orRemoveFromMap:(BOOL)remove
{
    static float content_scale = [self.view respondsToSelector: NSSelectorFromString(@"contentScaleFactor")] ? self.view.contentScaleFactor : 1;
    
    std::vector<TouchEvent::Touch> touch_list;
    for(UITouch *touch in touches){
        CGPoint pt = [touch locationInView: self.view];
        CGPoint pt_prev = [touch previousLocationInView: self.view];
        touch_list.push_back(TouchEvent::Touch(
            Vec2f(pt.x, pt.y) * content_scale,
            Vec2f(pt_prev.x, pt_prev.y ) * content_scale,
            add ? [self addTouchToMap: touch] : [self findTouchInMap: touch],
            [touch timestamp],
            (__bridge_retained void*)touch
        ));
        if(remove){
            [self removeTouchFromMap: touch];
        }
    }
    return touch_list;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent *)event
{   
    if(mSketch){
        auto touch_list = [self convertTouches: touches andAddToMap: YES orRemoveFromMap: NO];
        [self updateActiveTouches];
        if(!touch_list.empty())
            mSketch->touchesBegan(TouchEvent(touch_list));
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    if(mSketch){
        auto touch_list = [self convertTouches: touches andAddToMap: NO orRemoveFromMap: NO];
        [self updateActiveTouches];
        if(!touch_list.empty())
            mSketch->touchesMoved(TouchEvent(touch_list));
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    if(mSketch){
        auto touch_list = [self convertTouches: touches andAddToMap: NO orRemoveFromMap: YES];
        [self updateActiveTouches];
        if(!touch_list.empty())
            mSketch->touchesEnded(TouchEvent(touch_list));
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    if(mSketch){
        auto touch_list = [self convertTouches: touches andAddToMap: NO orRemoveFromMap: YES];
        [self updateActiveTouches];
        if(!touch_list.empty())
            mSketch->touchesCanceled(TouchEvent(touch_list));
    }
}

@end
