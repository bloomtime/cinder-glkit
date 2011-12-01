//
//  HtmlOverlay.h
//  CinderGLKitTemplate
//
//  Created by Tom Carden on 11/30/11.
//  Copyright (c) 2011 Bloom Studio, Inc. All rights reserved.
//

#pragma once

#include <string>
#include "cinder/Cinder.h"
#include "cinder/Rect.h"

typedef std::shared_ptr<class HtmlOverlay> HtmlOverlayRef;

class HtmlOverlay
{
public:
    
    virtual void setHtml( std::string html ) = 0;
    virtual void setRect( ci::Rectf rect ) = 0;
    virtual ci::Rectf getRect() = 0;
    
    virtual void setShowing( bool showing = true ) = 0;
    virtual void setHidden( bool hidden = true ) = 0;
    virtual bool isShowing() = 0;
    virtual bool isHidden() = 0;
    
    virtual ~HtmlOverlay() {};
};