#pragma once

#include "cinder/Vector.h"
#include "cinder/Area.h"
#include "cinder/Timer.h"

#include <ostream>

namespace cinder {

using namespace std;

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
    
    ostream& console(){ return cout; }
    
    bool mNeedsSetup;

protected:

    Vec2i mSize;
    Timer mTimer;

};

} // namespace cinder