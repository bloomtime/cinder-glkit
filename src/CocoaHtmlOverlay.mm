//
//  CocoaHtmlOverlay.mm
//  CinderGLKitTemplate
//
//  Created by Tom Carden on 11/30/11.
//  Copyright (c) 2011 Bloom Studio, Inc. All rights reserved.
//

#include "CocoaHtmlOverlay.h"

CocoaHtmlOverlay::~CocoaHtmlOverlay()
{
    //[mWebView release]; // not needed with ARC
}

void CocoaHtmlOverlay::setHtml( std::string html )
{
    NSString *str = [NSString stringWithUTF8String: html.c_str()];
    [mWebView loadHTMLString:str baseURL:nil];
}

void CocoaHtmlOverlay::setRect( ci::Rectf size )
{
    mWebView.frame = CGRectMake( size.x1, size.y1, size.getWidth(), size.getHeight() );
}

ci::Rectf CocoaHtmlOverlay::getRect( )
{
    return ci::Rectf( mWebView.frame.origin.x, 
                      mWebView.frame.origin.y, 
                      mWebView.frame.origin.x + mWebView.frame.size.width, 
                      mWebView.frame.origin.y + mWebView.frame.size.height );
}

void CocoaHtmlOverlay::setShowing( bool showing )
{
    mWebView.hidden = !showing;
}

void CocoaHtmlOverlay::setHidden( bool hidden )
{
    mWebView.hidden = hidden;
}

bool CocoaHtmlOverlay::isShowing()
{
    return !mWebView.hidden;
}

bool CocoaHtmlOverlay::isHidden()
{
    return mWebView.hidden;
}