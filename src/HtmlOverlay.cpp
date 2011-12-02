//
//  HtmlOverlay.cpp
//  CinderGLKitTemplate
//
//  Created by Tom Carden on 12/1/11.
//  Copyright (c) 2011 Bloom Studio, Inc. All rights reserved.
//

#include "HtmlOverlay.h"

void HtmlOverlay::loadingStarted()
{
    mCbLoadingStarted.call( shared_from_this() );
}

void HtmlOverlay::loadingFinished()
{
    mCbLoadingFinished.call( shared_from_this() );
}

void HtmlOverlay::loadingError()
{
    mCbLoadingError.call( shared_from_this() );
}

bool HtmlOverlay::loadingShouldStart( std::string url )
{
	bool shouldStart = true;
    // all callbacks must return true for the webview to be allowed to load the url
	for( auto iter : mCbLoadingShouldStart ) {
        shouldStart = shouldStart && (iter.second)( shared_from_this(), url );
    }
    return shouldStart;
}