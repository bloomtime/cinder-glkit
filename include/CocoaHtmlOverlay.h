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
    
    CocoaHtmlOverlay( UIView *view )
    {
        CGRect webFrame = CGRectMake(0.0, 0.0, 300.0, 100.0);
        mWebView = [[UIWebView alloc] initWithFrame:webFrame];
        mWebView.opaque = NO;
        mWebView.backgroundColor = [UIColor clearColor];
        mWebView.scalesPageToFit = NO;
        //    mWebView.delegate = self;
        mWebView.hidden = YES;
        [view addSubview: mWebView];
    }
    
    UIWebView *mWebView;
    
};
