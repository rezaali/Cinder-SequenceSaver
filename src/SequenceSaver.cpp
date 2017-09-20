#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "SequenceSaver.h"

#include "cinder/Log.h"
#include "cinder/Utilities.h"

#include "Paths.h"

using namespace ci;
using namespace std;
using namespace reza::paths;
using namespace reza::tiler;

namespace reza {
namespace seq {
SequenceSaver::SequenceSaver( const ci::app::WindowRef &window,
	const std::function<void()> &drawFn,
	const std::function<void( glm::vec2, glm::vec2, glm::vec2, glm::vec2 )> &drawBgFn,
	const std::function<void( glm::vec2, glm::vec2, glm::vec2, glm::vec2 )> &drawPostFn )
	: mWindowRef( window ), mDrawFn( drawFn ), mDrawBgFn( drawBgFn ), mDrawPostFn( drawPostFn )
{
}

SequenceSaver::~SequenceSaver()
{
}

void SequenceSaver::save( const ci::fs::path &path, const std::string &filename, const std::string &extension, bool alpha )
{
    save( CameraPersp(), path, filename, extension, alpha );
}
    
void SequenceSaver::save( const ci::CameraPersp &cam, const ci::fs::path &path, const std::string &filename, const std::string &extension, bool alpha )
{
	if( mRecording ) {
		return;
	}

	mCurrentFrame = 0;
	mCurrentTime = 0.0f;
	mProgress = 0.0f;

	mSaveImagePath = path;
	mSaveImageName = filename;
	mSaveImageExtension = extension;

	ivec2 windowSize = mWindowRef->getSize();
	ivec2 outputSize = windowSize * mSizeMultiplier;

	mTilerRef = Tiler::create( outputSize, windowSize, mWindowRef, alpha );
	mTilerRef->setMatrices( cam );
	if( mDrawBgFn ) {
		mTilerRef->setDrawBgFn( [this]( glm::vec2 ul, glm::vec2 ur, glm::vec2 lr, glm::vec2 ll ) {
			mDrawBgFn( ul, ur, lr, ll );
		} );
	}

	if( mDrawFn ) {
		mTilerRef->setDrawFn( [this]() {
			mDrawFn();
		} );
	}

	if( mDrawPostFn ) {
		mTilerRef->setDrawPostFn( [this]( const vec2 &ul, const vec2 &ur, const vec2 &lr, const vec2 &ll ) {
			mDrawPostFn( ul, ur, lr, ll );
		} );
	}

	mRecording = true;
}

void SequenceSaver::update()
{
	mCurrentTime = float( mCurrentFrame ) / float( mTotalFrames );
	if( mRecording && mCurrentFrame > 0 ) {
		int frameNumber = mCurrentFrame;
		if( frameNumber <= mTotalFrames ) {
			mProgress = float( frameNumber ) / float( mTotalFrames );

			std::stringstream out;
			int width = 1 + int( log10( mTotalFrames ) );
			out << fixed << setfill( '0' ) << std::setw( width ) << frameNumber;
            
			auto path = addPath( addPath( mSaveImagePath, mSaveImageName + out.str() ), mSaveImageExtension, "." );
			writeImage( path, mTilerRef->getSurface(), ImageTarget::Options().quality( 1.0 ) );
            
		}
		if( frameNumber == mTotalFrames ) {
			mRecording = false;
			mProgress = 0.0f;
		}
	}
	if( mCurrentFrame >= mTotalFrames ) {
		mCurrentFrame = 0;
	}
	mCurrentFrame++;
}
} // namespace seq
} // namespace reza
