//
//  CocoaHtmlOverlay.h
//  CinderGLKitTemplate
//
//  Created by Tom Carden on 11/30/11.
//  Copyright (c) 2011 Bloom Studio, Inc. All rights reserved.
//

#pragma once

#import <UIKit/UIKit.h>
#include "HtmlOverlay.h" // interface

class CocoaHtmlOverlay;

@interface WVDelegate : NSObject <UIWebViewDelegate> {
@public
    CocoaHtmlOverlay *mCocoaHtmlOverlay;
}
@end

class CocoaHtmlOverlay : public HtmlOverlay 
{
public:
    
    static HtmlOverlayRef create( UIView *view )
    {
        return HtmlOverlayRef( new CocoaHtmlOverlay( view ) );
    }
    
    void setHtml( std::string html );
    void setRect( ci::Rectf rect );
    ci::Rectf getRect();
    
    void setShowing( bool showing = true );
    void setHidden( bool hidden = true );
    bool isShowing();
    bool isHidden();
    
    ~CocoaHtmlOverlay();
    
private:
    
    CocoaHtmlOverlay( UIView *view );
    
    UIWebView *mWebView;
    WVDelegate *mDelegate;
};
