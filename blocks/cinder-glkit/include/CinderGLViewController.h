#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include <tr1/unordered_map>
#include "CinderGLSketch.h"

using namespace ci;

@interface CinderGLViewController : GLKViewController {
    CinderGLSketch *mSketch;
    std::tr1::unordered_map< UITouch*, uint32_t > mTouchIds;
}

@property (strong, nonatomic) EAGLContext *context;

- (void)setSketch:(CinderGLSketch*)sketch;

- (uint32_t)addTouchToMap:(UITouch*)touch;
- (void)removeTouchFromMap:(UITouch*)touch;
- (uint32_t)findTouchInMap:(UITouch*)touch;
- (std::vector<ci::app::TouchEvent::Touch>)convertTouches:(NSSet*)touches andRemoveFromMap:(BOOL)remove;
- (void)updateActiveTouches;
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event;
- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event;

@end
