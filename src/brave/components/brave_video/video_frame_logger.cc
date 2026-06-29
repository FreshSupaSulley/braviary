#include "brave/components/brave_video/video_frame_logger.h"
#include "base/logging.h"

namespace brave {

VideoFrameLogger::VideoFrameLogger() = default;
VideoFrameLogger::~VideoFrameLogger() = default;

void VideoFrameLogger::OnFrame(scoped_refptr<media::VideoFrame> frame) {
  frame_count_++;
  VLOG(1) << "Frame #" << frame_count_
          << " | Size: " << frame->coded_size().ToString()
          << " | Timestamp: " << frame->timestamp()
          << " | Format: " << frame->format();
}

}  // namespace brave