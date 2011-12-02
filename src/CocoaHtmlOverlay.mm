//
//  CocoaHtmlOverlay.mm
//  CinderGLKitTemplate
//
//  Created by Tom Carden on 11/30/11.
//  Copyright (c) 2011 Bloom Studio, Inc. All rights reserved.
//

#include "CocoaHtmlOverlay.h"
#import <QuartzCore/CAAnimation.h>
#import <QuartzCore/CATransaction.h>

//////////////////////////////

@implementation WVDelegate

- (void)webViewDidStartLoad:(UIWebView *)wv 
{
//    NSLog (@"webViewDidStartLoad");
    mCocoaHtmlOverlay->loadingStarted();
}

- (void)webViewDidFinishLoad:(UIWebView *)wv {
//    NSLog (@"webViewDidFinishLoad");
   // TODO: if autoSize, in overridden cocoahtmloverlay thinger 
    CGRect frame = wv.frame;
    frame.size.height = 1;
    wv.frame = frame;
    CGSize fittingSize = [wv sizeThatFits:CGSizeZero];
    fittingSize.width += 5;  // box-shadow-hack
    fittingSize.height += 5; // box-shadow-hack
    frame.size = fittingSize;
    wv.frame = frame;
    mCocoaHtmlOverlay->loadingFinished();
}

- (void)webView:(UIWebView *)wv didFailLoadWithError:(NSError *)error {
    NSLog (@"webView:didFailLoadWithError");
//    NSLog ([error description]);
    mCocoaHtmlOverlay->loadingError();    
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType 
{
//    NSLog (@"webView:shouldStartLoadWithRequest");
    // TODO: pass the type as well (blah blah leaky abstractions blah)
    /*
        UIWebViewNavigationTypeLinkClicked,
        UIWebViewNavigationTypeFormSubmitted,
        UIWebViewNavigationTypeBackForward,
        UIWebViewNavigationTypeReload,
        UIWebViewNavigationTypeFormResubmitted,
        UIWebViewNavigationTypeOther
    */
    // if it's "other" then we set the HTML manually (so carry on), otherwise check the URL with the app...
    return navigationType == UIWebViewNavigationTypeOther || mCocoaHtmlOverlay->loadingShouldStart( std::string([[request.URL absoluteString] UTF8String]) );
}

@end

//////////////////////////////

CocoaHtmlOverlay::CocoaHtmlOverlay( UIView *view )
{
    CGRect webFrame = CGRectMake(0.0, 0.0, 300.0, 100.0);
    mWebView = [[UIWebView alloc] initWithFrame:webFrame];
    mWebView.opaque = NO;
    mWebView.backgroundColor = [UIColor clearColor];
    mWebView.scalesPageToFit = NO;

    if ([mWebView respondsToSelector:@selector(scrollView)]) {
        mWebView.scrollView.alwaysBounceHorizontal = NO;
        mWebView.scrollView.alwaysBounceVertical = NO;
        mWebView.scrollView.scrollEnabled = NO;
    }
    
    mDelegate = [[WVDelegate alloc] init];
    mDelegate->mCocoaHtmlOverlay = this;
    mWebView.delegate = mDelegate;
    
    mWebView.hidden = YES;
    [view addSubview: mWebView];
}

CocoaHtmlOverlay::~CocoaHtmlOverlay()
{
    mWebView.delegate = nil;
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
    setHidden( !showing );
}

void CocoaHtmlOverlay::setHidden( bool hidden )
{
    if (mWebView.hidden != hidden) {
        [CATransaction begin];
        CATransition *animation = [CATransition animation];
        animation.type = kCATransitionFade;
        animation.duration = 0.15;

        [[mWebView layer] addAnimation:animation forKey:@"fade"];
        [CATransaction commit];
        
        mWebView.hidden = hidden;
    }
}

bool CocoaHtmlOverlay::isShowing()
{
    return !mWebView.hidden;
}

bool CocoaHtmlOverlay::isHidden()
{
    return mWebView.hidden;
}

