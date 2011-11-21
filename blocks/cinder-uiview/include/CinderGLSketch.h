#pragma once

#include "cinder/Vector.h"
#include "cinder/Area.h"
#include "cinder/Timer.h"
#include "cinder/app/TouchEvent.h"

#include <ostream>

namespace cinder {

class CinderGLSketch {
public:

    CinderGLSketch()
    : mNeedsSetup(true)
    , mTimer(true)
    {};
    
    virtual ~CinderGLSketch(){};

    virtual void setup(){};
    virtual void tearDown(){};
    virtual void update(){};
    virtual void draw(const Area &area){};
    
    const Vec2i& getSize() const { return mSize; }
    void         setSize(const Vec2i &size){ mSize = size; }
    float        getAspectRatio() const { return (float)mSize.x / (float)mSize.y; }
    
    double getElapsedSeconds() const { return mTimer.getSeconds(); }

    virtual void touchesBegan(app::TouchEvent event){}
    virtual void touchesMoved(app::TouchEvent event){}
    virtual void touchesEnded(app::TouchEvent event){}
    
    const std::vector<app::TouchEvent::Touch>& getActiveTouches(){ return mActiveTouches; }
    void setActiveTouches(std::vector<app::TouchEvent::Touch> touches){ mActiveTouches = touches; }
    
    std::ostream& console(){ return std::cout; }
    
    bool mNeedsSetup;

protected:

    Vec2i mSize;
    Timer mTimer;
    
    std::vector<app::TouchEvent::Touch> mActiveTouches;

};

} // namespace cinder