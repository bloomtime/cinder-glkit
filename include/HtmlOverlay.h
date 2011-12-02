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
#include "cinder/Function.h"

typedef std::shared_ptr<class HtmlOverlay> HtmlOverlayRef;

class HtmlOverlay : public std::enable_shared_from_this<HtmlOverlay>
{
public:
    
    // TODO: setUrl
    
    virtual void setHtml( std::string html ) = 0;
    virtual void setRect( ci::Rectf rect ) = 0;
    virtual ci::Rectf getRect() = 0;
    
    virtual void setShowing( bool showing = true ) = 0;
    virtual void setHidden( bool hidden = true ) = 0;
    virtual bool isShowing() = 0;
    virtual bool isHidden() = 0;
    
    template<typename T>
    ci::CallbackId registerLoadingStarted( T *obj, void (T::*callback)(HtmlOverlayRef) )
    {
        return mCbLoadingStarted.registerCb(std::bind1st(std::mem_fun(callback), obj));
    }
    template<typename T>
    ci::CallbackId registerLoadingFinished( T *obj, void (T::*callback)(HtmlOverlayRef) )
    {
        return mCbLoadingFinished.registerCb(std::bind1st(std::mem_fun(callback), obj));
    }
    template<typename T>
    ci::CallbackId registerLoadingError( T *obj, void (T::*callback)(HtmlOverlayRef) )
    {
        return mCbLoadingError.registerCb(std::bind1st(std::mem_fun(callback), obj));
    }
    template<typename T>
    ci::CallbackId registerLoadingShouldStart( T *obj, bool (T::*callback)(HtmlOverlayRef,std::string) )
    {
        return mCbLoadingShouldStart.registerCb(std::bind( callback, obj, std::tr1::placeholders::_1, std::tr1::placeholders::_2));
    }
    
    // TODO: unregisterXXX
    
    void loadingStarted();
    void loadingFinished();
    void loadingError();
    bool loadingShouldStart( std::string url );
    
    virtual ~HtmlOverlay() {};
    
private:
    
    ci::CallbackMgr<void(HtmlOverlayRef)> mCbLoadingStarted, mCbLoadingFinished, mCbLoadingError;
    ci::CallbackMgr<bool(HtmlOverlayRef,std::string)> mCbLoadingShouldStart;
};