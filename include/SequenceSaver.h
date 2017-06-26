/*
Copyright(c) 2017 Reza Ali syed.reza.ali@gmail.com www.syedrezaali.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <functional>
#include <string>

#include "cinder/Camera.h"
#include "cinder/Filesystem.h"
#include "cinder/app/Window.h"

#include "glm/glm.hpp"

#include "Tiler.h"

namespace reza {
namespace seq {
typedef std::shared_ptr<class SequenceSaver> SequenceSaverRef;
class SequenceSaver {
  public:
	static SequenceSaverRef create(
		const ci::app::WindowRef &window, const std::function<void()> &drawFn, const std::function<void( glm::vec2, glm::vec2, glm::vec2, glm::vec2 )> &drawBgFn = nullptr, const std::function<void( glm::vec2, glm::vec2, glm::vec2, glm::vec2 )> &drawPostFn = nullptr )
	{
		return SequenceSaverRef( new SequenceSaver( window, drawFn, drawBgFn, drawPostFn ) );
	}
	virtual ~SequenceSaver();

	void update();

	void save( const ci::CameraPersp &cam, const ci::fs::path &path, const std::string &filename, const std::string &extension );

	bool isRecording() { return mRecording; }
	float getProgress() { return mProgress; }
	float getCurrentTime() { return mCurrentTime; }
	void setSizeMultiplier( int multiplier ) { mSizeMultiplier = multiplier; }
	int getSizeMultiplier() { return mSizeMultiplier; }
	void setTotalFrames( int totalframes ) { mTotalFrames = totalframes; }
	int getTotalFrames() { return mTotalFrames; }
  protected:
	SequenceSaver(
		const ci::app::WindowRef &window, const std::function<void()> &drawFn, const std::function<void( glm::vec2, glm::vec2, glm::vec2, glm::vec2 )> &drawBgFn = nullptr, const std::function<void( glm::vec2, glm::vec2, glm::vec2, glm::vec2 )> &drawPostFn = nullptr );

	ci::app::WindowRef mWindowRef = nullptr;
	ci::fs::path mSaveImagePath;
	std::string mSaveImageName;
	std::string mSaveImageExtension;
	reza::tiler::TilerRef mTilerRef;

	int mTotalFrames = 120;
	int mCurrentFrame = 0;
	float mCurrentTime = 0;
	float mProgress = 0.0f;
	bool mRecording = false;
	int mSizeMultiplier = 4;

	ci::CameraPersp mCam;
	std::function<void()> mDrawFn;
	std::function<void( glm::vec2, glm::vec2, glm::vec2, glm::vec2 )> mDrawBgFn;
	std::function<void( glm::vec2, glm::vec2, glm::vec2, glm::vec2 )> mDrawPostFn;
};
} // namespace seq
} // namespace reza