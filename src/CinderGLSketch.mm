#include "CinderGLSketch.h"

#include "cinder/Utilities.h"
#include "cinder/app/App.h"

namespace cinder {

fs::path CinderGLSketch::getResourcePath( const fs::path &rsrcRelativePath )
{
    fs::path path = rsrcRelativePath.parent_path();
    fs::path fileName = rsrcRelativePath.filename();
    
    if( fileName.empty() )
        return string();
    
    NSString *pathNS = 0;
    if( ( ! path.empty() ) && ( path != rsrcRelativePath ) )
        pathNS = [NSString stringWithUTF8String:path.c_str()];
    
    NSString *resultPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:fileName.c_str()] ofType:nil inDirectory:pathNS];
    if( ! resultPath )
        return string();
    
    return fs::path([resultPath cStringUsingEncoding:NSUTF8StringEncoding]);
}

fs::path CinderGLSketch::getResourcePath()
{
    char p[4096];
    
    CFURLRef url = ::CFBundleCopyResourcesDirectoryURL( ::CFBundleGetMainBundle() );
    ::CFURLGetFileSystemRepresentation( url, true, (UInt8*)p, 4096 );
    ::CFRelease( url );
    
    return fs::path( std::string( p ) ); // not casting this to a std::string seems to fail on iOS
}

DataSourcePathRef CinderGLSketch::loadResource(const string &path)
{
	fs::path resourcePath = getResourcePath( path );
	if( resourcePath.empty() )
		throw app::ResourceLoadExc( path );
	else
		return DataSourcePath::create( resourcePath );
}

} // namespace cinder