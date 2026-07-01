#include "brave/components/brave_video/video_frame_logger.h"

#include "base/logging.h"
#include "media/base/video_types.h"

namespace brave {

VideoFrameLogger::VideoFrameLogger() = default;
VideoFrameLogger::~VideoFrameLogger() = default;

void VideoFrameLogger::OnFrame(scoped_refptr<media::VideoFrame> frame) {
  ++frame_count_;
  VLOG(1) << "Frame #" << frame_count_
          << " | " << frame->coded_size().ToString()
          << " | visible=" << frame->visible_rect().ToString()
          << " | ts=" << frame->timestamp()
          << " | fmt=" << VideoPixelFormatToString(frame->format());
}

}  // namespace brave
